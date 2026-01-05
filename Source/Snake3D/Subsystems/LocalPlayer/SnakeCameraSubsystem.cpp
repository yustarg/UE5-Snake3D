// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeCameraSubsystem.h"

#include "Camera/CameraActor.h"
#include "Snake3D/CameraRig.h"
#include "Snake3D/GameState/SnakeGameState.h"

void USnakeCameraSubsystem::BindToGameState()
{
	if (ASnakeGameState* GS = GetWorld()->GetGameState<ASnakeGameState>())
	{
		GS->OnMatchStateChanged.AddUObject(this, &USnakeCameraSubsystem::HandleGameStateChanged);
		HandleGameStateChanged(GS->GetMatchState());
	}
}

void USnakeCameraSubsystem::SetCameraViewTarget() const
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (const auto* Rig = GetCurrentCameraRig())
	{
		if (auto* Cam = Rig->GetCameraActor())
		{
			FViewTargetTransitionParams Params;
			Params.BlendTime = 0.5f;
			Params.BlendFunction = VTBlend_EaseInOut;
			LocalPlayer->GetPlayerController(GetWorld())->SetViewTarget(Cam, Params);
		}
	}
}

void USnakeCameraSubsystem::HandleGameStateChanged(const ESnakeMatchState NewGameState) const
{
	if (NewGameState == ESnakeMatchState::Playing)
	{
		SetCameraViewTarget();
	}
}

void USnakeCameraSubsystem::RegisterCameraRig(ACameraRig* CameraRig)
{
	CurrentRig = CameraRig;
}


