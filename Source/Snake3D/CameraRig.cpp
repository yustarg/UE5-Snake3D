// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraRig.h"

#include "Subsystems/LocalPlayer/SnakeCameraSubsystem.h"

// Sets default values
ACameraRig::ACameraRig()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraRig::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto* LP = GetWorld()->GetFirstLocalPlayerFromController())
	{
		if (auto* sys = LP->GetSubsystem<USnakeCameraSubsystem>())
		{
			sys->RegisterCameraRig(this);
		}
	}
}

// Called every frame
void ACameraRig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ACameraActor* ACameraRig::GetCameraActor() const
{
	return Camera;
}

