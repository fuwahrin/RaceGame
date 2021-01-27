// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.generated.h"



//�����p�̈ړ��̍\����
USTRUCT()
struct FMyPawnState
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FMyPawnMove LastMove;
};

//�L���[�r�b�N��Ԍv�Z�p�̍\����
struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivate, TargetLocation, TargetDerivate;

	//��Ԉʒu���擾���郁�\�b�h
	FVector InterpolateLocation(float LerpAlpha) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivate, TargetLocation, TargetDerivate, LerpAlpha);
	}
	//�h����Ԉʒu���擾���郁�\�b�h
	FVector InterpolateDeriative(float LerpAlpha) const 
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivate, TargetLocation, TargetDerivate, LerpAlpha);
	}

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UMyCartMoveComponentReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCartMoveComponentReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//�R���|�[�l���g
	//�ړ��̃R���|�[�l���g
	UPROPERTY()
	UMyCartMoveComponent* MovementComponent;

	//�R���C�_�[�I�t�Z�b�g
	UPROPERTY()
	USceneComponent* MeshOffsetRoot;

private:


	//�����p�̈ʒu
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FMyPawnState ServerState;

	//�ړ��������\�b�h
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FMyPawnMove move);

	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();

	//�T�[�o�[�̈ړ���Ԃ��X�V����
	void UpdateServerState(const FMyPawnMove& move);

	//���m�F�̈ړ����X�g
	TArray<FMyPawnMove> UnacknowledgedMoves;

	void CliantTick(float DeltaTime);

	
	//�L���[�r�b�N��Ԃ̃X�v���C�����쐬���郁�\�b�h
	FHermiteCubicSpline CreateSpline();

	float VelocityToDerivarite();

	//�L���[�r�b�N��Ԉʒu���Z�o����
	void InterpoteLocation(const FHermiteCubicSpline& Spline, float LerpAlpha);

	//�L���[�r�b�N��ԑ��x���Z�o����B
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpAlpha);

	//�L���[�r�b�N��Ԃ̊p�x���Z�o����B
	void InterpolateRotation(float LerpAlpha);

	//���m�F�̈ړ����폜���郁�\�b�h
	void ClearUnacknowledgedMoves(FMyPawnMove LastMove);

	//�R���C�_�[�I�t�Z�b�g�̐e�R���|�[�l���g��ݒ肷��B
	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }



private:
	//�ړ��֘A
	//�ϐ�

	//�N���C�A���g���̈ړ��X�V��̎���
	float CliantTimeSinceUpdate;

	//�ŏI�X�V�܂ł̎���
	float CliantTimeBetweenLastUpdate;

	//�N���C�A���g���̌v�Z�J�n�ʒu
	FTransform CliantStartTransform;

	//�N���C�A���g���̌v�Z�J�n���̑��x
	FVector CliantStartVelocity;

	
	

	

		
};