// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeSegment.generated.h"

UENUM(BlueprintType)
enum class ESnakeSegmentType : uint8
{
	Head,
	Body
};

UCLASS()
class SNAKE3D_API ASnakeSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeSegment();

	void SetSegmentType(ESnakeSegmentType SegmentType);
	void SetFacingDirection(const FIntPoint& Dir);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Visual")
	UStaticMesh* HeadMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Visual")
	UStaticMesh* BodyMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Visual")
	UMaterialInterface* HeadMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category="Visual")
	UMaterialInterface* BodyMaterial;
};
