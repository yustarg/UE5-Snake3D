// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeGameManager.h"
#include "Snake.h"
#include "SnakeAIController.h"
#include "Interface/GameRule.h"
#include "Items/SnakeItem.h"
#include "Items/SnakeItemSpawner.h"
#include "Rules/WallWrapRule.h"
#include "Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
ASnakeGameManager::ASnakeGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASnakeGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
}

void ASnakeGameManager::Initialize()
{
	GridSubsystem = GetWorld()->GetSubsystem<USnakeGridSubsystem>();
	ItemSpawner->Initialize();
	// UWallWrapRule* WrapRule = NewObject<UWallWrapRule>(this);
	// ActiveRules.Add(WrapRule);
}

// Called every frame
void ASnakeGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeGameManager::Reset()
{
	Super::Reset();

	RestartGame();
}

void ASnakeGameManager::StartGame()
{
	SpawnPlayerSnake();
	SpawnAISnakes();
	SpawnItems();
	StartStepTimer();
}

void ASnakeGameManager::StopGame()
{
	// 1) 停止 Timer
	GetWorld()->GetTimerManager().ClearTimer(MoveTimer);
	
	// 2) UI / 结束流程（你自己接）
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
}

void ASnakeGameManager::RestartGame()
{
	CleanGame();
}

void ASnakeGameManager::CleanGame()
{
	GridSubsystem->Clear();
	for (const auto Snake : Snakes)
	{
		Snake->Destroy();
	}
	Snakes.Empty();
}

void ASnakeGameManager::StepMove()
{
	const float Delta = GlobalStepInterval;

	// ① Buff 时间
	for (ASnake* Snake : Snakes)
		Snake->TickBuffs(Delta);

	// ② 节拍判断
	for (ASnake* Snake : Snakes)
		Snake->TickLogic(Delta);

	// ③ PreStep：规则可修改行为
	for (ASnake* Snake : Snakes)
	{
		if (!Snake->IsAlive() || !Snake->IsMovingThisStep())
			continue;

		for (const auto& Rule : ActiveRules)
		{
			Rule->PreStep(this, Snake);
		}
	}

	// ④ 计算 NextHead
	for (ASnake* Snake : Snakes)
		Snake->ComputeNextHead();

	// ⑤ Occupancy
	GridSubsystem->RebuildDynamicOccupied();
	for (const ASnake* Snake : Snakes)
	{
		GridSubsystem->UpdateDynamicOccupied(Snake->GetHeadAndBody());
	}

	// ⑥ 判定 CanMove
	for (ASnake* Snake : Snakes)
	{
		if (!Snake->IsAlive() || !Snake->IsMovingThisStep())
			continue;

		FIntPoint Target = Snake->GetNextHeadGrid();
		bool bAllowed = true;

		for (const auto& Rule : ActiveRules)
		{
			if (!Rule->ModifyTarget(this, Snake, Target))
			{
				bAllowed = false;
				break;
			}
		}

		if (!bAllowed ||
			!GridSubsystem->IsInside(Snake->GetNextHeadGrid()) ||
			GridSubsystem->IsDynamicOccupied(Snake->GetNextHeadGrid()))
		{
			Snake->Die();
		}
		else
		{
			Snake->ApplyMoveWithTarget(Target);

			for (const auto& Rule : ActiveRules)
			{
				Rule->PostMove(this, Snake);
			}
			
			Snake->SyncSegments();
		}
	}

	if (PlayerSnake->GetHead() == ItemSpawner->CurrentItem->GetGrid())
	{
		if (ItemSpawner->CurrentItem->IsBuff())
		{
			PlayerSnake->AddBuff(ItemSpawner->CurrentItem->GetBuffClass());
		}
		else
		{
			PlayerSnake->ApplyEffect(ItemSpawner->CurrentItem->GetEffect());
		}
		
		ItemSpawner->SpawnRandomItem();
	}
}

void ASnakeGameManager::StartStepTimer()
{
	UWorld* World = GetWorld();
	if (!World) return;

	World->GetTimerManager().SetTimer(
		MoveTimer,
		this,
		&ASnakeGameManager::StepMove,
		GlobalStepInterval,
		true
	);
}

void ASnakeGameManager::SpawnPlayerSnake()
{
	const FIntPoint BirthPoint = GridSubsystem->GetRandomFreeCell();
	GridSubsystem->UpdateDynamicOccupied({BirthPoint});
	PlayerSnake = GetWorld()->SpawnActor<ASnake>(PlayerSnakeClass);
	PlayerSnake->Initialize(BirthPoint);
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PlayerSnake->SetDirectionProvider(PC);
	
	RegisterSnake(PlayerSnake);
}

void ASnakeGameManager::SpawnAISnakes()
{
	for (int32 i = 0; i < NumAI; ++i)
	{
		ASnake* Snake = GetWorld()->SpawnActor<ASnake>(PlayerSnakeClass);
		AActor* AIProvider = GetWorld()->SpawnActor<AActor>(AIControllerClass);
		
		const FIntPoint BirthPoint = GridSubsystem->GetRandomFreeCell();
		GridSubsystem->UpdateDynamicOccupied({BirthPoint});
		Snake->Initialize(BirthPoint);
		Snake->SetDirectionProvider(AIProvider);
		RegisterSnake(Snake);
	}
}

void ASnakeGameManager::SpawnItems() const
{
	if (ItemSpawner->CurrentItem)
	{
		ItemSpawner->RemoveItem(ItemSpawner->CurrentItem);
	}
	
	ItemSpawner->SpawnRandomItem();
}

void ASnakeGameManager::RegisterSnake(ASnake* InSnake)
{
	if (InSnake == nullptr) return;
	
	Snakes.Add(InSnake);

	InSnake->OnDied.AddUObject(this, &ASnakeGameManager::OnSnakeDied);
}

void ASnakeGameManager::OnSnakeDied(ASnake* Snake) const
{
	if (Snake == PlayerSnake)
	{
		OnPlayerSnakeDied.Broadcast();
	}
}

