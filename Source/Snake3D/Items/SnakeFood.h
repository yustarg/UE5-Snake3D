// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeItem.h"
#include "GameFramework/Actor.h"
#include "SnakeFood.generated.h"

UCLASS()
class SNAKE3D_API ASnakeFood : public ASnakeItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ApplyTo(ASnake* Snake) override;
};
