// Fill ot your copyright notice in the Description page of Project Settings.


#include "MyCartMoveComponent.h"
#include "GameFramework/GameStateBase.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UMyCartMoveComponent::UMyCartMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//�ϐ��̐ݒ�
	InitMaxDrivingForce = MaxDrivingForce;
}


// Called when the game starts
void UMyCartMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyCartMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//�N���C�A���g�������g�����삷��Pawn�Ȃ�
	//�Q�l�Fhttps://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/Actors/Roles/index.html
	//UE_LOG(LogTemp , Error , GetOwner()->GetInstigator<APawn>()->IsLocallyControlled());
	if (GetOwnerRole() == ROLE_AutonomousProxy || //�N���C�A���g���ő��삵�Ă��邩 
		GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())//�T�[�o���ő��삵�Ă��邩���i�jRemoteRole�̓}�b�v�J�ڂ���Ɛ��������L�҂��擾�ł��Ȃ��׎g�p����
	{

		//�������쐬
		LastMove = CreateMove(DeltaTime);

		//�V�~�����[�V�����ړ����s��
		SimulateMove(LastMove);
		
	}
}

//�X�s�[�h�A�b�v�l�̒���
void UMyCartMoveComponent::SpeedCalc(float Rate)
{
	//�X���b�g���ɗ^�����Z�l��ύX
	SpeedUpRate = Rate;

}

//���x�����̃^�C���C�x���g�֐�
void UMyCartMoveComponent::SpeedCalcTimeEvent(float Rate, float CallTime)
{
	//�C�x���g�p�̃n���h���[�ƃf���Q�[�g�ϐ���錾
	FTimerHandle SpeedHandler;
	FTimerDelegate SpeedDelegate;

	//���x�����̊֐���o�^
	SpeedDelegate.BindUFunction(this, FName(TEXT("SpeedCalc")), Rate);

	//UE_LOG(LogTemp, Warning, TEXT("RateInput = %f"), Rate);

	//�C�x���g�o�^�����֐����Ă�
	GetWorld()->GetTimerManager().SetTimer(SpeedHandler, SpeedDelegate, CallTime, false);
}

void UMyCartMoveComponent::SpeedUpEvent()
{
	float MaxSpeedRate = 10.0f;
	float CallTime = 0.1f;
	float Default = 1.0f;

	//TODO:�}�W�b�NNO�C��
	SpeedCalcTimeEvent(MaxSpeedRate, CallTime);
	SpeedCalcTimeEvent(Default, Default);
}


//�N���b�V���C�x���g
void UMyCartMoveComponent::CrashEvent()
{
	float CrashRate = 0.0f;
	float CallTime = 0.1f;
	float Default = 1.0f;

	//���x�����Z�b�g����B
	Velocity = FVector::ZeroVector;
	SpeedCalcTimeEvent(CrashRate, CallTime);
	SpeedCalcTimeEvent(Default, Default);
}

//�ړ��v�Z
void UMyCartMoveComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	//�����蔻��p�̕ϐ�
	FHitResult Hit;

	//�ړ�����
	FVector Translation = Velocity * 100 * DeltaTime;
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);


	//�ǂȂǂɓ������Ă������̏���
	if (Hit.IsValidBlockingHit())
	{
		//���x�����Z�b�g����B
		Velocity = FVector::ZeroVector;
	}
}

//�����v�Z
void UMyCartMoveComponent::ApplyRotation(float DeltaTime, float Steering)
{
	//�����v�Z

	//�x�N�g���̒���
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;

	//���E�̊p�x�̓x��
	float RoationAngle = DeltaLocation * MinTurnningRadius * Steering;

	//�v���C���[�̌���
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), FMath::DegreesToRadians(RoationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	//�A�N�^�[�Ɍ�����K�p
	GetOwner()->AddActorWorldRotation(RotationDelta);
}


//��C��R�͎擾
FVector UMyCartMoveComponent::GetAirResistance()
{
	//��C��R�́@= -���K���������x�̃x�N�g���@* ���x�x�N�g���̂Q�� * ��C��R�W��
	return  -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}


//�]�����R�͌v�Z
FVector UMyCartMoveComponent::GetRollingResistance()
{
	
	//�d��
	//UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), GetWorld()->GetGravityZ());
	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * accelerationDueToGravity;
	//�]�����R = ���K���������x�̃x�N�g���@* �]�����R�W�� * �@���x�N�g��
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}




//�V�~�����[�V�����ړ�
void UMyCartMoveComponent::SimulateMove(const FMyPawnMove& move)
{
	//�A�N�Z����
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * move.Throttle;

	
	//��C��R
	Force += GetAirResistance();
	//���C��
	Force += GetRollingResistance();



	//������ = �A�N�Z���� / ����
	FVector Accleration = Force / Mass;

	//���x�v�Z
	Velocity = Velocity + Accleration * move.DeltaTime;

	//UE_LOG(LogTemp, Warning, TEXT("FORCE : %f"), Force.Size());
	//UE_LOG(LogTemp, Warning, TEXT("Velocity : %f"), Velocity.Size());


	ApplyRotation(move.DeltaTime, move.SteeringThrow);

	UpdateLocationFromVelocity(move.DeltaTime);

}


//�ړ��쐬
FMyPawnMove UMyCartMoveComponent::CreateMove(float DeltaTime)
{
	//�J�[�u���̌����l�ݒ�
	if (SteeringThrow != 0.0f)
	{
		SteeringRate = 0.5f;
	}
	else
	{
		SteeringRate = 1.0f;
	}

	//UE_LOG(LogTemp, Error, TEXT("SteeringRate = %f"), SteeringRate);
	FMyPawnMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = (Throttle * (SpeedUpRate * RoadSpeedRate) * SteeringRate);
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;

}



void UMyCartMoveComponent::SetThrottle(float value)
{
	
	if (!bIsMoveStart && value != 0.0f)
	{
		//�ړ��̓����t���O��OFF�Ȃ瓮���n�߂��ۂɂn�m�ɂ���
		//��Exe�t�@�C�����s���ɃN���C�A���g����Pawn��Destroy����Ă��܂��h�~
		bIsMoveStart = true;
	}
	Throttle = value;
	//UE_LOG(LogTemp, Warning, TEXT("Throttle = %f"), Throttle);
}



