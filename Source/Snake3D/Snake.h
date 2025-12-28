// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake.generated.h"

class USnakeBuff;
enum class ESnakeEffectType : uint8;
class USnakeGridSubsystem;

UENUM(BlueprintType)
enum class ESnakeDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

DECLARE_MULTICAST_DELEGATE(FOnSnakeSpeedChanged);

UCLASS()
class SNAKE3D_API ASnake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FOnSnakeSpeedChanged OnSpeedChanged;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Initialize();
	void SetDirection(ESnakeDirection NewDirection);
	void StepMove();
	void SyncSegments();
	void Restart();

	ESnakeDirection Direction = ESnakeDirection::Right;
	const FIntPoint& GetHead() const {return HeadGrid;}
	const TArray<FIntPoint>& GetBody() const {return BodyGrids;}
	TArray<FIntPoint> GetHeadAndBody() const;
	
	// Effect & Buff Related
	void ApplyEffect(ESnakeEffectType Effect);
	
	void AddBuff(TSubclassOf<USnakeBuff> BuffClass);
	void TickBuffs(float DeltaTime);
	
	float GetMoveInterval() const { return MoveInterval; }
	void SetMoveInterval(float NewMoveInterval);
	
private:
	UPROPERTY()
	USnakeGridSubsystem* GridSubsystem;
	
	FIntPoint HeadGrid;
	FIntPoint PrevHeadGrid;
	TArray<FIntPoint> BodyGrids;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Segments;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	TSubclassOf<AActor> SnakeSegmentClass;
	
	void SpawnInitialSegments();
	
	void Grow();
	
	UPROPERTY()
	TArray<TObjectPtr<USnakeBuff>> ActiveBuffs;
	
	bool bPendingGrow = false;
	float MoveInterval = 0.25f;
	float MinInterval = 0.05f;
	float MaxInterval = 0.5f;
};
