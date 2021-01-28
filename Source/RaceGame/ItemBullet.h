// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemBullet.generated.h"


/**
 * 
 */
UCLASS()
class RACEGAME_API AItemBullet : public AItemBase
{
	GENERATED_BODY()
	
protected:
	//コンストラクタ
	AItemBullet();

	
	virtual void BeginPlay() override;

	virtual void ItemHit(AMyRaceGamePawn *HitPawn) override;
	virtual void ItemHit(AMyPawn* HitPawn) override;


protected:
	//--------コンポーネント
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* BulletMovement;



	

};
