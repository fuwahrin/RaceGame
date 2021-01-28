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
	//�R���X�g���N�^
	AItemBullet();

	
	virtual void BeginPlay() override;

	virtual void ItemHit(AMyRaceGamePawn *HitPawn) override;
	virtual void ItemHit(AMyPawn* HitPawn) override;


protected:
	//--------�R���|�[�l���g
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* BulletMovement;



	

};
