// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGamePawn.h"

#include "Engine/DataTable.h"

#include "MyRaceGamePawn.generated.h"

/**
 * 
 */


 //�N���X�̑O���錾
class UInputComponent;
class UTexture2D;


UCLASS()
class RACEGAME_API AMyRaceGamePawn : public ARaceGamePawn
{
	GENERATED_BODY()

protected:


	//==========================�R���|�[�l���g

	//�I�t���[�h�`�F�b�N�p�̃R���C�_�[
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* WheelColliderGroup;

	//�I�t���[�h�`�F�b�N�p�̃R���C�_�[
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ForwardCollision;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* BackwardCollision;

	//�A�C�e���Ǘ����s���R���|�[�l���g
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UItemSettingComponent* ItemSettingComponent;

	//���[�X��Ԃ��Ǘ�����R���|�[�l���g
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class URaceSettingComponent* RaceSettingComponent;

	// �A�N�Z���l���猸�Z����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		float DownSpeedOffset;


protected:
	
	
	//�u���[�L�����̃^�C���n���h���[
	FTimerHandle BrakeTimeHandler;


protected:
	//===========================���\�b�h
	//�R���X�g���N�^
	AMyRaceGamePawn();

	virtual void BeginPlay() override;

	// Begin Actor interface
	virtual void Tick(float Delta) override;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	
	//���͒l�̃��Z�b�g
	UFUNCTION()
		void InputReset();




	//-------------�����蔻��
	//BeginOverlap
	UFUNCTION()
		void ForwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void ForwardCollisionEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UFUNCTION()
		void BackwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void BackwardCollisionEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

public:
	//�N���b�V���C�x���g
	UFUNCTION()
		void CrashEvent();

	//�X�s�[�h�𒲐�����C�x���g
	UFUNCTION()
		void SpeedCalcFunction(float SpeedMultipication);

	//�A�N�Z���Ɋ|�����Z�l��ݒ肷�郁�\�b�h
	UFUNCTION()
		void AccelSetting(UPrimitiveComponent* HitComponent , bool bIsSpeedDown);



	//���X�|�[�������i�]�|���̋~�ρj
	UFUNCTION()
		void Respawn();

	UFUNCTION(Server , Reliable)
		void RespawnRunonServer();

	UFUNCTION()
		void ItemUse();


	
	
};
