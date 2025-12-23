// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeSegment.h"

// Sets default values
ASnakeSegment::ASnakeSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASnakeSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASnakeSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeSegment::SetSegmentType(const ESnakeSegmentType SegmentType)
{
	if (!StaticMesh) return;
	
	switch (SegmentType)
	{
	case ESnakeSegmentType::Head:
		if (HeadMesh) StaticMesh->SetStaticMesh(HeadMesh);
		if (HeadMaterial) StaticMesh->SetMaterial(0, HeadMaterial);
		break;
	case ESnakeSegmentType::Body:
		if (BodyMesh) StaticMesh->SetStaticMesh(BodyMesh);
		if (BodyMaterial) StaticMesh->SetMaterial(0, BodyMaterial);
		break;
	}
}

void ASnakeSegment::SetFacingDirection(const FIntPoint& Dir)
{
	FRotator Rot = FRotator::ZeroRotator;
	
	if (Dir == FIntPoint(1, 0))       Rot.Yaw = 0.f;     // Right
	else if (Dir == FIntPoint(-1, 0)) Rot.Yaw = 180.f;   // Left
	else if (Dir == FIntPoint(0, 1))  Rot.Yaw = 90.f;    // Up
	else if (Dir == FIntPoint(0, -1)) Rot.Yaw = -90.f;   // Down

	SetActorRotation(Rot);
}

