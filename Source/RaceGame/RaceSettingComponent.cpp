// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceSettingComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "RaceGameInstance.h"
#include "UIManagerComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
URaceSettingComponent::URaceSettingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	//�ϐ��̏����l�ݒ�
	GamePlayerNum = 2;
	bisTimerStart = false;
	bIsCheckPoint = false;
	GameTimer = 0.0f;
	GoalLap = 3;

}


// Called when the game starts
void URaceSettingComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSetting();
	
}


// Called every frame
void URaceSettingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RaceSettingTick();

}

void URaceSettingComponent::InitSetting()
{
	// ...

	//�`�F�b�N�|�C���g�𐔂���
	TArray<AActor*>FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CheckPointActorClass, FindActors);

	//������������ϐ��Ɋi�[����
	CheckPointCounter = FindActors.Num();
	//UE_LOG(LogTemp, Log, TEXT("Checkpoint = %d"), CheckPointCounter);

	//�Q���l���̃J�E���g(GameInstance�ŊǗ����Ă���)
	RaceGameInstance = URaceGameInstance::GetInstance();

	if (RaceGameInstance)
	{
		RaceGameInstance->PlayerCounter++;
	}

	//UI�R���|�[�l���g�����̃R���|�[�l���g�̏��L�҂���T��
	UIManagerComponent = GetOwner()->FindComponentByClass<UUIManagerComponent>();
}

void URaceSettingComponent::RaceSettingTick()
{

	if (RaceGameInstance)
	{
		//�Q���l�����������������
		if (RaceGameInstance->PlayerCounter == GamePlayerNum)
		{
			//���ԃJ�E���g���J�n���Ă��Ȃ����
			if (!bisTimerStart)
			{
				float TimeSpan = 1.0f;
				//���ԃJ�E���g���J�n
				GetWorld()->GetTimerManager().SetTimer(GameTimerCountHandler, this, &URaceSettingComponent::GameTimeCounter, TimeSpan, true);
				bisTimerStart = true;
			}

		}
	}
}





//	�S�[��������s�����\�b�h
void URaceSettingComponent::GoalCheck()
{
	if (UIManagerComponent != nullptr)
	{
		//�`�F�b�N�|�C���g�ɓ��B������
		if (bIsCheckPoint)
		{
			//���B���Ă����烉�b�v�����Z
			LapCounter++;

			//���b�v�����S�[�����b�v���ɓ��B���Ă���΃S�[������
			if (LapCounter == GoalLap)
			{
				UIManagerComponent->ShowGoalWidget();
				bIsGoal = true;
				CurrentCheckPoint = 0;
				APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
				if (PC)
				{
					PC->bShowMouseCursor = true;
				}

			}
			else
			{
				//���b�v�����S�[�����b�v�ɓ��B���Ă��Ȃ������ꍇ
				//���b�v���B�`�F�b�N��OFF�ɂ���B
				bIsCheckPoint = false;

				//�`�F�b�N�|�C���g�����Z�b�g
				CurrentCheckPoint = 0;
			}
			//UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
		}
	}
}

//	�`�F�b�N�|�C���g�ʉߎ��ɌĂ΂�郁�\�b�h
void URaceSettingComponent::CheckPointRun()
{
	//�ʉ߂����`�F�b�N�|�C���g�𐔂���
	CurrentCheckPoint++;

	//�ʉ߂��������P���ɕK�v�ȃ`�F�b�N�|�C���g���������Ă�����
	if (CurrentCheckPoint >= CheckPointCounter)
	{
		//�P�������L���ɂ���B
		bIsCheckPoint = true;

		//�ʉ߂����`�F�b�N�|�C���g����������
		CurrentCheckPoint = 0;
	}

	//UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
}


//�Q�[�����ԃJ�E���g
void URaceSettingComponent::GameTimeCounter()
{
	//�S�[�����Ă��Ȃ����
	if (!bIsGoal)
	{
		//���Ԃ��J�E���g
		GameTimer += 1.0f;
	}
	
}


//void URaceSettingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	DOREPLIFETIME(URaceSettingComponent, GameTimer);
//
//}

