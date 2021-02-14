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

	// アクセル値から減算する変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
	float DownSpeedOffset;

	//==========================コンポーネント

	//オフロードチェック用のコライダー
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* WheelColliderGroup;

	//オフロードチェック用のコライダー
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* ForwardCollision;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* BackwardCollision;

	//アイテム管理を行うコンポーネント
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UItemSettingComponent* ItemSettingComponent;

	//レース状態を管理するコンポーネント
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class URaceSettingComponent* RaceSettingComponent;


protected:
	
	
	//ブレーキ解除のタイムハンドラー
	FTimerHandle BrakeTimeHandler;


protected:
	//===========================メソッド
	//コンストラクタ
	AMyRaceGamePawn();

	virtual void BeginPlay() override;

	// Begin Actor interface
	virtual void Tick(float Delta) override;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	
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



	//リスポーン処理（転倒時の救済）
	UFUNCTION()
		void Reapawn();

	UFUNCTION(Server , Reliable)
		void RespawnRunonServer();

	//タイトルに戻る
	UFUNCTION()
		void MoveTitle();

	
	//ゲーム終了
	UFUNCTION()
		void GameEnd();

	UFUNCTION()
		void ItemUse();




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
