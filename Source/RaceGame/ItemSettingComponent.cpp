// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSettingComponent.h"
#include "Net/UnrealNetwork.h"
#include "ItemRefActor.h"
#include "MyCartMoveComponentReplicator.h"

// Sets default values for this component's properties
UItemSettingComponent::UItemSettingComponent()
{
	//Tickを無効
	PrimaryComponentTick.bCanEverTick = false;


	//変数の初期化
	bIsItemUse = false;

	//このコンポーネントの同期を有効にする
	SetIsReplicatedByDefault(true);


	//データテーブルの初期化
	static ConstructorHelpers::FObjectFinder<UDataTable>TableDataAsset(TEXT("DataTable'/Game/Developers/fuwarrin/Collections/BP/ImportData/RaceGameItemListCPP.RaceGameItemListCPP'"));
	if (TableDataAsset.Succeeded())
	{
		ItemDataTable = TableDataAsset.Object;
	}

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnPoint"));
	
}


// Called when the game starts
void UItemSettingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//所有者のPawnを取得
	OwnerPawn = GetOwner()->GetInstigator<APawn>();

	MoveReplicateComponent = GetOwner()->FindComponentByClass<UMyCartMoveComponentReplicator>();


	
	
}


// Called every frame
void UItemSettingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UItemSettingComponent::SpawnItemMulticast_Implementation()
{
	SpawnItem();
	UE_LOG(LogTemp, Warning, TEXT("Server: %s") ,*GetRole(GetOwnerRole()));
	
}


void UItemSettingComponent::SpawnItemRunonServer_Implementation()
{
	SpawnItem();
	UE_LOG(LogTemp, Warning, TEXT("Cliant: %s"), *GetRole(GetOwner()->GetRemoteRole()));

}

// アイテムを出現させるメソッド
void UItemSettingComponent::SpawnItem()
{
	if (OwnerPawn != nullptr && MoveReplicateComponent != nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("bIsItemUse = %d"), bIsItemUse);
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
				FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
				FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();


				
				//スポーンする際のScale
				FVector SpawnScale = FVector(ItemScale);

				//AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
				//SpawnItem->SetActorScale3D(SpawnScale);

				//サーバ側の操作者のみ
				if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					//UE_LOG(LogTemp, Warning, TEXT("Server"));
					
				}
				
				//クライアント側の操作者のみ
				if (GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					//UE_LOG(LogTemp, Warning, TEXT("Cliant"));
				}

				//UE_LOG(LogTemp, Error, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
				//UE_LOG(LogTemp, Warning, TEXT("ItemSpawnPoint%f%f%f"), ItemSpawnPoint->GetRelativeLocation().X, ItemSpawnPoint->GetRelativeLocation().Y, ItemSpawnPoint->GetRelativeLocation().Z);

				ItemUse();

			}
		}
	}
}


// アイテムを使用するメソッド
void UItemSettingComponent::ItemUse()
{
	//アイテムを使用したことを設定

	//UIの画像を消す。
	DrawIcon = nullptr;

	//アイテム使用可能変数をFalse
	bIsItemUse = false;

	//アイテムIDを初期化
	ItemNumber = 0;
}



//	アイテムを出現させるメソッド
void UItemSettingComponent::SpawnSetting()
{
	
	float LastMoveAdjuster = 0.0f;
	//ネットワークの権限によって処理を変える
	if (GetOwner()->HasAuthority())
	{
		//UE_LOG(LogTemp, Error, TEXT("%s:SpawnSetting:ItemNumber = %d"),GetOwner()->GetRemoteRole(), ItemNumber);

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

		
		//データテーブルから構造体に代入出来ていれば各アイテムの処理を行う。
		if (ItemStruct)
		{
			//アイテムの出現位置を設定
			FVector ItemSpawnLocation = FVector(ItemStruct->SpawnPosX, 0.0f, 0.0f);
			if (ItemSpawnPoint->GetAttachParent() != nullptr)
			{
				ItemSpawnPoint->SetRelativeLocation(ItemSpawnLocation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ItemSpawnNotParent"));
			}
			//アイテムの大きさを設定
			ItemScale = ItemStruct->SpawnScale;

			//アイコン設定
			DrawIcon = ItemStruct->Icon;
			//UE_LOG(LogTemp, Error, TEXT("Icon = %s") , *ItemStruct->Icon->GetName());

		}
	}
	else
	{
		//クライアント側
		SpawnSettingRunOnServer();
	}
}


void UItemSettingComponent::SpawnSettingRunOnServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Cliant:SpawnSetting"));
	SpawnSetting();
}



//	アイテム取得メソッド
void UItemSettingComponent::ItemPickup(int ItemNum)
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
		UE_LOG(LogTemp, Warning, TEXT("PickUp"));
		//アイテム設定をサーバで実行する。
		ItemSettingRunOnServer(ItemNum);
	}

	//ItemSetting(ItemNum);
}

FString UItemSettingComponent::GetRole(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "ROLE_MAX";
	default:
		return "ERROR";
	}
	
}

//アイテムの設定を行うメソッド
void UItemSettingComponent::ItemSetting(int32 ItemNum)
{
	//アイテムのIDを設定
	ItemNumber = ItemNum;

	//UE_LOG(LogTemp, Warning, TEXT("ItemSetting"));

	//出現する際の設定
	SpawnSetting();

	//アイテムを使用できる状態に変更
	bIsItemUse = true;

	//UE_LOG(LogTemp, Warning, TEXT("bIsItemUse = %d") , bIsItemUse);


}

void UItemSettingComponent::ItemSettingRunOnServer_Implementation(int32 ItemNum)
{
	ItemSetting(ItemNum);
	UE_LOG(LogTemp, Warning, TEXT("ItemSettingRunOnServer"));

}


void UItemSettingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemSettingComponent, ItemNumber);
	DOREPLIFETIME(UItemSettingComponent, ItemClass);
	DOREPLIFETIME(UItemSettingComponent, DrawIcon);
	DOREPLIFETIME(UItemSettingComponent, ItemScale);
	DOREPLIFETIME(UItemSettingComponent, bIsItemUse);


	//DOREPLIFETIME(UItemSettingComponent, BulletItem);
	//DOREPLIFETIME(UItemSettingComponent, SlipItem);
	//DOREPLIFETIME(UItemSettingComponent, SpeedUpItem);

}

