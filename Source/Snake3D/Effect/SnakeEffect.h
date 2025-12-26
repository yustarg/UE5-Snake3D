// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESnakeEffectType  : uint8
{
	None		UMETA(DisplayName="None"),
	Grow        UMETA(DisplayName="Grow"),
	SpeedUp     UMETA(DisplayName="Speed Up"),
	SpeedDown   UMETA(DisplayName="Speed Down"),
};

/**
 * 
 */
class SNAKE3D_API SnakeEffect
{
public:
	SnakeEffect();
	~SnakeEffect();
};
