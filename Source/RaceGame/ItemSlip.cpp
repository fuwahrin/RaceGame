// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlip.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"

//�R���X�g���N�^
AItemSlip::AItemSlip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//���b�V���̐ݒ�
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SlipItemMeshAsset(TEXT("StaticMesh'/Game/VehicleAdv/ArenaMesh/pole.pole'"));
	UStaticMesh* SlipItemMesh = SlipItemMeshAsset.Object;
	ItemMesh->SetStaticMesh(SlipItemMesh);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0, 25.0f));
	

	//�����ݒ�
	SetReplicates(true);

	//���X�|�[���h�~
	bIsItemRespawn = false;

}
void AItemSlip::BeginPlay()
{
	Super::BeginPlay();
}
//�A�C�e���Ƀq�b�g�����Ƃ��̃��\�b�h
void AItemSlip::ItemHit(AMyRaceGamePawn* HitPawn)
{
	//�v���C���[���N���b�V��������
	HitPawn->CrashEvent();

}

//�A�C�e���Ƀq�b�g�����Ƃ��̃��\�b�h
void AItemSlip::ItemHit(AMyPawn* HitPawn)
{
	//�v���C���[���N���b�V��������
	HitPawn->FindComponentByClass<UMyCartMoveComponent>()->CrashEvent();

}
