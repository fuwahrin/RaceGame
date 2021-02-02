// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ItemSettingComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(false);


	//コンポーネントの設定
	
	//コライダー
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);

	//メッシュのオフセット
	MeshOffSetRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshOffset"));
	MeshOffSetRoot->SetupAttachment(BoxCollision);

	//メッシュ
	CarMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeltalMesh"));
	CarMesh->SetupAttachment(MeshOffSetRoot);

	//SplingArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MeshOffSetRoot);

	//カメラ
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	
	//アイテム設定のコンポーネント
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(MeshOffSetRoot);
	

	//移動コンポーネントの設定
	MovementComponent = CreateDefaultSubobject<UMyCartMoveComponent>(TEXT("MovementComponent"));

	//移動同期コンポーネント設定
	MovementComponentReplicator = CreateDefaultSubobject<UMyCartMoveComponentReplicator>(TEXT("MovementComponentReplicator"));

	


}
// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//権限の取得
	Role = GetLocalRole();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1.0f;
	}
	
}


// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//プレイヤーの権限をデバッグ表示
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);

}





// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//キーアクション登録
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);

	//アイテム出現ボタン
	//ネットワークの権限を確認
	if (this->HasAuthority())
	{
		//サーバー側ならマルチキャスト
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUseMultiCast);

	}
	else
	{
		//クライアント側ならサーバで実行させる。
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUseRunonServer);
	}

}


void AMyPawn::MoveForward(float value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(value);
}

void AMyPawn::MoveRight(float value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(value);
}

FString AMyPawn::GetEnumText(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "ROLE_MAX";
	default:
		return "ERROR";
	}
}

void AMyPawn::ItemUseMultiCast()
{
	//UE_LOG(LogTemp, Log, TEXT("ItemUseMultiCast(1)"));
	if (ItemSettingComponent != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("ItemUseMultiCast(2)"));
		ItemSettingComponent->SpawnItemMulticast();
	}
}

void AMyPawn::ItemUseRunonServer()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemRunonServer();
	}
}

