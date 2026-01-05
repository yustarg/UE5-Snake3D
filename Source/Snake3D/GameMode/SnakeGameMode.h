// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SnakeGameMode.generated.h"

class ASnakeGameManager;
enum class ESnakeMatchState : uint8;
/**
 * 
 */
UCLASS()
class SNAKE3D_API ASnakeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void SetMatchState(ESnakeMatchState NewState) const;
	
	UFUNCTION(BlueprintCallable)
	void RequestRestart();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void OnEnterWaiting() const;
	void OnEnterPlaying() const;
	void OnEnterGameOver() const;
	
private:
	UPROPERTY()
	TObjectPtr<ASnakeGameManager> GameManager;
};
