// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RaceSettingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API URaceSettingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URaceSettingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//----�ϐ�
		//�S�[���ɕK�v�ȃ��b�v��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//���݉����b�v���������J�E���g����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//�`�F�b�N�|�C���g�ɓ��B������
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bIsCheckPoint;

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
	//UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		float GameTimer;

	UPROPERTY()
		class APawn *OwnerPawn;

protected:
	//---�֐�

	
	//�@��������v���p�e�B
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//�S�[������̏���
	UFUNCTION(BlueprintCallable)
		void GoalCheck();

	//�`�F�b�N�|�C���g���B���菈��
	UFUNCTION(BlueprintCallable)
		void CheckPointRun();

	//�Q�[�����ԃJ�E���g
	UFUNCTION()
		void GameTimeCounter();

public:
	UFUNCTION()
		void InitSetting();

	UFUNCTION()
		void RaceSettingTick();

	




};
