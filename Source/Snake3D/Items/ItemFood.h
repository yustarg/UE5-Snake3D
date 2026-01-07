// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeItem.h"
#include "ItemFood.generated.h"

UCLASS()
class SNAKE3D_API AItemFood : public ASnakeItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemFood();

	virtual void OnConsumed(ASnake* Snake) override;
};
