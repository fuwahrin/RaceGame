// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class AMyRaceGamePawn;
class AMyPawn;

//�A�C�e���擾���ɌĂ΂�铮�I�f���Q�[�g
DECLARE_DYNAMIC_DELEGATE(FItemPickupDispather);


UCLASS()
class RACEGAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	//------------------�R���|�[�l���g
	
	//�����蔻��̃R���W����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		class USphereComponent  *ItemColision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *ItemMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		class URotatingMovementComponent *RotationMovement;

protected:
	//------------------���\�b�h
	
	//BeginOverlap
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
						  AActor* OtherActor,
						  UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex,
						  bool bFromSweep,
						  const FHitResult&SweepResult);

	//�A�C�e���̕\���E��\����ݒ肵�Ă��郁�\�b�h
	UFUNCTION()
		void ItemVisibility(bool bIsVisible);

	UFUNCTION()
		void ItemRespawn();

	//�A�C�e���������������ɔ�������C�x���g
	virtual void ItemHit(AMyRaceGamePawn *HitPawn);
	virtual void ItemHit(AMyPawn* HitPawn);

	

protected:
	//------------------�ϐ�
	

	//�A�C�e���̃��X�|�[�����\���ǂ���
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemSetting", meta = (AllowPrivateAccess = "true"))
		bool bIsItemRespawn;

	//�t�^����A�C�e���̔ԍ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemSetting", meta = (AllowPrivateAccess = "true"))
		int32 SelectItemNumber;

	//�A�C�e���ďo���̃^�C���n���h��
	FTimerHandle RespawnHandler;

public:
	//----�C�x���g�f�B�X�p�b�`���[�Ăяo���p�i�Ăяo�����FMyRaceGamePawn ItemPickup�j
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	FItemPickupDispather ItemPickupDispather;


};
