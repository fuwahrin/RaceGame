// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(false);

	//�ړ��R���|�[�l���g�̐ݒ�
	MovementComponent = CreateDefaultSubobject<UMyCartMoveComponent>(TEXT("MovementComponent"));

	//�ړ������R���|�[�l���g�ݒ�
	MovementComponentReplicator = CreateDefaultSubobject<UMyCartMoveComponentReplicator>(TEXT("MovementComponentReplicator"));

}
// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	Role = GetLocalRole();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
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
}


void AMyPawn::MoveForward(float value)
{
	if (MovementComponent == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Spped = %f"), value);
	MovementComponent->SetThrottle(value);
}

void AMyPawn::MoveRight(float value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(value);

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
