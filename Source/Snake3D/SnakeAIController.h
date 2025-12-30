// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SnakeDirectionProvider.h"
#include "SnakeAIController.generated.h"

UCLASS()
class SNAKE3D_API ASnakeAIController : public AActor, public ISnakeDirectionProvider
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeAIController();
	
	virtual FIntPoint GetNextDirection(const ASnake* Snake) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
