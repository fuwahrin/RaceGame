// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ItemSettingComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

	
	//�A�C�e���ݒ�̃R���|�[�l���g
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(MeshOffSetRoot);
	

	//�ړ��R���|�[�l���g�̐ݒ�
	Movement = CreateDefaultSubobject<UMyCartMoveComponent>(TEXT("Movement"));

	//�ړ������R���|�[�l���g�ݒ�
	MovementReplicator = CreateDefaultSubobject<UMyCartMoveComponentReplicator>(TEXT("MovementComponentReplicator"));

	


}
// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//�����̎擾
	Role = GetLocalRole();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1.0f;
	}
	
}


// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//�v���C���[�̌������f�o�b�O�\��
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);

}





// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�L�[�A�N�V�����o�^
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);

	//�A�C�e���o���{�^��
	//�l�b�g���[�N�̌������m�F
	if (this->HasAuthority())
	{
		//�T�[�o�[���Ȃ�}���`�L���X�g
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUseMultiCast);

	}
	else
	{
		//�N���C�A���g���Ȃ�T�[�o�Ŏ��s������B
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUseRunonServer);
	}

}


void AMyPawn::MoveForward(float value)
{
	if (Movement == nullptr) return;

	Movement->SetThrottle(value);
}

void AMyPawn::MoveRight(float value)
{
	if (Movement == nullptr) return;

	Movement->SetSteeringThrow(value);
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

void AMyPawn::ItemUseMultiCast()
{
	//UE_LOG(LogTemp, Log, TEXT("ItemUseMultiCast(1)"));
	if (ItemSettingComponent != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("ItemUseMultiCast(2)"));
		ItemSettingComponent->SpawnItemMulticast();
	}
}

void AMyPawn::ItemUseRunonServer()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemRunonServer();
	}
}

