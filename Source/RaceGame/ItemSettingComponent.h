// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSettingComponent.generated.h"


USTRUCT()
struct FItemState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool IsUse;


	UPROPERTY()
	int32 ItemNum;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UItemSettingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSettingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:

	//�R���|�[�l���g
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ItemSpawnPoint;

private:
	//----�ϐ�
	FVector SpawnPoint;

protected:
	//----�ϐ�
	APawn *OwnerPawn;

	//�A�C�e����ID
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 ItemNumber;

	//�A�C�e�����g�p�ł����Ԃ��m�F����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool bIsItemUse;

	//�X�|�[��������ۂ�Scale
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	float ItemScale;

	//Spanw������A�C�e��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemClass;


	//�A�C�e���̉摜
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		class UTexture2D* DrawIcon;

	//�A�C�e���̎��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> BulletItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SlipItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SpeedUpItem;

	//�f�[�^�e�[�u��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
		class UDataTable* ItemDataTable;

	class UMyCartMoveComponentReplicator *MoveReplicateComponent;

	//�T�[�o�[�̏��
	UPROPERTY(Replicated)
	FItemState ServerItemState;

	FItemState LocalItemState;

	UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendItemSpawn(int itemnum);

	UFUNCTION()
	void ItemCreate(int32 ItemNum);






protected:
	//----�֐�
	//�A�C�e�����g�p�����Ƃ��̃��\�b�h
	UFUNCTION()
		void ItemUse();

	//�A�C�e�����X�|�[��������ۂ̐ݒ�
	UFUNCTION()
		void ItemSetting(int32 ItemNum);

	
	UFUNCTION()
		void SpawnSetting();
	
public:
	UFUNCTION()
	void SpawnItem();


	//�A�C�e�����擾����Ƃ��ɌĂ΂�郁�\�b�h
	UFUNCTION()
		void ItemPickup(int ItemNum);
	
	//�Z�b�^�[�Q�b�^�[
	UFUNCTION()
		void SetItemUse(bool value) { bIsItemUse = value; }

	UFUNCTION()
		FString GetRole(ENetRole role);
	

	
};
