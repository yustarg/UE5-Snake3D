// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakePlayerController.h"

#include "CameraRig.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Snake.h"
#include "Subsystems/LocalPlayer/SnakeCameraSubsystem.h"
#include "SnakeGameManager.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

void ASnakePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	CacheSnakeGameManager();
	BindGameState();
	ApplyInitialCamera();
	BindInput();
}

void ASnakePlayerController::CacheSnakeGameManager()
{
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASnakeGameManager::StaticClass(), Found);
	
	if (Found.Num() > 0)
	{
		SnakeGameManager = Cast<ASnakeGameManager>(Found[0]);
	}
}

void ASnakePlayerController::ApplyInitialCamera()
{
	if (auto* LP = GetLocalPlayer())
	{
		if (auto* CamSys = LP->GetSubsystem<USnakeCameraSubsystem>())
		{
			if (auto* Rig = CamSys->GetCurrentCameraRig())
			{
				if (auto* Cam = Rig->GetCameraActor())
				{
					FViewTargetTransitionParams Params;
                    Params.BlendTime = 0.5f;
                    Params.BlendFunction = VTBlend_EaseInOut;
					SetViewTarget(Cam, Params);
				}
			}
			CamSys->BindToGameManager(SnakeGameManager);
		}
	}
}

void ASnakePlayerController::BindInput()
{
	if (!IMC_Snake)
	{
		UE_LOG(LogTemp, Error, TEXT("IMC_Snake is NULL. Did you set it in BP_SnakePlayerController?"));
		return;
	}
	
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		if (auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(IMC_Snake, 0);
		}
	}
	
	if (auto* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(IA_Up, ETriggerEvent::Triggered, this, &ASnakePlayerController::OnUp);
		EIC->BindAction(IA_Down, ETriggerEvent::Triggered, this, &ASnakePlayerController::OnDown);
		EIC->BindAction(IA_Right, ETriggerEvent::Triggered, this, &ASnakePlayerController::OnRight);
		EIC->BindAction(IA_Left, ETriggerEvent::Triggered, this, &ASnakePlayerController::OnLeft);
		EIC->BindAction(IA_Restart, ETriggerEvent::Triggered, this, &ASnakePlayerController::OnRestart);
	}
}

void ASnakePlayerController::OnUp(const FInputActionValue& Value)
{
	if (SnakeGameManager->PlayerSnake->Direction != ESnakeDirection::Down)
	{
		SnakeGameManager->PlayerSnake->SetDirection(ESnakeDirection::Up);
	}
}

void ASnakePlayerController::OnDown(const FInputActionValue& Value)
{
	if (SnakeGameManager->PlayerSnake->Direction != ESnakeDirection::Up)
	{
		SnakeGameManager->PlayerSnake->SetDirection(ESnakeDirection::Down);
	}
}

void ASnakePlayerController::OnLeft(const FInputActionValue& Value)
{
	if (SnakeGameManager->PlayerSnake->Direction != ESnakeDirection::Right)
	{
		SnakeGameManager->PlayerSnake->SetDirection(ESnakeDirection::Left);
	}
}

void ASnakePlayerController::OnRight(const FInputActionValue& Value)
{
	if (SnakeGameManager->PlayerSnake->Direction != ESnakeDirection::Left)
	{
		SnakeGameManager->PlayerSnake->SetDirection(ESnakeDirection::Right);
	}
}

void ASnakePlayerController::OnRestart(const FInputActionValue& Value)
{
	if (SnakeGameManager && SnakeGameManager->GameState == ESnakeGameState::GameOver)
	{
		SnakeGameManager->RestartGame();
	}
}

void ASnakePlayerController::BindGameState()
{
	SnakeGameManager->OnGameStateChanged.AddUObject(this, &ASnakePlayerController::OnGameStateChanged);
}

void ASnakePlayerController::OnGameStateChanged(ESnakeGameState NewState)
{
	if (NewState == ESnakeGameState::GameOver)
	{
		SetIgnoreMoveInput(true);
		SetIgnoreLookInput(true);
	}
	else
	{
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
	}
}
