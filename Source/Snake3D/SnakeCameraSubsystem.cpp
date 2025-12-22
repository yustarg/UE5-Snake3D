// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraSubsystem.h"

ACameraRig* USnakeCameraSubsystem::GetCurrentCameraRig() const
{
	return CurrentRig;
}

void USnakeCameraSubsystem::RegisterCameraRig(ACameraRig* CameraRig)
{
	CurrentRig = CameraRig;
}


