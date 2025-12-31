// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraSubsystem.h"
#include "Snake3D/GameState/SnakeGameState.h"

ACameraRig* USnakeCameraSubsystem::GetCurrentCameraRig() const
{
	return CurrentRig;
}

void USnakeCameraSubsystem::BindToGameState()
{
	// if (const UWorld* World = GetWorld())
	// {
	// 	World->GetGameState<ASnakeGameState>()
	// 		->OnMatchStateChanged.AddUObject(
	// 			this,
	// 			&USnakeCameraSubsystem::HandleGameStateChanged
	// 		);
	// }
}

void USnakeCameraSubsystem::HandleGameStateChanged(ESnakeMatchState NewGameState)
{
	// TODO
	// switch (NewGameState)
	// {
	// case ESnakeMatchState::Playing:
	// 	SetCameraMode(ECameraMode::Gameplay);
	// 	break;
	//
	// case ESnakeMatchState::GameOver:
	// 	SetCameraMode(ECameraMode::GameOver);
	// 	break;
	// }
}

void USnakeCameraSubsystem::RegisterCameraRig(ACameraRig* CameraRig)
{
	CurrentRig = CameraRig;
}


