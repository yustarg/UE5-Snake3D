// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SnakeBuff.generated.h"

class ASnake;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SNAKE3D_API USnakeBuff : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(ASnake* InOwner);
	virtual void OnApply(); 	
	virtual void OnExpire();
	void Tick(float DeltaTime);
	
	bool IsExpired() const { return RemainingTime <= 0.f; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Buff")
	float Duration = 5.f;

	float RemainingTime;

	UPROPERTY()
	TObjectPtr<ASnake> Owner;
};
