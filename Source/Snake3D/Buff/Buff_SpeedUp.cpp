// Fill out your copyright notice in the Description page of Project Settings.


#include "Buff_SpeedUp.h"
#include "Snake3D/Snake.h"

void UBuff_SpeedUp::OnApply()
{
	OldMoveInterval = Owner->GetMoveInterval();
	Owner->SetMoveInterval(OldMoveInterval * 0.8f);
}

void UBuff_SpeedUp::OnExpire()
{
	Owner->SetMoveInterval(OldMoveInterval);
}
