// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeItem.h"

#include "Snake3D/Subsystems/World/SnakeGridSubsystem.h"

// Sets default values
ASnakeItem::ASnakeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMesh"));
	SetRootComponent(StaticMesh);
	StaticMesh->bEditableWhenInherited = true;
}

// Called when the game starts or when spawned
void ASnakeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnakeItem::OnConsumed(ASnake* Snake)
{
	OnItemConsumed.Broadcast(Snake, this);
}

void ASnakeItem::SetGrid(const FIntPoint& InGrid)
{
	Grid = InGrid;

	if (const UWorld* World = GetWorld())
	{
		if (const USnakeGridSubsystem* GridSys = World->GetSubsystem<USnakeGridSubsystem>())
		{
			SetActorLocation(GridSys->GridToWorld(Grid));
		}
	}
}

