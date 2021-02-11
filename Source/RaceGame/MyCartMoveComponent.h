// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCartMoveComponent.generated.h"



USTRUCT()
struct FMyPawnMove
{
	GENERATED_USTRUCT_BODY()

		//�X���b�g��
		UPROPERTY()
		float Throttle;

	//�X�e�A�����O
	UPROPERTY()
		float SteeringThrow;

	//����
	UPROPERTY()
		float DeltaTime;

	//�ړ����J�n��������
	UPROPERTY()
		float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UMyCartMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCartMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//
	
	//�X���b�g��
	float Throttle;

	//�X�e�A�����O
	float SteeringThrow;
	
	
	//�X���b�g�����ő�̎��ɎԂɉ��������(N)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 5000;

	//�X���b�g�����ő�̎��ɎԂɉ��������(N)�̏����l
	float InitMaxDrivingForce;
	//�t���X���b�g����1�b������ɉ�]����ŏ���]���a(m)
	UPROPERTY(EditAnywhere)
	float MinTurnningRadius = 10.0f;

	//��C��R�̌W��(�l��傫������ƒ�R���傫���Ȃ�)
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//���C�͂̌W��(�l��傫������ƒ�R���傫���Ȃ�)
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015f;

	//�Ԃ̎���(kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	
	
public:
	//�A�C�e���̉����n
	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (ClampMin = "0.0" , ClampMax = "1.0"))
	float SpeedUpRate =1.0f;

	//���H�̑��x�����p�̒l
	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float RoadSpeedRate = 1.0f;

	//�J�[�u���̌���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float SteeringRate = 1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void SpeedUpEvent();

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void CrashEvent();

private:
	//���x����
	UFUNCTION()
		void SpeedCalc(float Rate);

	//���x�_�E��
	UFUNCTION()
		void SpeedCalcTimeEvent(float Rate , float CallTime);


public:
	//�ړ��v�Z
	void UpdateLocationFromVelocity(float DeltaTime);

	//�����v�Z
	void ApplyRotation(float DeltaTime, float Steering);

	//��C��R�͎擾
	FVector GetAirResistance();

	//���C�͎擾
	FVector GetRollingResistance();

	//�V�~�����[�V�����ړ�
	void SimulateMove(const FMyPawnMove& move);

	//�ړ��쐬�֐�
	FMyPawnMove CreateMove(float DeltaTime);


	//�Z�b�^�[�Q�b�^�[

	//�x�N�g��
	void SetVelocity(FVector value) { Velocity = value; }
	FVector GetVelocity() { return Velocity; }

	//�X���b�g��
	void SetThrottle(float value) { Throttle = value; }

	//�X�e�A�����O
	void SetSteeringThrow(float value) { SteeringThrow = value; }

	//�Ō�̈ړ����
	FMyPawnMove LastMove;

	//�ŏI�ړ��̃Q�b�^�[
	FMyPawnMove GetLastMove() { return LastMove; }
	
};
