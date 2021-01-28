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

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UItemSettingComponent* ItemSettingComponent;



protected:
	//==========================�ϐ�
	//UI�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> GameUiClass;

	//UI�̎������i�[����ϐ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;

	
	
	//�u���[�L�����̃^�C���n���h���[
	FTimerHandle BrakeTimeHandler;

	//�S�[���ɕK�v�ȃ��b�v��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//���݉����b�v���������J�E���g����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//�`�F�b�N�|�C���g�ɓ��B������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bIsCheckPoint;

	//�S�[���p��UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> GoalWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* GoalWidget;

	//�S�[�����������肷�t���O
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
	bool bIsGoal;
	
	//�`�F�b�N�|�C���g�̃A�N�^�[�����p
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		TSubclassOf < class AActor > CheckPointActorClass;

	//�`�F�b�N�|�C���g�̌������������i�[����ϐ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 CheckPointCounter;

	//�`�F�b�N�|�C���g��ʂ������𐔂���ϐ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 CurrentCheckPoint;

	//�A�N�Z���l���猸�Z����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		float DownSpeedOffset;

	//�Q���l������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GamePlayerNum;

	//�Q�[���C���X�^���X
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		class URaceGameInstance* RaceGameInstance;

	//�Q�[�����Ԃ̃J�E���g����
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bisTimerStart;

	//���ԃJ�E���g�p�̃^�C�}�[�n���h���[
	FTimerHandle GameTimerCountHandler;

	//�Q�[�����ԃJ�E���g�ϐ�
	UPROPERTY(Replicated ,VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		float GameTimer;







protected:
	//===========================���\�b�h
	//�R���X�g���N�^
	AMyRaceGamePawn();

	virtual void BeginPlay() override;

	// Begin Actor interface
	virtual void Tick(float Delta) override;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	//�@��������v���p�e�B
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//inputcalc�ϐ��̃Z�b�^�[
	UFUNCTION()
		void InputReset();
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


	//�S�[������̏���
	UFUNCTION(BlueprintCallable)
		void GoalCheck();
	
	//�`�F�b�N�|�C���g���B���菈��
	UFUNCTION(BlueprintCallable)
		void CheckPointRun();

	//���X�|�[�������i�]�|���̋~�ρj
	UFUNCTION()
		void Reapawn();

	UFUNCTION(Server, unreliable)
		void RespawnRunonServer();

	//�^�C�g���ɖ߂�
	UFUNCTION()
		void MoveTitle();

	//�Q�[���I��
	UFUNCTION()
		void GameEnd();

	//�Q�[�����ԃJ�E���g
	UFUNCTION()
		void GameTimeCounter();

	UFUNCTION()
		void ItemUseMultiCast();

	UFUNCTION()
		void ItemUseRunonServer();




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
	
};
