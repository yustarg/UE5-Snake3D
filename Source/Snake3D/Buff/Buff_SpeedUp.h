// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeBuff.h"
#include "Buff_SpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API UBuff_SpeedUp : public USnakeBuff
{
	GENERATED_BODY()
	
public:
	virtual void OnApply() override;
	virtual void OnExpire() override;
	
private:
	float OldMoveInterval;
};
