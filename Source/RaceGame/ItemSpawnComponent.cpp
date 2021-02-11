// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnComponent.h"
#include "ItemRefActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UItemSpawnComponent::UItemSpawnComponent()
{
	//Tickを無効
	PrimaryComponentTick.bCanEverTick = false;

	//変数の初期化
	bIsItemUse = false;

	//データテーブルの初期化
	static ConstructorHelpers::FObjectFinder<UDataTable>TableDataAsset(TEXT("DataTable'/Game/Developers/fuwarrin/Collections/BP/ImportData/RaceGameItemListCPP.RaceGameItemListCPP'"));
	if (TableDataAsset.Succeeded())
	{
		ItemDataTable = TableDataAsset.Object;
	}

}


// Called when the game starts
void UItemSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	//所有者のPawnを取得
	OwnerPawn = GetOwner()->GetInstigator<APawn>();

	//このコンポーネントの同期を有効にする
	SetIsReplicatedByDefault(true);
	
}


// Called every frame
void UItemSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemSpawnComponent::SpawnItemMulticast_Implementation()
{
	SpawnItem();
}


void UItemSpawnComponent::SpawnItemRunonServer_Implementation()
{
	SpawnItem();
}

// アイテムを出現させるメソッド
void UItemSpawnComponent::SpawnItem()
{
	if (OwnerPawn != nullptr)
	{
		//アイテムが使用できる状態か確認
		if (bIsItemUse)
		{
			//アイテムをSpawnさせる
			UWorld* const World = GetWorld();
			if (World)
			{
				//スポーンさせる為のパラメータ
				FActorSpawnParameters SpawnParams;

				//オーナー
				SpawnParams.Owner = this->GetOwner();

				//スポーン位置の設定(ItemSpawnPointのワールド座標を格納)
				//FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
				//FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();
				FVector SpawnLocation = GetComponentLocation();
				FRotator SpawnRotation = GetComponentRotation();

				//スポーンする際のScale
				FVector SpawnScale = FVector(ItemScale);



				//アイテムをスポーンする。
				AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
				SpawnItem->SetActorScale3D(SpawnScale);

				//UE_LOG(LogTemp, Error, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

				ItemUse();

			}
		}
	}
}


//RPC呼び出し検証用のメソッド
//※falseを返すとRPCが実行されない
bool UItemSpawnComponent::SpawnItemRunonServer_Validate()
{
	return true;
}

// アイテムを使用するメソッド
void UItemSpawnComponent::ItemUse()
{
	//アイテムを使用したことを設定

	//UIの画像を消す。
	DrawIcon = nullptr;

	//アイテム使用可能変数をFalse
	bIsItemUse = false;

	//アイテムIDを初期化
	ItemNumber = 0;
}

//アイテムの設定を行うメソッド
void UItemSpawnComponent::ItemSetting(int32 ItemNum)
{
	//アイテムのIDを設定
	ItemNumber = ItemNum;


	//出現する際の設定
	SpawnSetting();

	//アイテムを使用できる状態に変更
	bIsItemUse = true;
}

void UItemSpawnComponent::ItemSettingRunOnServer_Implementation(int32 ItemNum)
{
	ItemSetting(ItemNum);
}


//	アイテムを出現させるメソッド
void UItemSpawnComponent::SpawnSetting()
{
	UE_LOG(LogTemp, Error, TEXT("SpawnSetting"));
	//ネットワークの権限によって処理を変える
	if (GetOwner()->HasAuthority())
	{
		//サーバ側

		//ItemClassをIDによって設定する。
		//-----TOOD:外部ファイルから設定か初期でアセットを読み込むように変更する。
		switch (ItemNumber)
		{
		case 1:
			ItemClass = BulletItem;
			break;
		case 2:
			ItemClass = SlipItem;
			break;
		case 3:
			ItemClass = SpeedUpItem;
			break;
		}

		FName RowName = FName(FString::FromInt(ItemNumber));
		FItemStructCpp* ItemStruct = ItemDataTable->FindRow<FItemStructCpp>(RowName, FString("Error"));

		//UE_LOG(LogTemp, Error, TEXT("ItemHasAu"));

		//データテーブルから構造体に代入出来ていれば各アイテムの処理を行う。
		if (ItemStruct)
		{
			//UE_LOG(LogTemp, Error, TEXT("ItemStructSuccess"));
			//アイテムの出現位置を設定
			FVector ItemSpawnLocation = FVector(ItemStruct->SpawnPosX, 0.0f, 0.0f);
			//ItemSpawnPoint->SetRelativeLocation(ItemSpawnLocation); // エラー原因ここが呼ばれない
			SetRelativeLocation(ItemSpawnLocation);
			//UE_LOG(LogTemp, Error, TEXT("SpawnX = %f"), ItemStruct->SpawnPosX);

			//アイテムの大きさを設定
			ItemScale = ItemStruct->SpawnScale;

			//アイコン設定
			DrawIcon = ItemStruct->Icon;
		}

	}
	else
	{
		//クライアント側
		SpawnSettingRunOnServer();
	}

}


//	アイテム取得メソッド
void UItemSpawnComponent::ItemPickup(int ItemNum)
{
	//サーバ側かクライアント側で処理を分ける
	if (GetOwner()->HasAuthority())
	{
		//サーバー側の処理

		//アイテムの設定を行う。
		ItemSetting(ItemNum);
	}
	else
	{
		//クライアント側の処理

		//アイテム設定をサーバで実行する。
		ItemSettingRunOnServer(ItemNum);
	}
}

void UItemSpawnComponent::SpawnSettingRunOnServer_Implementation()
{
	SpawnSetting();
}

void UItemSpawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemSpawnComponent, ItemNumber);
	DOREPLIFETIME(UItemSpawnComponent, ItemClass);
	DOREPLIFETIME(UItemSpawnComponent, DrawIcon);
	DOREPLIFETIME(UItemSpawnComponent, ItemScale);
	DOREPLIFETIME(UItemSpawnComponent, bIsItemUse);
	DOREPLIFETIME(UItemSpawnComponent, BulletItem);
	DOREPLIFETIME(UItemSpawnComponent, SlipItem);
	DOREPLIFETIME(UItemSpawnComponent, SpeedUpItem);

}
