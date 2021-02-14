// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCartMoveComponentReplicator.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UMyCartMoveComponentReplicator::UMyCartMoveComponentReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//���̃R���|�[�l���g�̓�����L���ɂ���
	SetIsReplicatedByDefault(true);
	
	
}


// Called when the game starts
void UMyCartMoveComponentReplicator::BeginPlay()
{
	Super::BeginPlay();

	//�ړ��R���|�[�l���g�����̃R���|�[�l���g�̏��L�҂���T��
	MovementComponent = GetOwner()->FindComponentByClass<UMyCartMoveComponent>();
}


// Called every frame
void UMyCartMoveComponentReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//�ړ��R���|�[�l���g���Z�b�g����Ă��Ȃ���Ώ������s��Ȃ�
	if (MovementComponent == nullptr) return;

	FMyPawnMove LastMove = MovementComponent->GetLastMove();

	//�����ɂ���ĊePawn�̓������쐬

	//�N���C�A���g���ő��삵�Ă���Pawn�̓������T�[�o�[�ɕ���������
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		if (MovementComponent->bIsMoveStart)
		{

			//�T�[�o�[�����m�F�̓�����o�^
			UnacknowledgedMoves.Add(MovementComponent->LastMove);

			//���m�F�̓����̐����m�F
			//UE_LOG(LogTemp, Warning, TEXT("Queue length: %d"), UnacknowledgedMoves.Num());
			//�T�[�o�Ɉړ����𑗐M
			Server_SendMove(LastMove);
			//UE_LOG(LogTemp, Warning, TEXT("Thottole = %f"), LastMove.Throttle);
		}
	
	}

	//�T�[�o�[���Ŏ��g�����삷��Pawn�̓������N���C�A���g���ɕ���������
	//��RemoteRole�̓}�b�v�J�ڂ���Ɛ��������L�҂��擾�ł��Ȃ��׎g�p����
	//�Q�l�Fhttps://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/Actors/Roles/index.html
	if(GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
	{
		//UE_LOG(LogTemp, Error, TEXT("Server"));
		//�T�[�o�[�̈ړ������X�V����
		if (MovementComponent->bIsMoveStart)
		{

			UpdateServerState(LastMove);
		}
	}
	
	//�N���C�A���g���ŕ�������Ă���Pawn�Ȃ�
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		//UE_LOG(LogTemp, Error, TEXT("Cliant"));
	
		CliantTick(DeltaTime);
	}

}
void UMyCartMoveComponentReplicator::Server_SendMove_Implementation(FMyPawnMove move)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SimulateMove(move);

	UpdateServerState(move);


}

void UMyCartMoveComponentReplicator::UpdateServerState(const FMyPawnMove& move)
{
	//�����p�̕ϐ��Ɍ��݂̈ړ���x�N�g�������i�[����B
	ServerState.LastMove = move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();

}

void UMyCartMoveComponentReplicator::CliantTick(float DeltaTime)
{
	//�N���C�A���g���Ŗ��t���[���s���鏈��
	CliantTimeSinceUpdate += DeltaTime;

	//�O��̍X�V���������������l�Ȃ珈�����s��Ȃ�
	if (CliantTimeBetweenLastUpdate < KINDA_SMALL_NUMBER) return;

	//�ړ��R���|�[�l���g���ݒ肳��Ă��Ȃ���Ώ������s��Ȃ��B
	if (MovementComponent == nullptr) return;

	float LerpAlpha = CliantTimeSinceUpdate / CliantTimeBetweenLastUpdate;

	FHermiteCubicSpline Spline = CreateSpline();
	
	//�L���[�r�b�N��Ԉʒu���Z�o
	InterpoteLocation(Spline, LerpAlpha);

	//�L�[�r�b�N��Ԃ̑��x���Z�o
	InterpolateVelocity(Spline, LerpAlpha);

	//�L���[�r�b�N��Ԃ̊p�x���Z�o
	InterpolateRotation(LerpAlpha);
}

