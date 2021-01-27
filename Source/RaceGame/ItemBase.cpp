// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "MyRaceGamePawn.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//コンポーネントの初期設定
	ItemColision = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollision"));
	ItemColision->SetupAttachment(RootComponent);
	ItemColision->InitSphereRadius(50.0f);
	ItemColision->SetEnableGravity(false);
	ItemColision->bApplyImpulseOnDamage = false;
	RootComponent = ItemColision;

	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshSet(TEXT("StaticMesh'/Engine/EditorMeshes/EditorHelp.EditorHelp'"));
	UStaticMesh* ItemMeshAsset = MeshSet.Object;
	ItemMesh->SetStaticMesh(ItemMeshAsset);
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RotationMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationMovement"));
	RotationMovement->RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	//オーバーラップイベントを登録
	ItemColision->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::BeginOverlap);

	//変数の初期設定
	bIsItemRespawn = true;

	//SelectItemNumber = 0;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}
//BeginOverlap
void AItemBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//ヒットしたアクターがプレイヤーならキャストを行う
	AMyRaceGamePawn* const MyRaceGamePawn = Cast<AMyRaceGamePawn>(OtherActor);

	//上記の変数設定がうまく行っていれば処理
	if (MyRaceGamePawn)
	{
		//アイテムを非表示
		ItemVisibility(false);

		//アイテムに当たったイベント
		ItemHit(MyRaceGamePawn);

		//一定時間たったらアイテムを再表示
		const float DelayTime = 3.0f;
		GetWorldTimerManager().SetTimer(RespawnHandler, this, &AItemBase::ItemRespawn, DelayTime, false);

	}
}

void AItemBase::ItemVisibility(bool bIsVisible)
{
	//リスポーン可能のものか判定
	if (bIsItemRespawn)
	{
		//引数によってアイテムの表示・非表示を設定
		if (bIsVisible)
		{
			//アイテムを表示
			ItemMesh->SetVisibility(true, true);

			//コリジョンを有効にする。
			ItemColision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		}
		else
		{
			//アイテムの非表示
			ItemMesh->SetVisibility(false, true);
			//コリジョンを無効にする。
			ItemColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		//アイテムの非表示
		ItemMesh->SetVisibility(false, true);
		//コリジョンを無効にする。
		ItemColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AItemBase::ItemRespawn()
{
	ItemVisibility(true);
}

void AItemBase::ItemHit(AMyRaceGamePawn *HitPawn)
{

	UE_LOG(LogTemp, Error, TEXT("ItemHitBase"));
}

