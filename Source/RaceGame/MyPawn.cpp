// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ItemSettingComponent.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ItemBase.h"
#include "RayActor.h"
#include "RaceSettingComponent.h"


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

	//レイトレースの起点
	ForwardRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayForward"));
	ForwardRayPoint->SetupAttachment(MeshOffSetRoot);
	ForwardRayPoint->SetRelativeLocation(FVector(70.0f, 0.0f, 0.0f));

	BackwardRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayBackward"));
	BackwardRayPoint->SetupAttachment(MeshOffSetRoot);
	BackwardRayPoint->SetRelativeLocation(FVector(-70.0f, 0.0f, 0.0f));

	LeftRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayLeft"));
	LeftRayPoint->SetupAttachment(MeshOffSetRoot);
	LeftRayPoint->SetRelativeLocation(FVector(0.0f, -30.0f, 0.0f));

	RightRayPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RayRight"));
	RightRayPoint->SetupAttachment(MeshOffSetRoot);
	RightRayPoint->SetRelativeLocation(FVector(0.0f, 30.0f, 0.0f));

	// Setup the audio component and allocate it a sound cue
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/VehicleAdv/Sound/Engine_Loop_CueMyPawn.Engine_Loop_CueMyPawn"));
	
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(CarMesh);

	///---ActorComponentの設定
	//アイテム設定のコンポーネント
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(MeshOffSetRoot);

	//移動コンポーネントの設定
	Movement = CreateDefaultSubobject<UMyCartMoveComponent>(TEXT("Movement"));
	//移動同期コンポーネント設定
	MovementReplicator = CreateDefaultSubobject<UMyCartMoveComponentReplicator>(TEXT("MovementComponentReplicator"));

	
	//レース状態を管理するコンポーネント
	RaceSettingComponent = CreateDefaultSubobject<URaceSettingComponent>(TEXT("RaceSettingComponent"));
}
// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//権限の取得
	Role = GetLocalRole();

	//同期を行う秒数の設定（１秒）
	if (HasAuthority())
	{
		NetUpdateFrequency = 1.0f;
	}

	//レイトレースアクターをスポーンする。
	RayForward = SpawnRayActor(ForwardRayPoint);
	RayBackward = SpawnRayActor(BackwardRayPoint);
	RayLeft = SpawnRayActor(LeftRayPoint);
	RayRight = SpawnRayActor(RightRayPoint);

}


// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//プレイヤーの権限をデバッグ表示
	//DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);

	//レイトレース
	float forwardState =  RoadSpeedCalcFunction(RayForward);
	float backwardState = RoadSpeedCalcFunction(RayBackward);
	float leftState = RoadSpeedCalcFunction(RayLeft);
	float rightState = RoadSpeedCalcFunction(RayRight);

	//道路状態の速度を設定
	Movement->RoadSpeedRate = forwardState * backwardState * leftState * rightState;
	//UE_LOG(LogTemp, Warning, TEXT("SpeedUpRate = %f"), Movement->SpeedUpRate);

	float AudioScale = FVector::DotProduct(GetActorForwardVector(), Movement->Velocity);
	AudioScale = FMath::Clamp<float>(AudioScale ,0.0f , 2500.0f);
	EngineSoundComponent->SetFloatParameter("RPM", AudioScale);
	//UE_LOG(LogTemp, Warning, TEXT("AudioScale = %f"), AudioScale);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//キーアクション登録
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPawn::MoveRight);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyPawn::ItemUse);
}

//前進・後退
void AMyPawn::MoveForward(float value)
{
	if (Movement == nullptr) return;

	Movement->SetThrottle(value);
}

//左右
void AMyPawn::MoveRight(float value)
{
	if (Movement == nullptr) return;

	Movement->SetSteeringThrow(value);
}


//アイテムを仕様完了状態にするメソッド
void AMyPawn::ItemUse()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItem();
	}
}

//道路の速度値を取得するメソッド
float AMyPawn::RoadSpeedCalcFunction(ARayActor *rayActor)
{
	//スピードの計算用変数
	float SpeedRate = 1.0f;

	//道路のレイ判定を取得
	if (rayActor->IsStateChange())
	{
		//道路を走っているなら
		if (rayActor->GetRayHitState() == FName("Road"))
		{
			//最大速度を上回っていなければ
			if (SpeedRate < 1.0f)
			{
				//スピードを上げる
				SpeedRate += 0.3f;
			}
		}
		else
		{
			//最小速度を下回っていれば
			if (SpeedRate < 0.1f)
			{
				//速度を最小に
				SpeedRate = 0.1f;
			}
			else
			{
				//下回っていなければ速度を下げる
				SpeedRate -= 0.3f;
			}
		}
	}

	return SpeedRate;
}


//レイトレースのアクターを出現させる。
ARayActor* AMyPawn::SpawnRayActor(USceneComponent * parent)
{
	ARayActor* SpawnActor;
	//レイトーレス用のアクターをSpawnする。
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	//スポーン位置の設定(ItemSpawnPointのワールド座標を格納)
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	//スポーンする際のScale
	FVector SpawnScale = FVector::OneVector;
	SpawnActor = GetWorld()->SpawnActor<ARayActor>(RayActorClass, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnActor->AttachToComponent(parent , { EAttachmentRule::KeepRelative , true });

	return SpawnActor;
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