void UMyCartMoveComponentReplicator::InterpolateRotation(float LerpAlpha)
{
	//�p�x�v�Z
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	//�J�n�p�x���擾
	FQuat StartRotation = CliantStartTransform.GetRotation();

	//���`�ړ��x�N�g�����Z�o
	FQuat  NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpAlpha);
	
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldRotation(NewRotation);
	}
}

float UMyCartMoveComponentReplicator::VelocityToDerivarite()
{
	return  CliantTimeBetweenLastUpdate * 100;
}

//�L���[�r�b�N��Ԉʒu���Z�o
void UMyCartMoveComponentReplicator::InterpoteLocation(const FHermiteCubicSpline& Spline, float LerpAlpha)
{

	FVector NewLocation = Spline.InterpolateLocation(LerpAlpha);
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldLocation(NewLocation);
	}
}

void UMyCartMoveComponentReplicator::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpAlpha)
{
	FVector NewDerivaive = Spline.InterpolateDeriative(LerpAlpha);
	FVector NewVelocity = NewDerivaive / VelocityToDerivarite();
	MovementComponent->SetVelocity(NewVelocity);
}


FHermiteCubicSpline UMyCartMoveComponentReplicator::CreateSpline()
{

	//�L���[�r�b�N��Ԃ̍\���̂�錾
	FHermiteCubicSpline Spline;

	//�ړ��ʒu���v�Z
	Spline.TargetLocation = ServerState.Transform.GetLocation();
	
	//�J�n�ʒu���擾
	Spline.StartLocation = CliantStartTransform.GetLocation();

	//�L���[�r�b�N��ԗp�̒l
	Spline.StartDerivate = CliantStartVelocity * VelocityToDerivarite();
	Spline.TargetDerivate = ServerState.Velocity * VelocityToDerivarite();

	return Spline;

}



void UMyCartMoveComponentReplicator::ClearUnacknowledgedMoves(FMyPawnMove LastMove)
{
	TArray<FMyPawnMove> newMove;

	for (const FMyPawnMove& move : UnacknowledgedMoves)
	{
		//�����œn����Ă������Ԃ����������ԂȂ�i���m�F�̈ړ��Ȃ�j
		if (move.Time > LastMove.Time)
		{
			//�V�������m�F�̓�����o�^
			newMove.Add(move);
		}
	}

	//�o�^���Ă�������㏑��
	UnacknowledgedMoves = newMove;
}



void UMyCartMoveComponentReplicator::OnRep_ServerState()
{
	//�t�^���ꂽ�����ɂ���ď�����ς���
	switch (GetOwnerRole())
	{
		case ROLE_AutonomousProxy:
			//�T�[�o�[��
			AutonomousProxy_OnRep_ServerState();
			break;

		case ROLE_SimulatedProxy:
			//�N���C�A���g��
			SimulatedProxy_OnRep_ServerState();
			break;

	}
	
}

FVector UMyCartMoveComponentReplicator::GetSimulateLocation()
{
	return MeshOffsetRoot->GetComponentLocation();
}

void UMyCartMoveComponentReplicator::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	//�X�V���Ԃ��i�[
	CliantTimeBetweenLastUpdate = CliantTimeSinceUpdate;

	//�X�V�J�E���^�[�����Z�b�g
	CliantTimeSinceUpdate = 0;

	//�ړ��J�n�ʒu���i�[
	if (MeshOffsetRoot != nullptr)
	{
		//�R���C�_�[�̈ʒu����擾
		CliantStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		CliantStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	
	//�ړ����x���i�[
	CliantStartVelocity = MovementComponent->GetVelocity();

	//���L�҂̈ʒu��ύX
	GetOwner()->SetActorTransform(ServerState.Transform);

	


}

//�������T�[�o���������Ƃ��̓�������
void UMyCartMoveComponentReplicator::AutonomousProxy_OnRep_ServerState()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReplicatedLocation"));

	if (MovementComponent == nullptr) return;

	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	//���m�F�̓����̃��X�g���X�V����
	ClearUnacknowledgedMoves(ServerState.LastMove);

	//���m�F�̓������X�g�̕��̓������s��
	for (const FMyPawnMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UMyCartMoveComponentReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyCartMoveComponentReplicator, ServerState);

}




