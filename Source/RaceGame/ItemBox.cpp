// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ItemSettingComponent.h"

//�R���X�g���N�^
AItemBox::AItemBox()
{
	//��]�̐ݒ�
	RotationMovement->RotationRate = FRotator(0.0f, 0.0f, 30.0f);

}

void AItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBox::ItemHit(AMyRaceGamePawn* HitPawn)
{
	//�A�C�e����ID�𗐐�����t�^����
	const int32 randMin = 1;
	const int32 randMax = 3;
	SelectItemNumber = FMath::RandRange(randMin, randMax);

	UItemSettingComponent* ItemSetting = HitPawn->FindComponentByClass<UItemSettingComponent>();
	if (ItemSetting != nullptr && HitPawn->IsLocallyControlled())
	{
		ItemSetting->ItemPickup(SelectItemNumber);
	}

}

void AItemBox::ItemHit(AMyPawn* HitPawn)
{
	//�A�C�e����ID�𗐐�����t�^����
	const int32 randMin = 1;
	const int32 randMax = 3;
	SelectItemNumber = FMath::RandRange(randMin, randMax);

	UItemSettingComponent* ItemSetting = HitPawn->FindComponentByClass<UItemSettingComponent>();
	if (ItemSetting != nullptr && HitPawn->IsLocallyControlled())
	{
		ItemSetting->ItemPickup(SelectItemNumber);
	}

}
