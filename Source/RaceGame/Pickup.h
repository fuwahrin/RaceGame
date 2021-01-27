// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

private:
	//���x����ŏE���郁�b�V��
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
public:

	//�E���郁�b�V����Ԃ�
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return PickupMesh; };

	//���̃A�C�e�����E���邩�ǂ����̊m�F�p
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	//�A�C�e�����E���邩�ǂ�����ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	//�A�C�e��������ɌĂяo�����
	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	virtual void WasCollected_Implementation();


protected:

	//���̃A�C�e�����E���邩�ǂ���
	UPROPERTY()
	bool bIsActive;
	
};
