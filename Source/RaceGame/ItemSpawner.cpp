// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBox.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//コンポーネント設定
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;

	//ダミーメッシュの設定
	DummyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshSet(TEXT("StaticMesh'/Engine/EditorMeshes/EditorHelp.EditorHelp'"));
	UStaticMesh* DummyItemMeshAsset = MeshSet.Object;
	DummyItemMesh->SetStaticMesh(DummyItemMeshAsset);
	DummyItemMesh->SetHiddenInGame(true);
	DummyItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	DummyItemMesh->SetupAttachment(Arrow);
	
	//変数の初期値設定
	ItemSpawnNum = 4;
	ItemMeshSize = 100;
	Offset = 0.0f;

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	//アイテムを並べる座標に矢印を向かせる
	//アイテムを出現させる
	for (int i = 0; i < ItemSpawnNum; i++)
	{
		//アイテム整列位置の設定
		FVector AlignmentLocation;
		float LocationScaler = ItemMeshSize * i + Offset;

		//自身の向きに合わせてアイテムを出現させる
		AlignmentLocation = LocationScaler * GetActorForwardVector();

		//出現させる
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParam;

			//オーナー
			SpawnParam.Owner = this;
			
			//出現位置の設定
			FVector SpawnLocation = GetActorLocation() + AlignmentLocation;
			FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

			//スポーン処理
			AActor* const SpawnItems = World->SpawnActor<AActor>(ItemBoxClass, SpawnLocation , SpawnRotation);
		}
	}
	
}
