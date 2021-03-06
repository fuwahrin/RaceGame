// Fill ot your copyright notice in the Description page of Project Settings.


#include "MyCartMoveComponent.h"
#include "GameFramework/GameStateBase.h"
#include "DrawDebugHelpers.h"



// Sets default values for this component's properties
UMyCartMoveComponent::UMyCartMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//変数の設定
	InitMaxDrivingForce = MaxDrivingForce;
}


// Called when the game starts
void UMyCartMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyCartMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//クライアント側か自身が操作するPawnなら
	//参考：https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/Actors/Roles/index.html
	//UE_LOG(LogTemp , Error , GetOwner()->GetInstigator<APawn>()->IsLocallyControlled());
	if (GetOwnerRole() == ROLE_AutonomousProxy || //クライアント側で操作しているか 
		GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())//サーバ側で操作しているか※（）RemoteRoleはマップ遷移すると正しい所有者を取得できない為使用せず
	{

		//動きを作成
		LastMove = CreateMove(DeltaTime);

		//シミュレーション移動を行う
		SimulateMove(LastMove);
		
	}
}

//スピードアップ値の調整
void UMyCartMoveComponent::SpeedCalc(float Rate)
{
	//スロットルに与える乗算値を変更
	SpeedUpRate = Rate;

}

//速度調整のタイムイベント関数
void UMyCartMoveComponent::SpeedCalcTimeEvent(float Rate, float CallTime)
{
	//イベント用のハンドラーとデリゲート変数を宣言
	FTimerHandle SpeedHandler;
	FTimerDelegate SpeedDelegate;

	//速度調整の関数を登録
	SpeedDelegate.BindUFunction(this, FName(TEXT("SpeedCalc")), Rate);

	//UE_LOG(LogTemp, Warning, TEXT("RateInput = %f"), Rate);

	//イベント登録した関数を呼ぶ
	GetWorld()->GetTimerManager().SetTimer(SpeedHandler, SpeedDelegate, CallTime, false);
}

void UMyCartMoveComponent::SpeedUpEvent()
{
	float MaxSpeedRate = 10.0f;
	float CallTime = 0.1f;
	float Default = 1.0f;

	//TODO:マジックNO修正
	SpeedCalcTimeEvent(MaxSpeedRate, CallTime);
	SpeedCalcTimeEvent(Default, Default);
}


//クラッシュイベント
void UMyCartMoveComponent::CrashEvent()
{
	float CrashRate = 0.0f;
	float CallTime = 0.1f;
	float Default = 1.0f;

	//速度をリセットする。
	Velocity = FVector::ZeroVector;
	SpeedCalcTimeEvent(CrashRate, CallTime);
	SpeedCalcTimeEvent(Default, Default);
}

//移動計算
void UMyCartMoveComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	//あたり判定用の変数
	FHitResult Hit;

	//移動処理
	FVector Translation = Velocity * 100 * DeltaTime;
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);


	//壁などに当たっていた時の処理
	if (Hit.IsValidBlockingHit())
	{
		//速度をリセットする。
		Velocity = FVector::ZeroVector;
	}
}

//向き計算
void UMyCartMoveComponent::ApplyRotation(float DeltaTime, float Steering)
{
	//向き計算

	//ベクトルの長さ
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;

	//左右の角度の度数
	float RoationAngle = DeltaLocation * MinTurnningRadius * Steering;

	//プレイヤーの向き
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), FMath::DegreesToRadians(RoationAngle));

	Velocity = RotationDelta.RotateVector(Velocity);

	//アクターに向きを適用
	GetOwner()->AddActorWorldRotation(RotationDelta);
}


//空気抵抗力取得
FVector UMyCartMoveComponent::GetAirResistance()
{
	//空気抵抗力　= -正規化した速度のベクトル　* 速度ベクトルの２乗 * 空気抵抗係数
	return  -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}


//転がり抵抗力計算
FVector UMyCartMoveComponent::GetRollingResistance()
{
	
	//重力
	//UE_LOG(LogTemp, Warning, TEXT("Gravity: %f"), GetWorld()->GetGravityZ());
	float accelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;

	float NormalForce = Mass * accelerationDueToGravity;
	//転がり抵抗 = 正規化した速度のベクトル　* 転がり抵抗係数 * 法線ベクトル
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}




//シミュレーション移動
void UMyCartMoveComponent::SimulateMove(const FMyPawnMove& move)
{
	//アクセル力
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * move.Throttle;

	
	//空気抵抗
	Force += GetAirResistance();
	//転がり抵抗
	Force += GetRollingResistance();



	//加速力 = アクセル力 / 質量
	FVector Accleration = Force / Mass;

	//速度計算
	Velocity = Velocity + Accleration * move.DeltaTime;

	//UE_LOG(LogTemp, Warning, TEXT("FORCE : %f"), Force.Size());
	//UE_LOG(LogTemp, Warning, TEXT("Velocity : %f"), Velocity.Size());


	ApplyRotation(move.DeltaTime, move.SteeringThrow);

	UpdateLocationFromVelocity(move.DeltaTime);

}


//移動作成
FMyPawnMove UMyCartMoveComponent::CreateMove(float DeltaTime)
{
	//カーブ時の減速値設定
	if (SteeringThrow != 0.0f)
	{
		SteeringRate = 0.5f;
	}
	else
	{
		SteeringRate = 1.0f;
	}

	//UE_LOG(LogTemp, Error, TEXT("SteeringRate = %f"), SteeringRate);
	FMyPawnMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = (Throttle * (SpeedUpRate * RoadSpeedRate) * SteeringRate);
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	return Move;

}


//スロットル値のセット＆同期タイミングのセット
void UMyCartMoveComponent::SetThrottle(float value)
{
	
	if (!bIsMoveStart && value != 0.0f)
	{
		//移動の同期フラグがOFFなら動き始めた際にＯＮにする
		//※Exeファイル実行時にクライアント側のPawnがDestroyされてしまう防止
		bIsMoveStart = true;
	}
	Throttle = value;
	//UE_LOG(LogTemp, Warning, TEXT("Throttle = %f"), Throttle);
}



