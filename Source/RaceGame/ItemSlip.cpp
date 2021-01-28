// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlip.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"

//コンストラクタ
AItemSlip::AItemSlip()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	//メッシュの設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SlipItemMeshAsset(TEXT("StaticMesh'/Game/VehicleAdv/ArenaMesh/pole.pole'"));
	UStaticMesh* SlipItemMesh = SlipItemMeshAsset.Object;
	ItemMesh->SetStaticMesh(SlipItemMesh);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0, 25.0f));
	

	//同期設定
	SetReplicates(true);

	//リスポーン防止
	bIsItemRespawn = false;

}
void AItemSlip::BeginPlay()
{
	Super::BeginPlay();
}
//アイテムにヒットしたときのメソッド
void AItemSlip::ItemHit(AMyRaceGamePawn* HitPawn)
{
	//プレイヤーをクラッシュさせる
	HitPawn->CrashEvent();

}

//アイテムにヒットしたときのメソッド
void AItemSlip::ItemHit(AMyPawn* HitPawn)
{
	//プレイヤーをクラッシュさせる
	HitPawn->FindComponentByClass<UMyCartMoveComponent>()->CrashEvent();

}
