// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRaceGamePawn.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "ItemSettingComponent.h"
#include "RaceSettingComponent.h"


// コンストラクタ
AMyRaceGamePawn::AMyRaceGamePawn()
{
	USkeletalMeshComponent* OwnerMesh = GetMesh();
	OwnerMesh->SetCollisionProfileName(TEXT("MyRacePawn"));

	

	//オフロード確認用のタイヤコリジョンコンポーネントの設定
	WheelColliderGroup = CreateDefaultSubobject<USceneComponent>(TEXT("WheelColiderGroup"));
	WheelColliderGroup->SetupAttachment(OwnerMesh);

	//アイテム管理のコンポーネント設定
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));
	ItemSettingComponent->ItemSpawnPoint->SetupAttachment(GetMesh());

	float radius = 15.0f;
	ForwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ForwardColision"));
	ForwardCollision->SetupAttachment(WheelColliderGroup);
	ForwardCollision->SetRelativeLocation(FVector(-50.0f, 0.0f, 10.0f));
	ForwardCollision->InitSphereRadius(radius);
	ForwardCollision->SetCollisionProfileName(TEXT("Wheel"));

	BackwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BackwardColision"));
	BackwardCollision->SetupAttachment(WheelColliderGroup);
	BackwardCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 10.0f));
	BackwardCollision->InitSphereRadius(radius);
	BackwardCollision->SetCollisionProfileName(TEXT("Wheel"));

	
	//レース状態を管理するコンポーネント
	RaceSettingComponent = CreateDefaultSubobject<URaceSettingComponent>(TEXT("RaceSettingComponent"));
	
}

// BeginPlay
void AMyRaceGamePawn::BeginPlay()
{
	Super::BeginPlay();

	//コライダーイベントの登録
	ForwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionBeginOverlap);
	ForwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionEndOverlap);
	BackwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionBeginOverlap);
	BackwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionEndOverlap);

}

// Tick
void AMyRaceGamePawn::Tick(float Delta)
{
	Super::Tick(Delta);
}

// インプット登録
void AMyRaceGamePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アイテム出現ボタン
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUse);

	//リスポーン
	PlayerInputComponent->BindAction("PlayerRespawn", IE_Pressed, this, &AMyRaceGamePawn::Respawn);
	
}

//入力値のリセット
void AMyRaceGamePawn::InputReset()
{
	InputCalc = 1.0f;
	
	//アイテムを使用可能状態にする。
	ItemSettingComponent->SetItemUse(true);
}


//　タイヤのコライダー判定（前輪）※BeginOverlap
void AMyRaceGamePawn::ForwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//　タイヤのコライダー判定(前輪）※Endlap
void AMyRaceGamePawn::ForwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//　タイヤのコライダー判定（後輪）※BeginOverlap
void AMyRaceGamePawn::BackwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//　タイヤのコライダー判定(後輪）※Endlap
void AMyRaceGamePawn::BackwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//クラッシュイベント
void AMyRaceGamePawn::CrashEvent()
{

	const float SpeedMultipication = 0.0f;

	//0を乗算することによってスピードをなくす。
	SpeedCalcFunction(SpeedMultipication);

	//入力値に掛けている値を0にする。（入力しても進ませない為）
	InputCalc = 0.0f;

	//アイテムを使用できないようにする、
	ItemSettingComponent->SetItemUse(true);


	//一定時間完了後入力値に掛けている値をもとに戻す
	float DelayTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(BrakeTimeHandler, this, &AMyRaceGamePawn::InputReset, DelayTime, false);


}

//	スピードを調整するイベント
void AMyRaceGamePawn::SpeedCalcFunction(float SpeedMultipication)
{
	//正面のベクトルに引数で指定した値を乗算する
	FVector NewVelocity = GetMesh()->GetForwardVector() * SpeedMultipication;
	GetMesh()->SetPhysicsLinearVelocity(NewVelocity);

}

//減速処理
void AMyRaceGamePawn::AccelSetting(UPrimitiveComponent* HitComponent, bool bIsSpeedDown)
{
	//UE_LOG(LogTemp, Log, TEXT("AccleSettingOverlap"));

	//ブレーキ値に加算減算させる値
	const float BrakeValue = 0.25f;

	//最大のインプット値
	const float InputCalcMax = 1.0f;


	//減速するかどうか？
	if (bIsSpeedDown)
	{
		//減速を行う場合は減速値としてブレーキ値を減速値に加える
		DownSpeedOffset += BrakeValue;
	}
	else
	{
		//減速をやめる場合はブレーキ値を減速値から引く
		DownSpeedOffset += (BrakeValue * -1);
	}

	//インプット値にブレーキ値の計算を加える
	InputCalc = InputCalcMax - DownSpeedOffset;

}

//	プレイヤーリスポーン（救済措置）
void AMyRaceGamePawn::Respawn()
{
	//サーバー側で呼ばれているなら処理を行う
	if (this->HasAuthority())
	{
		//リスポーン位置の調整値
		FVector VectorOffset = FVector(0.0f, 0.0f, 20.0f);
		//リスポーン位置の設定
		FVector RespawnLocation = GetMesh()->GetComponentLocation() + VectorOffset;
		FTransform RespawnTransform = FTransform(RespawnLocation);
		GetMesh()->SetWorldTransform(RespawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		//クライアント側ならサーバで実行するように処理する
		RespawnRunonServer();
	}
	
}

void AMyRaceGamePawn::RespawnRunonServer_Implementation()
{
	Respawn();
}


void AMyRaceGamePawn::ItemUse()
{
	ItemSettingComponent->SpawnItem();
}



