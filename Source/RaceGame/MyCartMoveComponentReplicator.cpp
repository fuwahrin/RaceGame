// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCartMoveComponentReplicator.h"
#include "Runtime/Engine/Public/Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UMyCartMoveComponentReplicator::UMyCartMoveComponentReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//このコンポーネントの同期を有効にする
	SetIsReplicatedByDefault(true);
	
	
}


// Called when the game starts
void UMyCartMoveComponentReplicator::BeginPlay()
{
	Super::BeginPlay();

	//移動コンポーネントをこのコンポーネントの所有者から探す
	MovementComponent = GetOwner()->FindComponentByClass<UMyCartMoveComponent>();
}


// Called every frame
void UMyCartMoveComponentReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//移動コンポーネントがセットされていなければ処理を行わない
	if (MovementComponent == nullptr) return;

	FMyPawnMove LastMove = MovementComponent->GetLastMove();

	//権限によって各Pawnの動きを作成

	//クライアント側で操作しているPawnの動きをサーバーに複製させる
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		if (MovementComponent->bIsMoveStart)
		{

			//サーバーが未確認の動きを登録
			UnacknowledgedMoves.Add(MovementComponent->LastMove);

			//未確認の動きの数を確認
			//UE_LOG(LogTemp, Warning, TEXT("Queue length: %d"), UnacknowledgedMoves.Num());
			//サーバに移動情報を送信
			Server_SendMove(LastMove);
			//UE_LOG(LogTemp, Warning, TEXT("Thottole = %f"), LastMove.Throttle);
		}
	
	}

	//サーバー側で自身が操作するPawnの動きをクライアント側に複製させる
	//※RemoteRoleはマップ遷移すると正しい所有者を取得できない為使用せず
	//参考：https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/Actors/Roles/index.html
	if(GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
	{
		//UE_LOG(LogTemp, Error, TEXT("Server"));
		//サーバーの移動情報を更新する
		if (MovementComponent->bIsMoveStart)
		{

			UpdateServerState(LastMove);
		}
	}
	
	//クライアント側で複製されているPawnなら
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		//UE_LOG(LogTemp, Error, TEXT("Cliant"));
	
		CliantTick(DeltaTime);
	}

}
void UMyCartMoveComponentReplicator::Server_SendMove_Implementation(FMyPawnMove move)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SimulateMove(move);

	UpdateServerState(move);


}

void UMyCartMoveComponentReplicator::UpdateServerState(const FMyPawnMove& move)
{
	//同期用の変数に現在の移動やベクトル情報を格納する。
	ServerState.LastMove = move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();

}

void UMyCartMoveComponentReplicator::CliantTick(float DeltaTime)
{
	//クライアント側で毎フレーム行われる処理
	CliantTimeSinceUpdate += DeltaTime;

	//前回の更新がすごく小さい値なら処理を行わない
	if (CliantTimeBetweenLastUpdate < KINDA_SMALL_NUMBER) return;

	//移動コンポーネントが設定されていなければ処理を行わない。
	if (MovementComponent == nullptr) return;

	float LerpAlpha = CliantTimeSinceUpdate / CliantTimeBetweenLastUpdate;

	FHermiteCubicSpline Spline = CreateSpline();
	
	//キュービック補間位置を算出
	InterpoteLocation(Spline, LerpAlpha);

	//キービック補間の速度を算出
	InterpolateVelocity(Spline, LerpAlpha);

	//キュービック補間の角度を算出
	InterpolateRotation(LerpAlpha);
}

void UMyCartMoveComponentReplicator::InterpolateRotation(float LerpAlpha)
{
	//角度計算
	FQuat TargetRotation = ServerState.Transform.GetRotation();
	//開始角度を取得
	FQuat StartRotation = CliantStartTransform.GetRotation();

	//線形移動ベクトルを算出
	FQuat  NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpAlpha);
	
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldRotation(NewRotation);
	}
}

float UMyCartMoveComponentReplicator::VelocityToDerivarite()
{
	return  CliantTimeBetweenLastUpdate * 100;
}

//キュービック補間位置を算出
void UMyCartMoveComponentReplicator::InterpoteLocation(const FHermiteCubicSpline& Spline, float LerpAlpha)
{

	FVector NewLocation = Spline.InterpolateLocation(LerpAlpha);
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldLocation(NewLocation);
	}
}

void UMyCartMoveComponentReplicator::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpAlpha)
{
	FVector NewDerivaive = Spline.InterpolateDeriative(LerpAlpha);
	FVector NewVelocity = NewDerivaive / VelocityToDerivarite();
	MovementComponent->SetVelocity(NewVelocity);
}


FHermiteCubicSpline UMyCartMoveComponentReplicator::CreateSpline()
{

	//キュービック補間の構造体を宣言
	FHermiteCubicSpline Spline;

	//移動位置を計算
	Spline.TargetLocation = ServerState.Transform.GetLocation();
	
	//開始位置を取得
	Spline.StartLocation = CliantStartTransform.GetLocation();

	//キュービック補間用の値
	Spline.StartDerivate = CliantStartVelocity * VelocityToDerivarite();
	Spline.TargetDerivate = ServerState.Velocity * VelocityToDerivarite();

	return Spline;

}



void UMyCartMoveComponentReplicator::ClearUnacknowledgedMoves(FMyPawnMove LastMove)
{
	TArray<FMyPawnMove> newMove;

	for (const FMyPawnMove& move : UnacknowledgedMoves)
	{
		//引数で渡されてきた時間よりも多い時間なら（未確認の移動なら）
		if (move.Time > LastMove.Time)
		{
			//新しい未確認の動きを登録
			newMove.Add(move);
		}
	}

	//登録している情報を上書き
	UnacknowledgedMoves = newMove;
}



void UMyCartMoveComponentReplicator::OnRep_ServerState()
{
	//付与された権限によって処理を変える
	switch (GetOwnerRole())
	{
		case ROLE_AutonomousProxy:
			//サーバー側
			AutonomousProxy_OnRep_ServerState();
			break;

		case ROLE_SimulatedProxy:
			//クライアント側
			SimulatedProxy_OnRep_ServerState();
			break;

	}
	
}

FVector UMyCartMoveComponentReplicator::GetSimulateLocation()
{
	return MeshOffsetRoot->GetComponentLocation();
}

void UMyCartMoveComponentReplicator::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	//更新時間を格納
	CliantTimeBetweenLastUpdate = CliantTimeSinceUpdate;

	//更新カウンターをリセット
	CliantTimeSinceUpdate = 0;

	//移動開始位置を格納
	if (MeshOffsetRoot != nullptr)
	{
		//コライダーの位置から取得
		CliantStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		CliantStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	
	//移動速度を格納
	CliantStartVelocity = MovementComponent->GetVelocity();

	//所有者の位置を変更
	GetOwner()->SetActorTransform(ServerState.Transform);

	


}

//権限がサーバ側だったときの同期処理
void UMyCartMoveComponentReplicator::AutonomousProxy_OnRep_ServerState()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReplicatedLocation"));

	if (MovementComponent == nullptr) return;

	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	//未確認の動きのリストを更新する
	ClearUnacknowledgedMoves(ServerState.LastMove);

	//未確認の動きリストの分の動きを行う
	for (const FMyPawnMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UMyCartMoveComponentReplicator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMyCartMoveComponentReplicator, ServerState);

}




