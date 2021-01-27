// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemSlip.generated.h"

/**
 * 
 */
UCLASS()
class RACEGAME_API AItemSlip : public AItemBase
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	AItemSlip();

	virtual void BeginPlay() override;

	virtual void ItemHit(AMyRaceGamePawn* HitPawn) override;

};
