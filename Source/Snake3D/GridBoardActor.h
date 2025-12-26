// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridBoardActor.generated.h"

UCLASS()
class SNAKE3D_API AGridBoardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridBoardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> Tiles;
	
	UPROPERTY(EditAnywhere, Category="Board Visual")
	float ZOffset = 0.f;

	UPROPERTY(EditAnywhere, Category="Board Visual")
	float TileScale = 1.f;

	UPROPERTY(EditAnywhere, Category="Board Visual")
	bool bRebuildInEditor = true;

	void Rebuild() const;
};
