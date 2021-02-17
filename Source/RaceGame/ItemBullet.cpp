// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyCartMoveComponent.h"



AItemBullet::AItemBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����ݒ�
	SetReplicates(true);

	//���b�V��
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BulletMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	UStaticMesh* BulletMesh = BulletMeshAsset.Object;
	ItemMesh->SetStaticMesh(BulletMesh);		
	
	//�����ݒ�
	ItemColision->SetEnableGravity(true);
	ItemColision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	//�R���|�[�l���g�̐ݒ�
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->InitialSpeed = 3000.0f;
	BulletMovement->MaxSpeed = 10000.0f;
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 1.0f;
	BulletMovement->Friction = 0.0f;
	BulletMovement->BounceVelocityStopSimulatingThreshold = 0.0f;

	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0 ,0.0f));

	//�ϐ��̐ݒ�
	bIsItemRespawn = false;

}

void AItemBullet::BeginPlay()
{
	Super::BeginPlay();

	//�������悤�ɃR���W������ݒ�
	ItemColision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

}

//�A�C�e���������������̃C�x���g
void AItemBullet::ItemHit(AMyRaceGamePawn *HitPawn)
{
	//�v���C���[���N���b�V��������
	HitPawn->CrashEvent();
}

//�A�C�e���������������̃C�x���g
void AItemBullet::ItemHit(AMyPawn* HitPawn)
{
	//�v���C���[���N���b�V��������
	HitPawn->FindComponentByClass<UMyCartMoveComponent>()->CrashEvent();
}
