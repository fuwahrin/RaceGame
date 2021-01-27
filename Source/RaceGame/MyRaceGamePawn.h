// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RaceGamePawn.h"

#include "Engine/DataTable.h"

#include "MyRaceGamePawn.generated.h"

/**
 * 
 */


 //クラスの前方宣言
class UInputComponent;
class UTexture2D;


UCLASS()
class RACEGAME_API AMyRaceGamePawn : public ARaceGamePawn
{
	GENERATED_BODY()

protected:
	//==========================コンポーネント

	//アイテムの出現位置
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* ItemSpawnPoint;

	//オフロードチェック用のコライダー
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* WheelColliderGroup;

	//オフロードチェック用のコライダー
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ForwardCollision;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* BackwardCollision;


protected:
	//==========================変数
	//UIのインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> GameUiClass;

	//UIの実数を格納する変数
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;

	//アイテムのID
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		int32 ItemNumber;

	//Spanwさせるアイテム
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemClass;


	//アイテムの画像
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		class UTexture2D *DrawIcon;

	//スポーンさせる際のScale
	UPROPERTY(Replicated , VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		float ItemScale;

	//アイテムが使用できる状態か確認する変数
	UPROPERTY(Replicated , EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		bool bIsItemUse;

	//アイテムの種類
	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> BulletItem;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SlipItem;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SpeedUpItem;

	//データテーブル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
		class UDataTable* ItemDataTable;

	//ブレーキ解除のタイムハンドラー
	FTimerHandle BrakeTimeHandler;

	//ゴールに必要なラップ数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//現在何ラップ走ったかカウントする変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//チェックポイントに到達したか
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		bool bIsCheckPoint;

	//ゴール用のUI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> GoalWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUserWidget* GoalWidget;

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

	//アクセル値から減算する変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		float DownSpeedOffset;

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
	UPROPERTY(Replicated ,VisibleDefaultsOnly, BlueprintReadOnly, Category = "Race")
		float GameTimer;







protected:
	//===========================メソッド
	//コンストラクタ
	AMyRaceGamePawn();

	virtual void BeginPlay() override;

	// Begin Actor interface
	virtual void Tick(float Delta) override;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	//　同期するプロパティ
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//アイテムを出現させるメソッド
	UFUNCTION(NetMulticast , unreliable )
		void SpawnItemMulticast();
	UFUNCTION(Server , unreliable , WithValidation)
		void SpawnItemRunonServer();
	UFUNCTION()
		void SpawnItem();

	//アイテムを使用したときのメソッド
	UFUNCTION()
		void ItemUse();

	//アイテムをスポーンさせる際の設定
	UFUNCTION()
		void ItemSetting(int32 ItemNum);
	UFUNCTION(Server , unreliable)
		void ItemSettingRunOnServer(int32 ItemNum);

	UFUNCTION()
		void SpawnSetting();
	UFUNCTION(Server, unreliable)
		void SpawnSettingRunOnServer();


	//inputcalc変数のセッター
	UFUNCTION()
		void InputReset();
public:
	//クラッシュイベント
	UFUNCTION()
		void CrashEvent();

	//スピードを調整するイベント
	UFUNCTION()
		void SpeedCalcFunction(float SpeedMultipication);

	//アクセルに掛ける乗算値を設定するメソッド
	UFUNCTION()
		void AccelSetting(UPrimitiveComponent* HitComponent , bool bIsSpeedDown);

	//アイテムを取得するときに呼ばれるメソッド
	UFUNCTION()
		void ItemPickup(int ItemNum);

	//ゴール判定の処理
	UFUNCTION(BlueprintCallable)
		void GoalCheck();
	
	//チェックポイント到達判定処理
	UFUNCTION(BlueprintCallable)
		void CheckPointRun();

	//リスポーン処理（転倒時の救済）
	UFUNCTION()
		void Reapawn();

	UFUNCTION(Server, unreliable)
		void RespawnRunonServer();

	//タイトルに戻る
	UFUNCTION()
		void MoveTitle();

	//ゲーム終了
	UFUNCTION()
		void GameEnd();

	//ゲーム時間カウント
	UFUNCTION()
		void GameTimeCounter();



	//-------------当たり判定
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
