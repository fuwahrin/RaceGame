// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "MyRaceGamePawn.h"
#include "GameFramework/RotatingMovementComponent.h"
//コンストラクタ
AItemBox::AItemBox()
{
	//回転の設定
	RotationMovement->RotationRate = FRotator(0.0f, 0.0f, 30.0f);

}

void AItemBox::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBox::ItemHit(AMyRaceGamePawn* HitPawn)
{
	//アイテムのIDを乱数から付与する
	const int32 randMin = 1;
	const int32 randMax = 3;
	SelectItemNumber = FMath::RandRange(randMin, randMax);

	HitPawn->ItemPickup(SelectItemNumber);

}
