// Fill out your copyright notice in the Description page of Project Settings.

#include "Snake.h"
#include "SnakeSegment.h"
#include "Buff/SnakeBuff.h"
#include "Effect/SnakeEffect.h"
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

// Called every frame
void ASnake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnake::Initialize()
{
	HeadGrid = {0, 0};
	BodyGrids = { {-1, 0}, {-2, 0} };
	PrevHeadGrid = HeadGrid;
}

void ASnake::SetDirection(ESnakeDirection NewDirection)
{
	Direction = NewDirection;
}

void ASnake::StepMove()
{
	PrevHeadGrid = HeadGrid;
	
	FIntPoint NewHeadGrid(0, 0);
	switch (Direction)
	{
	case ESnakeDirection::Left:
		NewHeadGrid = HeadGrid + FIntPoint(0, -1);
		break;
	case ESnakeDirection::Right:
		NewHeadGrid = HeadGrid + FIntPoint(0, 1);
		break;
	case ESnakeDirection::Up:
		NewHeadGrid = HeadGrid + FIntPoint(1, 0);
		break;
	case ESnakeDirection::Down:
		NewHeadGrid = HeadGrid + FIntPoint(-1, 0);
		break;
	}
	
	BodyGrids.Insert(HeadGrid, 0);
	HeadGrid = NewHeadGrid;
	
	if (!bPendingGrow)
	{
		BodyGrids.Pop();
	}
	else
	{
		bPendingGrow = false;
	}
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
		Head->SetFacingDirection(HeadGrid - PrevHeadGrid);
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

void ASnake::Restart()
{
	for (AActor* Seg : Segments)
	{
		if (Seg) Seg->Destroy();
	}
	Segments.Empty();
	
	HeadGrid = FIntPoint(0, 0);
	BodyGrids = {
		FIntPoint(-1, 0),
		FIntPoint(-2, 0)
	};
	PrevHeadGrid = HeadGrid;
	
	SpawnInitialSegments();
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
