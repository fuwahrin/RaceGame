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

	//�A�C�e���̏o���ʒu
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ItemSpawnPoint;

	//�I�t���[�h�`�F�b�N�p�̃R���C�_�[
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* WheelColliderGroup;

	//�I�t���[�h�`�F�b�N�p�̃R���C�_�[
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ForwardCollision;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* BackwardCollision;


protected:
	//==========================�ϐ�
	//UI�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> GameUiClass;

	//UI�̎������i�[����ϐ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;

	//�A�C�e����ID
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 ItemNumber;

	//Spanw������A�C�e��
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemClass;


	//�A�C�e���̉摜
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		class UTexture2D *DrawIcon;

	//�X�|�[��������ۂ�Scale
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float ItemScale;

	//�A�C�e�����g�p�ł����Ԃ��m�F����ϐ�
	UPROPERTY(Replicated , EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		bool bIsItemUse;

	//�A�C�e���̎��
	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> BulletItem;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SlipItem;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SpeedUpItem;

	//�f�[�^�e�[�u��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
		class UDataTable* ItemDataTable;

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

	//�A�C�e�����o�������郁�\�b�h
	UFUNCTION(NetMulticast , unreliable )
		void SpawnItemMulticast();
	UFUNCTION(Server , unreliable , WithValidation)
		void SpawnItemRunonServer();
	UFUNCTION()
		void SpawnItem();

	//�A�C�e�����g�p�����Ƃ��̃��\�b�h
	UFUNCTION()
		void ItemUse();

	//�A�C�e�����X�|�[��������ۂ̐ݒ�
	UFUNCTION()
		void ItemSetting(int32 ItemNum);
	UFUNCTION(Server , unreliable)
		void ItemSettingRunOnServer(int32 ItemNum);

	UFUNCTION()
		void SpawnSetting();
	UFUNCTION(Server, unreliable)
		void SpawnSettingRunOnServer();


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

	//�A�C�e�����擾����Ƃ��ɌĂ΂�郁�\�b�h
	UFUNCTION()
		void ItemPickup(int ItemNum);

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
