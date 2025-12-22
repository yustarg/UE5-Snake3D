// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "SnakeCameraSubsystem.generated.h"

class ACameraRig;

/**
 * 
 */
UCLASS()
class SNAKE3D_API USnakeCameraSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterCameraRig(ACameraRig* CameraRig);
	ACameraRig* GetCurrentCameraRig() const;
	
private:
	UPROPERTY()
	TObjectPtr<ACameraRig> CurrentRig;
	
};
