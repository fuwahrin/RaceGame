// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRaceGamePawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "RaceGameInstance.h"
#include "ItemSettingComponent.h"


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




	//変数の初期値設定
	GamePlayerNum = 2;
	bisTimerStart = false;
	bIsCheckPoint = false;
	GameTimer = 0.0f;
	GoalLap = 3;

	
}

// BeginPlay
void AMyRaceGamePawn::BeginPlay()
{
	Super::BeginPlay();

	//UIが設定されているなら
	if (GameUiClass != nullptr)
	{
		//UIを作成
		GameUi = CreateWidget<UUserWidget>(GetWorld(), GameUiClass);

		//UIが無事に作成されたら表示する。
		if (GameUi != nullptr)
		{
			GameUi->AddToViewport();
		}
	}

	//GoalUIの作成
	if (GoalWidgetClass != nullptr)
	{
		GoalWidget = CreateWidget<UUserWidget>(GetWorld(), GoalWidgetClass);
	}

	//チェックポイントを数える
	TArray<AActor*>FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CheckPointActorClass, FindActors);

	//見つかった数を変数に格納する
	CheckPointCounter = FindActors.Num();
	UE_LOG(LogTemp, Log, TEXT("Checkpoint = %d"), CheckPointCounter);

	//コライダーイベントの登録
	ForwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionBeginOverlap);
	ForwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionEndOverlap);
	BackwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionBeginOverlap);
	BackwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionEndOverlap);

	//参加人数のカウント(GameInstanceで管理している)
	RaceGameInstance = URaceGameInstance::GetInstance();

	if (RaceGameInstance)
	{
		RaceGameInstance->PlayerCounter++;
	}


}

// Tick
void AMyRaceGamePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	if (RaceGameInstance)
	{
		//参加人数がそろったか判定
		if (RaceGameInstance->PlayerCounter == GamePlayerNum)
		{
			//時間カウントを開始していなければ
			if (!bisTimerStart)
			{
				float TimeSpan = 1.0f;
				//時間カウントを開始
				GetWorld()->GetTimerManager().SetTimer(GameTimerCountHandler, this , &AMyRaceGamePawn::GameTimeCounter, TimeSpan, true);
				bisTimerStart = true;
			}

		}
	}
}

// インプット登録
void AMyRaceGamePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アイテム出現ボタン
	//ネットワークの権限を確認
	if (this->HasAuthority())
	{
		//サーバー側ならマルチキャスト
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUseMultiCast);
		
	}
	else
	{
		//クライアント側ならサーバで実行させる。
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUseRunonServer);
	}

	//リスポーン
	PlayerInputComponent->BindAction("PlayerRespawn", IE_Pressed, this, &AMyRaceGamePawn::Reapawn);
	
	//ゲーム終了
	PlayerInputComponent->BindAction("EndGame", IE_Pressed, this, &AMyRaceGamePawn::GameEnd);

	//タイトルに戻る
	PlayerInputComponent->BindAction("GameQuit", IE_Pressed, this, &AMyRaceGamePawn::MoveTitle);
}



//	スピードを調整するイベント
void AMyRaceGamePawn::SpeedCalcFunction(float SpeedMultipication)
{
	//正面のベクトルに引数で指定した値を乗算する
	FVector NewVelocity = GetMesh()->GetForwardVector() * SpeedMultipication;
	GetMesh()->SetPhysicsLinearVelocity(NewVelocity);

}

//	ゴール判定を行うメソッド
void AMyRaceGamePawn::GoalCheck()
{
	//チェックポイントに到達したか
	if (bIsCheckPoint)
	{
		//到達していたらラップを加算
		LapCounter++;

		//ラップ数がゴールラップ数に到達していればゴール判定
		if (LapCounter == GoalLap)
		{
			GoalWidget->AddToViewport();
			bIsGoal = true;
			CurrentCheckPoint = 0;
			APlayerController *PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				PC->bShowMouseCursor = true;
			}

		}
		else
		{
			//ラップ数がゴールラップに到達していなかった場合
			//ラップ到達チェックをOFFにする。
			bIsCheckPoint = false;
			
			//チェックポイント数リセット
			CurrentCheckPoint = 0;
		}
		UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
	}
}

//	チェックポイント通過時に呼ばれるメソッド
void AMyRaceGamePawn::CheckPointRun()
{
	//通過したチェックポイントを数える
	CurrentCheckPoint++;

	//通過した数が１周に必要なチェックポイント数を上回っていたら
	if (CurrentCheckPoint >= CheckPointCounter)
	{
		//１周判定を有効にする。
		bIsCheckPoint = true;

		//通過したチェックポイント数を初期化
		CurrentCheckPoint = 0;
	}

	UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
}

//	プレイヤーリスポーン（救済措置）
void AMyRaceGamePawn::Reapawn()
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
	Reapawn();
}

//	タイトル画面に戻る
void AMyRaceGamePawn::MoveTitle()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainMenu")), true, TEXT("listen"));
}

//	ゲーム終了
void AMyRaceGamePawn::GameEnd()
{
	APlayerController *MyController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this ,MyController , EQuitPreference::Quit, false);
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
void AMyRaceGamePawn::ForwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex)
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
void AMyRaceGamePawn::BackwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//ゲーム時間カウント
void AMyRaceGamePawn::GameTimeCounter()
{
	//サーバのみで実行する。
	if (this->HasAuthority())
	{
		//ゴールしていなければ
		if (!bIsGoal)
		{
			//時間をカウント
			GameTimer += 1.0f;
		}
	}
}

void AMyRaceGamePawn::ItemUseMultiCast()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemMulticast();
	}
}

void AMyRaceGamePawn::ItemUseRunonServer()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemRunonServer();
	}
}


void AMyRaceGamePawn::CrashEvent()
{
	
	const float SpeedMultipication = 0.0f;

	//0を乗算することによってスピードをなくす。
	SpeedCalcFunction(SpeedMultipication);

	//入力値に掛けている値を0にする。（入力しても進ませない為）
	InputCalc = 0.0f;

	//アイテムを使用できないようにする、
	//bIsItemUse = false;
	ItemSettingComponent->SetItemUse(true);


	//一定時間完了後入力値に掛けている値をもとに戻す
	float DelayTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(BrakeTimeHandler, this, &AMyRaceGamePawn::InputReset, DelayTime ,  false);
	

}

void AMyRaceGamePawn::InputReset()
{
	InputCalc = 1.0f;
	//bIsItemUse = true;
	ItemSettingComponent->SetItemUse(true);
}


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


void AMyRaceGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyRaceGamePawn, GameTimer);

}


