// Fill out your copyright notice in the Description page of Project Settings.


#include "GridBoardActor.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
AGridBoardActor::AGridBoardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Tiles = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Tiles"));
	Tiles->SetupAttachment(Root);
	Tiles->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Tiles->SetMobility(EComponentMobility::Static);
}

// Called when the game starts or when spawned
void AGridBoardActor::BeginPlay()
{
	Super::BeginPlay();
	
	Rebuild();
}

void AGridBoardActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (bRebuildInEditor)
	{
		Rebuild();
	}
}

void AGridBoardActor::Rebuild() const
{
	UWorld* World = GetWorld();
	if (!World || !Tiles) return;
	
	USnakeGridSubsystem* GridSubsystem = World->GetSubsystem<USnakeGridSubsystem>();
	if (!GridSubsystem) return;

	Tiles->ClearInstances();
	
	const int32 GridSize = GridSubsystem->GetGridSize();
	const FIntPoint Min = GridSubsystem->GetGridMin();
	const FIntPoint Max = GridSubsystem->GetGridMax();
	
	const float UniformScale = (GridSize / 100.f) * TileScale;
	
	for (int32 X = Min.X; X <= Max.X; ++X)
	{
		for (int32 Y = Min.Y; Y <= Max.Y; ++Y)
		{
			const FIntPoint Cell(X, Y);

			FVector Center = GridSubsystem->GridToWorld(Cell);
			Center.Z += ZOffset;

			FTransform T;
			T.SetLocation(Center);
			T.SetRotation(FQuat::Identity);
			T.SetScale3D(FVector(UniformScale, UniformScale, 1.f));

			Tiles->AddInstance(T);
		}
	}
}

