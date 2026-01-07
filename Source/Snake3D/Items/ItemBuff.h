// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SnakeItem.h"
#include "ItemBuff.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE3D_API AItemBuff : public ASnakeItem
{
	GENERATED_BODY()

public:
	virtual void OnConsumed(ASnake* Snake) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Item")
	TSubclassOf<USnakeBuff> BuffClass;

};
