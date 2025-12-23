// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SnakePlayerController.generated.h"

enum class ESnakeGameState : uint8;
struct FInputActionValue;
class ASnakeGameManager;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class SNAKE3D_API ASnakePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Snake;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Up;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Down;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Left;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Right;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Restart;

private:
	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TSubclassOf<ACameraActor> CameraActorClass;
	
	UPROPERTY()
	ASnakeGameManager* SnakeGameManager;
	
	void CacheSnakeGameManager();
	void ApplyInitialCamera();
	
	void BindInput();
	void OnUp(const FInputActionValue& Value);
	void OnDown(const FInputActionValue& Value);
	void OnLeft(const FInputActionValue& Value);
	void OnRight(const FInputActionValue& Value);
	void OnRestart(const FInputActionValue& Value);
	
	void BindGameState();
	void OnGameStateChanged(ESnakeGameState NewState);
};
