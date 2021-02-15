// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.generated.h"



//同期用の移動の構造体
USTRUCT()
struct FMyPawnState
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FMyPawnMove LastMove;
};

//キュービック補間計算用の構造体
struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivate, TargetLocation, TargetDerivate;

	//補間位置を取得するメソッド
	FVector InterpolateLocation(float LerpAlpha) const
	{
		return FMath::CubicInterp(StartLocation, StartDerivate, TargetLocation, TargetDerivate, LerpAlpha);
	}
	//派生補間位置を取得するメソッド
	FVector InterpolateDeriative(float LerpAlpha) const 
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivate, TargetLocation, TargetDerivate, LerpAlpha);
	}

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UMyCartMoveComponentReplicator : public UActorComponent
{
	GENERATED_BODY()

public:
	//未確認の移動リスト
	TArray<FMyPawnMove> UnacknowledgedMoves;


private:
	//============変数

	//クライアント側の移動更新後の時間
	float CliantTimeSinceUpdate;

	//最終更新までの時間
	float CliantTimeBetweenLastUpdate;

	//クライアント側の計算開始位置
	FTransform CliantStartTransform;

	//クライアント側の計算開始時の速度
	FVector CliantStartVelocity;

	//同期用の位置
	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FMyPawnState ServerState;


private:
	//==========コンポーネント

	//移動のコンポーネント
	UPROPERTY()
		UMyCartMoveComponent* MovementComponent;

	//コライダーオフセット
	UPROPERTY()
		USceneComponent* MeshOffsetRoot;


public:	
	// Sets default values for this component's properties
	UMyCartMoveComponentReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	//移動処理メソッド
	UFUNCTION(Server, Reliable)
		void Server_SendMove(FMyPawnMove move);

	UFUNCTION()
		void OnRep_ServerState();

public:
	

	//サーバーの移動状態を更新する
	void UpdateServerState(const FMyPawnMove& move);

	//クライアント側のTick
	void CliantTick(float DeltaTime);

	//権限がサーバ側だったときの同期処理
	void AutonomousProxy_OnRep_ServerState();

	//権限がクライアント側だったときの同期処理
	void SimulatedProxy_OnRep_ServerState();

	
	//キュービック補間位置を算出する
	void InterpoteLocation(const FHermiteCubicSpline& Spline, float LerpAlpha);

	//キュービック補間の角度を算出する。
	void InterpolateRotation(float LerpAlpha);

	//キュービック補間速度を算出する。
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpAlpha);

	float VelocityToDerivarite();

	//キュービック補間のスプラインを作成するメソッド
	FHermiteCubicSpline CreateSpline();

	
	//未確認の移動を削除するメソッド
	void ClearUnacknowledgedMoves(FMyPawnMove LastMove);

	//シミュレーション移動先を取得
	UFUNCTION()
		FVector GetSimulateLocation();

	//コライダーオフセットの親コンポーネントを設定する。
	UFUNCTION(BlueprintCallable)
		void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
	
		
};
