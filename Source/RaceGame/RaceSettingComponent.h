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
	//----変数
		//ゴールに必要なラップ数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//現在何ラップ走ったかカウントする変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//チェックポイントに到達したか
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bIsCheckPoint;

	//ゴールしたか判定すつフラグ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bIsGoal;

	//チェックポイントのアクター検索用
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		TSubclassOf < class AActor > CheckPointActorClass;

	//チェックポイントの見つかった数を格納する変数
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 CheckPointCounter;

	//チェックポイントを通った数を数える変数
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 CurrentCheckPoint;

	//参加人数判定
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GamePlayerNum;

	//ゲームインスタンス
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		class URaceGameInstance* RaceGameInstance;

	//ゲーム時間のカウント処理
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bisTimerStart;

	//時間カウント用のタイマーハンドラー
	FTimerHandle GameTimerCountHandler;

	//ゲーム時間カウント変数
	//UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		float GameTimer;

	UPROPERTY()
		class APawn *OwnerPawn;

protected:
	//---関数

	
	//　同期するプロパティ
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//ゴール判定の処理
	UFUNCTION(BlueprintCallable)
		void GoalCheck();

	//チェックポイント到達判定処理
	UFUNCTION(BlueprintCallable)
		void CheckPointRun();

	//ゲーム時間カウント
	UFUNCTION()
		void GameTimeCounter();

public:
	UFUNCTION()
		void InitSetting();

	UFUNCTION()
		void RaceSettingTick();

	




};
