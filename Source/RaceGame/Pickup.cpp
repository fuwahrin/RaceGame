// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"


// Sets default values
APickup::APickup()
{
	//　Tick関数内で処理を行わない
 	PrimaryActorTick.bCanEverTick = false;

	//オブジェクトが生成された時点でアクティブにしておく
	bIsActive = true;

	//オブジェクトのメッシュを設定する。
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//staticMeshのコンポーネントをルートに設定
	RootComponent = PickupMesh;

}

//bIsActiveのゲッター
bool APickup::IsActive()
{
	return bIsActive;
}

//bIsActiveのセッター
void APickup::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}
void APickup::WasCollected_Implementation()
{
	FString name = GetName();
	UE_LOG(LogTemp, Log, TEXT("You havr collected %s "), *name);

}
