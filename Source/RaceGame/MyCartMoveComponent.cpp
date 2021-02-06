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

		//GetOwner()->get
		//�������쐬
		LastMove = CreateMove(DeltaTime);

		//�V�~�����[�V�����ړ����s��
		SimulateMove(LastMove);
		
	}
}

//�ړ��쐬
FMyPawnMove UMyCartMoveComponent::CreateMove(float DeltaTime)
{
	
	FMyPawnMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle * (SpeedUpRate * RoadSpeedRate);
	//UE_LOG(LogTemp, Error, TEXT("Rate = %f"), Move,Throttle);
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;

}


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

//�ʒu���ړ������郁�\�b�h
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


//��C��R�͌v�Z���\�b�h
FVector UMyCartMoveComponent::GetAirResistance()
{
	//��C��R�́@= -���K���������x�̃x�N�g���@* ���x�x�N�g���̂Q�� * ��C��R�W��
	return  -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

//�]�����R�͎擾���\�b�h
FVector UMyCartMoveComponent::GetRollingResistance()
{
	//�]�����R�͌v�Z

	//�d��
	//UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), GetWorld()->GetGravityZ());
	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * accelerationDueToGravity;
	//�]�����R = ���K���������x�̃x�N�g���@* �]�����R�W�� * �@���x�N�g��
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UMyCartMoveComponent::SpeedCalc(float Rate)
{

	//�X���b�g���ɗ^�����Z�l��ύX
	SpeedUpRate = Rate;

}

//���x�����̃^�C���C�x���g�֐�
void UMyCartMoveComponent::SpeedCalcTimeEvent(float Rate , float CallTime)
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
	//TODO:�}�W�b�NNO�C��
	SpeedCalcTimeEvent(10.0f,  0.01f);
	SpeedCalcTimeEvent(1.0f,  1.0f);
}

void UMyCartMoveComponent::CrashEvent()
{
	//���x�����Z�b�g����B
	Velocity = FVector::ZeroVector;
	SpeedCalcTimeEvent(0.0f,  0.01f);
	SpeedCalcTimeEvent(1.0f, 1.0f);
}




