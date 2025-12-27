// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake3D/Effect/SnakeEffect.h"
#include "SnakeItem.generated.h"

class ASnake;

UCLASS(Abstract)
class SNAKE3D_API ASnakeItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FIntPoint Grid;
	
	UPROPERTY(EditDefaultsOnly, Category="Item")
	ESnakeEffectType Effect;

public:	
	ESnakeEffectType GetEffect() const { return Effect; }
	void SetGrid(const FIntPoint& InGrid);
	FIntPoint GetGrid() const { return Grid; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
};
