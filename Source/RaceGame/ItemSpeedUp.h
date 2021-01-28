// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemSpeedUp.generated.h"

/**
 * 
 */
UCLASS()
class RACEGAME_API AItemSpeedUp : public AItemBase
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	AItemSpeedUp();

	virtual void BeginPlay() override;

	virtual void ItemHit(AMyRaceGamePawn* HitPawn);
	virtual void ItemHit(AMyPawn* HitPawn);

};
