// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeItemSpawner.generated.h"

class USnakeGridSubsystem;
class ASnakeItem;

UCLASS()
class SNAKE3D_API ASnakeItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Item")
	TSubclassOf<ASnakeItem> FoodClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Item")
	TSubclassOf<ASnakeItem> SpeedUpClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnFood();
	void RemoveItem(ASnakeItem* Item);
	
	UPROPERTY()
	TObjectPtr<ASnakeItem> CurrentFood;
	
private:
	UPROPERTY()
	USnakeGridSubsystem* GridSystem;
};
