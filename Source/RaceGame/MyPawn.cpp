// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ItemSettingComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ItemBase.h"
#include "RayActor.h"
#include "RaceSettingComponent.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(false);


	//�R���|�[�l���g�̐ݒ�
	
	//�R���C�_�[
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	//���b�V���̃I�t�Z�b�g
	MeshOffSetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshOffset"));
	MeshOffSetRoot->SetupAttachment(BoxCollision);

	//���b�V��
	CarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeltalMesh"));
	CarMesh->SetupAttachment(MeshOffSetRoot);

	//SplingArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MeshOffSetRoot);

	//�J����
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	//���C�g���[�X�̋N�_
	ForwardRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayForward"));
	ForwardRayPoint->SetupAttachment(MeshOffSetRoot);
	ForwardRayPoint->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));

	BackwardRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayBackward"));
	BackwardRayPoint->SetupAttachment(MeshOffSetRoot);
	BackwardRayPoint->SetRelativeLocation(FVector(-70.0f, 0.0f, 0.0f));

	LeftRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayLeft"));
	LeftRayPoint->SetupAttachment(MeshOffSetRoot);
	LeftRayPoint->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));

	RightRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayRight"));
	RightRayPoint->SetupAttachment(MeshOffSetRoot);
	RightRayPoint->SetRelativeLocation(FVector(0.0f, 30.0f, 0.0f));

	// Setup the audio component and allocate it a sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_CueMyPawn.Engine_Loop_CueMyPawn"));
	
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(CarMesh);

	///---ActorComponent�̐ݒ�
	//�A�C�e���ݒ�̃R���|�[�l���g
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(MeshOffSetRoot);

	//�ړ��R���|�[�l���g�̐ݒ�
	Movement = CreateDefaultSubobject<UMyCartMoveComponent>(TEXT("Movement"));
	//�ړ������R���|�[�l���g�ݒ�
	MovementReplicator = CreateDefaultSubobject<UMyCartMoveComponentReplicator>(TEXT("MovementComponentReplicator"));

	
	//���[�X��Ԃ��Ǘ�����R���|�[�l���g
	RaceSettingComponent = CreateDefaultSubobject<URaceSettingComponent>(TEXT("RaceSettingComponent"));
}
// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//�����̎擾
	Role = GetLocalRole();

	//�������s���b���̐ݒ�i�P�b�j
	if (HasAuthority())
	{
		NetUpdateFrequency = 1.0f;
	}

	//���C�g���[�X�A�N�^�[���X�|�[������B
	RayForward = SpawnRayActor(ForwardRayPoint);
	RayBackward = SpawnRayActor(BackwardRayPoint);
	RayLeft = SpawnRayActor(LeftRayPoint);
	RayRight = SpawnRayActor(RightRayPoint);

}


// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�v���C���[�̌������f�o�b�O�\��
	//DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);

	//���C�g���[�X
	float forwardState =  RoadSpeedCalcFunction(RayForward);
	float backwardState = RoadSpeedCalcFunction(RayBackward);
	float leftState = RoadSpeedCalcFunction(RayLeft);
	float rightState = RoadSpeedCalcFunction(RayRight);

	//���H��Ԃ̑��x��ݒ�
	Movement->RoadSpeedRate = forwardState * backwardState * leftState * rightState;
	//UE_LOG(LogTemp, Warning, TEXT("SpeedUpRate = %f"), Movement->SpeedUpRate);

	float AudioScale = FVector::DotProduct(GetActorForwardVector(), Movement->Velocity);
	AudioScale = FMath::Clamp<float>(AudioScale ,0.0f , 2500.0f);
	EngineSoundComponent->SetFloatParameter("RPM", AudioScale);
	//UE_LOG(LogTemp, Warning, TEXT("AudioScale = %f"), AudioScale);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�L�[�A�N�V�����o�^
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUse);
}

//�O�i�E���
void AMyPawn::MoveForward(float value)
{
	if (Movement == nullptr) return;

	Movement->SetThrottle(value);
}

//���E
void AMyPawn::MoveRight(float value)
{
	if (Movement == nullptr) return;

	Movement->SetSteeringThrow(value);
}


//�A�C�e�����d�l������Ԃɂ��郁�\�b�h
void AMyPawn::ItemUse()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItem();
	}
}

//���H�̑��x�l���擾���郁�\�b�h
float AMyPawn::RoadSpeedCalcFunction(ARayActor *rayActor)
{
	//�X�s�[�h�̌v�Z�p�ϐ�
	float SpeedRate = 1.0f;

	//���H�̃��C������擾
	if (rayActor->IsStateChange())
	{
		//���H�𑖂��Ă���Ȃ�
		if (rayActor->GetRayHitState() == FName("Road"))
		{
			//�ő呬�x�������Ă��Ȃ����
			if (SpeedRate < 1.0f)
			{
				//�X�s�[�h���グ��
				SpeedRate += 0.3f;
			}
		}
		else
		{
			//�ŏ����x��������Ă����
			if (SpeedRate < 0.1f)
			{
				//���x���ŏ���
				SpeedRate = 0.1f;
			}
			else
			{
				//������Ă��Ȃ���Α��x��������
				SpeedRate -= 0.3f;
			}
		}
	}

	return SpeedRate;
}


//���C�g���[�X�̃A�N�^�[���o��������B
ARayActor* AMyPawn::SpawnRayActor(USceneComponent * parent)
{
	ARayActor* SpawnActor;
	//���C�g�[���X�p�̃A�N�^�[��Spawn����B
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	//�X�|�[���ʒu�̐ݒ�(ItemSpawnPoint�̃��[���h���W���i�[)
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	//�X�|�[������ۂ�Scale
	FVector SpawnScale = FVector::OneVector;
	SpawnActor = GetWorld()->SpawnActor<ARayActor>(RayActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnActor->AttachToComponent(parent , { EAttachmentRule::KeepRelative , true });

	return SpawnActor;
}


FString AMyPawn::GetEnumText(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "ROLE_MAX";
	default:
		return "ERROR";
	}
}

