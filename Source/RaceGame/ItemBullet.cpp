// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MyCartMoveComponent.h"



AItemBullet::AItemBullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//同期設定
	SetReplicates(true);

	//メッシュ
	static ConstructorHelpers::FObjectFinder<UStaticMesh>BulletMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	UStaticMesh* BulletMesh = BulletMeshAsset.Object;
	ItemMesh->SetStaticMesh(BulletMesh);		
	
	//物理設定
	ItemColision->SetEnableGravity(true);
	ItemColision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	//コンポーネントの設定
	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletMovement"));
	BulletMovement->InitialSpeed = 3000.0f;
	BulletMovement->MaxSpeed = 10000.0f;
	BulletMovement->bShouldBounce = true;
	BulletMovement->Bounciness = 1.0f;
	BulletMovement->Friction = 0.0f;
	BulletMovement->BounceVelocityStopSimulatingThreshold = 0.0f;

	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0 ,0.0f));

	//変数の設定
	bIsItemRespawn = false;

}

void AItemBullet::BeginPlay()
{
	Super::BeginPlay();

	//床抜けようにコリジョンを設定
	ItemColision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

}

//アイテムが当たった時のイベント
void AItemBullet::ItemHit(AMyRaceGamePawn *HitPawn)
{
	//プレイヤーをクラッシュさせる
	HitPawn->CrashEvent();
}

//アイテムが当たった時のイベント
void AItemBullet::ItemHit(AMyPawn* HitPawn)
{
	//プレイヤーをクラッシュさせる
	HitPawn->FindComponentByClass<UMyCartMoveComponent>()->CrashEvent();
}
