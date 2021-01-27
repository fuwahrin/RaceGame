// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "MyRaceGamePawn.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//�R���|�[�l���g�̏����ݒ�
	ItemColision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollision"));
	ItemColision->SetupAttachment(RootComponent);
	ItemColision->InitSphereRadius(50.0f);
	ItemColision->SetEnableGravity(false);
	ItemColision->bApplyImpulseOnDamage = false;
	RootComponent = ItemColision;

	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshSet(TEXT("StaticMesh'/Engine/EditorMeshes/EditorHelp.EditorHelp'"));
	UStaticMesh* ItemMeshAsset = MeshSet.Object;
	ItemMesh->SetStaticMesh(ItemMeshAsset);
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));
	RotationMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	//�I�[�o�[���b�v�C�x���g��o�^
	ItemColision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::BeginOverlap);

	//�ϐ��̏����ݒ�
	bIsItemRespawn = true;

	//SelectItemNumber = 0;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}
//BeginOverlap
void AItemBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//�q�b�g�����A�N�^�[���v���C���[�Ȃ�L���X�g���s��
	AMyRaceGamePawn* const MyRaceGamePawn = Cast<AMyRaceGamePawn>(OtherActor);

	//��L�̕ϐ��ݒ肪���܂��s���Ă���Ώ���
	if (MyRaceGamePawn)
	{
		//�A�C�e�����\��
		ItemVisibility(false);

		//�A�C�e���ɓ��������C�x���g
		ItemHit(MyRaceGamePawn);

		//��莞�Ԃ�������A�C�e�����ĕ\��
		const float DelayTime = 3.0f;
		GetWorldTimerManager().SetTimer(RespawnHandler, this, &AItemBase::ItemRespawn, DelayTime, false);

	}
}

void AItemBase::ItemVisibility(bool bIsVisible)
{
	//���X�|�[���\�̂��̂�����
	if (bIsItemRespawn)
	{
		//�����ɂ���ăA�C�e���̕\���E��\����ݒ�
		if (bIsVisible)
		{
			//�A�C�e����\��
			ItemMesh->SetVisibility(true, true);

			//�R���W������L���ɂ���B
			ItemColision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		}
		else
		{
			//�A�C�e���̔�\��
			ItemMesh->SetVisibility(false, true);
			//�R���W�����𖳌��ɂ���B
			ItemColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		//�A�C�e���̔�\��
		ItemMesh->SetVisibility(false, true);
		//�R���W�����𖳌��ɂ���B
		ItemColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AItemBase::ItemRespawn()
{
	ItemVisibility(true);
}

void AItemBase::ItemHit(AMyRaceGamePawn *HitPawn)
{

	UE_LOG(LogTemp, Error, TEXT("ItemHitBase"));
}

