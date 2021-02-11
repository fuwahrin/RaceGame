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

	//変数の初期値設定
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

	//チェックポイントを数える
	TArray<AActor*>FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CheckPointActorClass, FindActors);

	//見つかった数を変数に格納する
	CheckPointCounter = FindActors.Num();
	//UE_LOG(LogTemp, Log, TEXT("Checkpoint = %d"), CheckPointCounter);

	//参加人数のカウント(GameInstanceで管理している)
	RaceGameInstance = URaceGameInstance::GetInstance();

	if (RaceGameInstance)
	{
		RaceGameInstance->PlayerCounter++;
	}

	//UIコンポーネントをこのコンポーネントの所有者から探す
	UIManagerComponent = GetOwner()->FindComponentByClass<UUIManagerComponent>();
}

void URaceSettingComponent::RaceSettingTick()
{

	if (RaceGameInstance)
	{
		//参加人数がそろったか判定
		if (RaceGameInstance->PlayerCounter == GamePlayerNum)
		{
			//時間カウントを開始していなければ
			if (!bisTimerStart)
			{
				float TimeSpan = 1.0f;
				//時間カウントを開始
				GetWorld()->GetTimerManager().SetTimer(GameTimerCountHandler, this, &URaceSettingComponent::GameTimeCounter, TimeSpan, true);
				bisTimerStart = true;
			}

		}
	}
}





//	ゴール判定を行うメソッド
void URaceSettingComponent::GoalCheck()
{
	if (UIManagerComponent != nullptr)
	{
		//チェックポイントに到達したか
		if (bIsCheckPoint)
		{
			//到達していたらラップを加算
			LapCounter++;

			//ラップ数がゴールラップ数に到達していればゴール判定
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
				//ラップ数がゴールラップに到達していなかった場合
				//ラップ到達チェックをOFFにする。
				bIsCheckPoint = false;

				//チェックポイント数リセット
				CurrentCheckPoint = 0;
			}
			//UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
		}
	}
}

//	チェックポイント通過時に呼ばれるメソッド
void URaceSettingComponent::CheckPointRun()
{
	//通過したチェックポイントを数える
	CurrentCheckPoint++;

	//通過した数が１周に必要なチェックポイント数を上回っていたら
	if (CurrentCheckPoint >= CheckPointCounter)
	{
		//１周判定を有効にする。
		bIsCheckPoint = true;

		//通過したチェックポイント数を初期化
		CurrentCheckPoint = 0;
	}

	//UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
}


//ゲーム時間カウント
void URaceSettingComponent::GameTimeCounter()
{
	//ゴールしていなければ
	if (!bIsGoal)
	{
		//時間をカウント
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

