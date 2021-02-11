// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ItemSpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UItemSpawnComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSpawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//----�ϐ�
	APawn* OwnerPawn;

	//�A�C�e����ID
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 ItemNumber;

	//�A�C�e�����g�p�ł����Ԃ��m�F����ϐ�
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		bool bIsItemUse;

	//�X�|�[��������ۂ�Scale
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float ItemScale;

	//Spanw������A�C�e��
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemClass;


	//�A�C�e���̉摜
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		class UTexture2D* DrawIcon;

	//�A�C�e���̎��
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> BulletItem;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SlipItem;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SpeedUpItem;

	//�f�[�^�e�[�u��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
		class UDataTable* ItemDataTable;




protected:
	//----�֐�
	UFUNCTION()
		void SpawnItem();

	//�A�C�e�����g�p�����Ƃ��̃��\�b�h
	UFUNCTION()
		void ItemUse();

	//�A�C�e�����X�|�[��������ۂ̐ݒ�
	UFUNCTION()
		void ItemSetting(int32 ItemNum);

	UFUNCTION(Server, reliable)
		void ItemSettingRunOnServer(int32 ItemNum);

	UFUNCTION()
		void SpawnSetting();
	UFUNCTION(Server, reliable)
		void SpawnSettingRunOnServer();

public:

	//�A�C�e�����o�������郁�\�b�h
	UFUNCTION(NetMulticast, reliable)
		void SpawnItemMulticast();

	UFUNCTION(Server, reliable, WithValidation)
		void SpawnItemRunonServer();

	//�A�C�e�����擾����Ƃ��ɌĂ΂�郁�\�b�h
	UFUNCTION()
		void ItemPickup(int ItemNum);


	//�Z�b�^�[�Q�b�^�[
	UFUNCTION()
		void SetItemUse(bool value) { bIsItemUse = value; }

		
};