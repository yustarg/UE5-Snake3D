// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Snake.generated.h"

class ISnakeDirectionProvider;
class ASnake;
class USnakeBuff;
enum class ESnakeEffectType : uint8;
class USnakeGridSubsystem;

DECLARE_MULTICAST_DELEGATE(FOnSnakeSpeedChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSnakeDied, ASnake*);

UCLASS()
class SNAKE3D_API ASnake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnake();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	/* ===== 生命周期 ===== */
	void Initialize(FIntPoint InHeadGrid);
	bool IsAlive() const { return bAlive; }
	void Die();
	void Restart(FIntPoint InHeadGrid);
	FOnSnakeDied OnDied;
	
	/* ===== Step 驱动 ===== */
	void TickLogic(float DeltaTime);
	bool IsMovingThisStep() const { return bMoveThisStep; }
	FOnSnakeSpeedChanged OnSpeedChanged;

	/* ===== 移动 ===== */
	void ComputeNextHead();
	void ApplyMoveWithTarget(const FIntPoint& Target);
	void ApplyMove();
	float GetMoveInterval() const { return MoveInterval; }
	void SetMoveInterval(float NewMoveInterval);
	void SyncSegments();

	/* ===== 方向 ===== */
	FIntPoint Direction = {1, 0};
	void SetDirectionProvider(const TScriptInterface<ISnakeDirectionProvider>& InProvider);
	
	/* ===== Grid 数据 ===== */
	const FIntPoint& GetHead() const {return HeadGrid;}
	const FIntPoint& GetNextHeadGrid() const {return NextHeadGrid;}
	const TArray<FIntPoint>& GetBody() const {return BodyGrids;}
	TArray<FIntPoint> GetHeadAndBody() const;
	
	// Effect & Buff Related
	void ApplyEffect(ESnakeEffectType Effect);
	void AddBuff(TSubclassOf<USnakeBuff> BuffClass);
	void TickBuffs(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/* ===== 状态 ===== */
	bool bAlive = true;
	
	/* ===== 移动节奏 ===== */
	TScriptInterface<ISnakeDirectionProvider> DirectionProvider;
	float MoveInterval = 0.25f;
	float MoveAccumulator = 0.f;
	float MinInterval = 0.01f;
	float MaxInterval = 1.f;
	bool  bMoveThisStep = false;
	
	/* ===== Grid ===== */
	UPROPERTY()
	USnakeGridSubsystem* GridSubsystem;
	FIntPoint HeadGrid;
	FIntPoint NextHeadGrid;
	TArray<FIntPoint> BodyGrids;
	
	/* ===== Visual ===== */
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Segments;
	
	UPROPERTY(EditDefaultsOnly, Category="Snake")
	TSubclassOf<AActor> SnakeSegmentClass;
	void SpawnInitialSegments();
	void Grow();
	bool bPendingGrow = false;
	
	/* ===== Buff ===== */
	UPROPERTY()
	TArray<TObjectPtr<USnakeBuff>> ActiveBuffs;
};
