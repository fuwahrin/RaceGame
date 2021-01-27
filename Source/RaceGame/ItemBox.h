// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemBox.generated.h"

/**
 * 
 */
UCLASS()
class RACEGAME_API AItemBox : public AItemBase
{
	GENERATED_BODY()

protected:
	//コンストラクタ
	AItemBox();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ItemHit(AMyRaceGamePawn* HitPawn) override;
	
};
