// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpeedUp.h"
#include "MyRaceGamePawn.h"
#include "MyPawn.h"
#include "MyCartMoveComponent.h"


//コンストラクタ
AItemSpeedUp::AItemSpeedUp()
{
	PrimaryActorTick.bCanEverTick = true;

	//リスポーンさせなうように設定
	bIsItemRespawn = false;

	//メッシュの設定
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SpeedUpItemMeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	UStaticMesh* SpeedUpMesh = SpeedUpItemMeshAsset.Object;
	ItemMesh->SetStaticMesh(SpeedUpMesh);
	ItemMesh->SetRelativeLocation(FVector(0.0f, 0.0, 0.0f));

	//同期設定
	SetReplicates(true);


}
void AItemSpeedUp::BeginPlay()
{
	Super::BeginPlay();
}

void AItemSpeedUp::ItemHit(AMyRaceGamePawn* HitPawn)
{
	HitPawn->SpeedCalcFunction(2000.0f);
}

void AItemSpeedUp::ItemHit(AMyPawn* HitPawn)
{
	
	UE_LOG(LogTemp, Warning, TEXT("SpeedUp"));
	HitPawn->FindComponentByClass<UMyCartMoveComponent>()->SpeedUpEvent();
}
