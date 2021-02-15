// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRaceGamePawn.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "ItemSettingComponent.h"
#include "RaceSettingComponent.h"


// �R���X�g���N�^
AMyRaceGamePawn::AMyRaceGamePawn()
{
	USkeletalMeshComponent* OwnerMesh = GetMesh();
	OwnerMesh->SetCollisionProfileName(TEXT("MyRacePawn"));

	

	//�I�t���[�h�m�F�p�̃^�C���R���W�����R���|�[�l���g�̐ݒ�
	WheelColliderGroup = CreateDefaultSubobject<USceneComponent>(TEXT("WheelColiderGroup"));
	WheelColliderGroup->SetupAttachment(OwnerMesh);

	//�A�C�e���Ǘ��̃R���|�[�l���g�ݒ�
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(GetMesh());

	float radius = 15.0f;
	ForwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ForwardColision"));
	ForwardCollision->SetupAttachment(WheelColliderGroup);
	ForwardCollision->SetRelativeLocation(FVector(-50.0f, 0.0f, 10.0f));
	ForwardCollision->InitSphereRadius(radius);
	ForwardCollision->SetCollisionProfileName(TEXT("Wheel"));

	BackwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BackwardColision"));
	BackwardCollision->SetupAttachment(WheelColliderGroup);
	BackwardCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 10.0f));
	BackwardCollision->InitSphereRadius(radius);
	BackwardCollision->SetCollisionProfileName(TEXT("Wheel"));

	
	//���[�X��Ԃ��Ǘ�����R���|�[�l���g
	RaceSettingComponent = CreateDefaultSubobject<URaceSettingComponent>(TEXT("RaceSettingComponent"));
	
}

// BeginPlay
void AMyRaceGamePawn::BeginPlay()
{
	Super::BeginPlay();

	//�R���C�_�[�C�x���g�̓o�^
	ForwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionBeginOverlap);
	ForwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionEndOverlap);
	BackwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionBeginOverlap);
	BackwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionEndOverlap);

}

// Tick
void AMyRaceGamePawn::Tick(float Delta)
{
	Super::Tick(Delta);
}

// �C���v�b�g�o�^
void AMyRaceGamePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�A�C�e���o���{�^��
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUse);

	//���X�|�[��
	PlayerInputComponent->BindAction("PlayerRespawn", IE_Pressed, this, &AMyRaceGamePawn::Respawn);
	
}

//���͒l�̃��Z�b�g
void AMyRaceGamePawn::InputReset()
{
	InputCalc = 1.0f;
	
	//�A�C�e�����g�p�\��Ԃɂ���B
	ItemSettingComponent->SetItemUse(true);
}


//�@�^�C���̃R���C�_�[����i�O�ցj��BeginOverlap
void AMyRaceGamePawn::ForwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//�@�^�C���̃R���C�_�[����(�O�ցj��Endlap
void AMyRaceGamePawn::ForwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//�@�^�C���̃R���C�_�[����i��ցj��BeginOverlap
void AMyRaceGamePawn::BackwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//�@�^�C���̃R���C�_�[����(��ցj��Endlap
void AMyRaceGamePawn::BackwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//�N���b�V���C�x���g
void AMyRaceGamePawn::CrashEvent()
{

	const float SpeedMultipication = 0.0f;

	//0����Z���邱�Ƃɂ���ăX�s�[�h���Ȃ����B
	SpeedCalcFunction(SpeedMultipication);

	//���͒l�Ɋ|���Ă���l��0�ɂ���B�i���͂��Ă��i�܂��Ȃ��ׁj
	InputCalc = 0.0f;

	//�A�C�e�����g�p�ł��Ȃ��悤�ɂ���A
	ItemSettingComponent->SetItemUse(true);


	//��莞�Ԋ�������͒l�Ɋ|���Ă���l�����Ƃɖ߂�
	float DelayTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(BrakeTimeHandler, this, &AMyRaceGamePawn::InputReset, DelayTime, false);


}

//	�X�s�[�h�𒲐�����C�x���g
void AMyRaceGamePawn::SpeedCalcFunction(float SpeedMultipication)
{
	//���ʂ̃x�N�g���Ɉ����Ŏw�肵���l����Z����
	FVector NewVelocity = GetMesh()->GetForwardVector() * SpeedMultipication;
	GetMesh()->SetPhysicsLinearVelocity(NewVelocity);

}

//��������
void AMyRaceGamePawn::AccelSetting(UPrimitiveComponent* HitComponent, bool bIsSpeedDown)
{
	//UE_LOG(LogTemp, Log, TEXT("AccleSettingOverlap"));

	//�u���[�L�l�ɉ��Z���Z������l
	const float BrakeValue = 0.25f;

	//�ő�̃C���v�b�g�l
	const float InputCalcMax = 1.0f;


	//�������邩�ǂ����H
	if (bIsSpeedDown)
	{
		//�������s���ꍇ�͌����l�Ƃ��ău���[�L�l�������l�ɉ�����
		DownSpeedOffset += BrakeValue;
	}
	else
	{
		//��������߂�ꍇ�̓u���[�L�l�������l�������
		DownSpeedOffset += (BrakeValue * -1);
	}

	//�C���v�b�g�l�Ƀu���[�L�l�̌v�Z��������
	InputCalc = InputCalcMax - DownSpeedOffset;

}

//	�v���C���[���X�|�[���i�~�ϑ[�u�j
void AMyRaceGamePawn::Respawn()
{
	//�T�[�o�[���ŌĂ΂�Ă���Ȃ珈�����s��
	if (this->HasAuthority())
	{
		//���X�|�[���ʒu�̒����l
		FVector VectorOffset = FVector(0.0f, 0.0f, 20.0f);
		//���X�|�[���ʒu�̐ݒ�
		FVector RespawnLocation = GetMesh()->GetComponentLocation() + VectorOffset;
		FTransform RespawnTransform = FTransform(RespawnLocation);
		GetMesh()->SetWorldTransform(RespawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		//�N���C�A���g���Ȃ�T�[�o�Ŏ��s����悤�ɏ�������
		RespawnRunonServer();
	}
	
}

void AMyRaceGamePawn::RespawnRunonServer_Implementation()
{
	Respawn();
}


void AMyRaceGamePawn::ItemUse()
{
	ItemSettingComponent->SpawnItem();
}



