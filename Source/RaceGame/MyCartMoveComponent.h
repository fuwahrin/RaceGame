// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyCartMoveComponent.generated.h"



USTRUCT()
struct FMyPawnMove
{
	GENERATED_USTRUCT_BODY()

		//スロットル
		UPROPERTY()
		float Throttle;

	//ステアリング
	UPROPERTY()
		float SteeringThrow;

	//時間
	UPROPERTY()
		float DeltaTime;

	//移動を開始した時間
	UPROPERTY()
		float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UMyCartMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCartMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	//
	
	//スロットル
	float Throttle;

	//ステアリング
	float SteeringThrow;


	//スロットルが最大の時に車に加えられる力(N)
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	//フルスロットルで1秒あたりに回転する最小回転半径(m)
	UPROPERTY(EditAnywhere)
	float MinTurnningRadius = 10.0f;

	//空気抵抗の係数(値を大きくすると抵抗が大きくなる)
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//摩擦力の係数(値を大きくすると抵抗が大きくなる)
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015f;

	//車の質量(kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedUpRate =1.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	//速度調整
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void SpeedCalc(float Rate, float DrivingForce);

	//速度アップ
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void SpeedUp();

	//速度ダウン
	UFUNCTION(BlueprintCallable, Category = "Speed")
	void SpeedDown();


public:
	//移動計算
	void UpdateLocationFromVelocity(float DeltaTime);

	//向き計算
	void ApplyRotation(float DeltaTime, float Steering);

	//空気抵抗力取得
	FVector GetAirResistance();

	//摩擦力取得
	FVector GetRollingResistance();

	//シミュレーション移動
	void SimulateMove(const FMyPawnMove& move);

	//移動作成関数
	FMyPawnMove CreateMove(float DeltaTime);


	//セッターゲッター

	//ベクトル
	void SetVelocity(FVector value) { Velocity = value; }
	FVector GetVelocity() { return Velocity; }

	//スロットル
	void SetThrottle(float value) { Throttle = value; }

	//ステアリング
	void SetSteeringThrow(float value) { SteeringThrow = value; }

	//最後の移動情報
	FMyPawnMove LastMove;

	//最終移動のゲッター
	FMyPawnMove GetLastMove() { return LastMove; }
	
};
