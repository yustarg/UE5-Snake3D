// Fill out your copyright notice in the Description page of Project Settings.

#include "Snake.h"
#include "SnakeSegment.h"
#include "Buff/SnakeBuff.h"
#include "Effect/SnakeEffect.h"
#include "Interface/SnakeDirectionProvider.h"
#include "Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
ASnake::ASnake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnake::BeginPlay()
{
	Super::BeginPlay();
	
	GridSubsystem = GetWorld()->GetSubsystem<USnakeGridSubsystem>();
}

void ASnake::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UE_LOG(LogTemp, Warning, TEXT("Snake EndPlay: %s"), *GetName());
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	for (const auto Segment : Segments)
	{
		Segment->Destroy();
	}

	Segments.Empty();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnake::Initialize(const FIntPoint InHeadGrid)
{
	HeadGrid = InHeadGrid;
	//BodyGrids = { {-1, 0}, {-2, 0} };
	NextHeadGrid = HeadGrid;
}

void ASnake::SyncSegments()
{
	const int32 SegmentsNeeded = BodyGrids.Num() + 1;

	while (Segments.Num() < SegmentsNeeded)
	{
		if (!SnakeSegmentClass) break;
		
		AActor* Seg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, 
			FVector::Zero(), FRotator::ZeroRotator);
		Segments.Add(Seg);
	}
	
	while (Segments.Num() > SegmentsNeeded)
	{
		if (AActor* Seg = Segments.Last())
		{
			Seg->Destroy();
		}
		Segments.Pop();
	}
	
	// Head
	if (Segments.Num() > 0)
	{
		ASnakeSegment* Head = Cast<ASnakeSegment>(Segments[0]);
		Head->SetSegmentType(ESnakeSegmentType::Head);
		Head->SetFacingDirection(Direction);
		Head->SetActorLocation(GridSubsystem->GridToWorld(HeadGrid));
	}
	
	// Body
	for (int i = 0; i < BodyGrids.Num(); ++i)
	{
		const int SegIndex = i + 1;

		if (ASnakeSegment* Body = Cast<ASnakeSegment>(Segments[SegIndex]))
		{
			Body->SetSegmentType(ESnakeSegmentType::Body);
			Body->SetActorLocation(GridSubsystem->GridToWorld(BodyGrids[i]));
		}
	}
}

void ASnake::SetDirectionProvider(const TScriptInterface<ISnakeDirectionProvider>& InProvider)
{
	DirectionProvider = InProvider;
}

void ASnake::Die()
{    
	if (!bAlive) return;
	bAlive = false;
	OnDied.Broadcast(this);
}

void ASnake::Restart(FIntPoint InHeadGrid)
{
	for (AActor* Seg : Segments)
	{
		if (Seg) Seg->Destroy();
	}
	Segments.Empty();
	
	HeadGrid = InHeadGrid;
	NextHeadGrid = HeadGrid;
	
	SpawnInitialSegments();
}

void ASnake::TickLogic(const float DeltaTime)
{
	if (!bAlive)
	{
		bMoveThisStep = false;
		return;
	}

	MoveAccumulator += DeltaTime;
	if (MoveAccumulator >= MoveInterval)
	{
		MoveAccumulator -= MoveInterval;
		bMoveThisStep = true;
	}
	else
	{
		bMoveThisStep = false;
	}
}

void ASnake::ComputeNextHead()
{
	if (!bAlive || !bMoveThisStep) return;

	if (DirectionProvider)
	{
		Direction = DirectionProvider->GetNextDirection(this);
	}
	
	NextHeadGrid = HeadGrid + Direction;
}

void ASnake::ApplyMoveWithTarget(const FIntPoint& Target)
{
	if (!bAlive || !bMoveThisStep) return;
	BodyGrids.Insert(HeadGrid, 0);
	HeadGrid = Target;
	
	if (!bPendingGrow)
	{
		BodyGrids.Pop();
	}
	else
	{
		bPendingGrow = false;
	}
}

void ASnake::ApplyMove()
{
	if (!bAlive || !bMoveThisStep) return;

	BodyGrids.Insert(HeadGrid, 0);
	HeadGrid = NextHeadGrid;
	
	if (!bPendingGrow)
	{
		BodyGrids.Pop();
	}
	else
	{
		bPendingGrow = false;
	}
}

TArray<FIntPoint> ASnake::GetHeadAndBody() const
{
	TArray<FIntPoint> Result;
	Result.Reserve(1 + BodyGrids.Num());

	Result.Add(HeadGrid);
	Result.Append(BodyGrids);

	return Result;
}

void ASnake::ApplyEffect(const ESnakeEffectType Effect)
{
	switch (Effect)
	{
		case ESnakeEffectType::Grow:
		Grow();
		break;
		default:
		break;
	}
}

void ASnake::AddBuff(TSubclassOf<USnakeBuff> BuffClass)
{
	if (!BuffClass) return;
	
	USnakeBuff* Buff = NewObject<USnakeBuff>(this, BuffClass);
	Buff->Initialize(this);
	ActiveBuffs.Add(Buff);
}

void ASnake::TickBuffs(float DeltaTime)
{
	for (int i = ActiveBuffs.Num() - 1; i >= 0; --i)
	{
		ActiveBuffs[i]->Tick(DeltaTime);
		if (ActiveBuffs[i]->IsExpired())
		{
			ActiveBuffs.RemoveAt(i);
		}
	}
}

void ASnake::SetMoveInterval(float NewMoveInterval)
{
	if (FMath::IsNearlyEqual(NewMoveInterval, MoveInterval)) return;
	
	MoveInterval = FMath::Clamp(NewMoveInterval, MinInterval, MaxInterval);
	OnSpeedChanged.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("SetMoveInterval %f"), MoveInterval);
}

void ASnake::Grow()
{
	bPendingGrow = true;
}

void ASnake::SpawnInitialSegments()
{
	if (!SnakeSegmentClass) return;

	for (auto Seg : Segments)
	{
		if (Seg) Seg->Destroy();
	}
	Segments.Empty();
	
	FVector HeadPos = GridSubsystem->GridToWorld(HeadGrid);
	AActor* HeadSeg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, HeadPos, FRotator::ZeroRotator);
	Segments.Add(HeadSeg);
	
	for (const FIntPoint& BodyGrid : BodyGrids)
	{
		FVector BodyPos = GridSubsystem->GridToWorld(BodyGrid);
		AActor* BodySeg = GetWorld()->SpawnActor<AActor>(SnakeSegmentClass, BodyPos, FRotator::ZeroRotator);
		Segments.Add(BodySeg);
	}
}
