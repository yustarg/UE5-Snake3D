// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraSubsystem.h"
#include "Snake3D/SnakeGameManager.h"

ACameraRig* USnakeCameraSubsystem::GetCurrentCameraRig() const
{
	return CurrentRig;
}

void USnakeCameraSubsystem::BindToGameManager(ASnakeGameManager* GM)
{
	if (!GM) return;
	
	GM->OnGameStateChanged.AddUObject(this, &USnakeCameraSubsystem::HandleGameStateChanged);
}

void USnakeCameraSubsystem::HandleGameStateChanged(ESnakeGameState NewGameState)
{
	// switch (NewGameState)
	// {
	// case ESnakeGameState::Playing:
	// 	SetCameraMode(ECameraMode::Gameplay);
	// 	break;
	//
	// case ESnakeGameState::GameOver:
	// 	SetCameraMode(ECameraMode::GameOver);
	// 	break;
	// }
}

void USnakeCameraSubsystem::RegisterCameraRig(ACameraRig* CameraRig)
{
	CurrentRig = CameraRig;
}


