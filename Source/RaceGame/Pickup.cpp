// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"


// Sets default values
APickup::APickup()
{
	//�@Tick�֐����ŏ������s��Ȃ�
 	PrimaryActorTick.bCanEverTick = false;

	//�I�u�W�F�N�g���������ꂽ���_�ŃA�N�e�B�u�ɂ��Ă���
	bIsActive = true;

	//�I�u�W�F�N�g�̃��b�V����ݒ肷��B
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//staticMesh�̃R���|�[�l���g�����[�g�ɐݒ�
	RootComponent = PickupMesh;

}

//bIsActive�̃Q�b�^�[
bool APickup::IsActive()
{
	return bIsActive;
}

//bIsActive�̃Z�b�^�[
void APickup::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}
void APickup::WasCollected_Implementation()
{
	FString name = GetName();
	UE_LOG(LogTemp, Log, TEXT("You havr collected %s "), *name);

}
