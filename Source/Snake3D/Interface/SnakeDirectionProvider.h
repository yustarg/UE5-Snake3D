// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeDirectionProvider.generated.h"

class ASnake;

UINTERFACE()
class SNAKE3D_API USnakeDirectionProvider : public UInterface
{
	GENERATED_BODY()
};
	
class SNAKE3D_API ISnakeDirectionProvider
{
	GENERATED_BODY()

public:
	// 返回一个 Grid 方向，例如 (1,0) (0,1)
	virtual FIntPoint GetNextDirection(const ASnake* Snake) = 0;
};
