// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSettingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UItemSettingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemSettingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:

	//コンポーネント
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ItemSpawnPoint;

private:
	//----変数
	FVector SpawnPoint;

protected:
	//----変数
	APawn *OwnerPawn;

	//アイテムのID
	UPROPERTY(Replicated ,VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 ItemNumber;

	//アイテムが使用できる状態か確認する変数
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	bool bIsItemUse;

	//スポーンさせる際のScale
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
	float ItemScale;

	//Spanwさせるアイテム
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemClass;


	//アイテムの画像
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		class UTexture2D* DrawIcon;

	//アイテムの種類
	UPROPERTY(/*Replicated ,*/ EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> BulletItem;

	UPROPERTY(/*Replicated ,*/ EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SlipItem;

	UPROPERTY(/*Replicated ,*/ EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> SpeedUpItem;

	//データテーブル
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
		class UDataTable* ItemDataTable;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	class UMyCartMoveComponentReplicator *MoveReplicateComponent;






protected:
	//----関数
	//アイテムを使用したときのメソッド
	UFUNCTION()
		void ItemUse();

	//アイテムをスポーンさせる際の設定
	UFUNCTION()
		void ItemSetting(int32 ItemNum);

	UFUNCTION(Server, reliable)
		void ItemSettingRunOnServer(int32 ItemNum);

	UFUNCTION()
		void SpawnSetting();
	UFUNCTION(Server, reliable)
		void SpawnSettingRunOnServer();

public:

	//アイテムを出現させるメソッド
	UFUNCTION(NetMulticast, reliable)
	void SpawnItemMulticast();

	UFUNCTION(Server, reliable)
	void SpawnItemRunonServer();

	UFUNCTION()
	void SpawnItem();


	//アイテムを取得するときに呼ばれるメソッド
	UFUNCTION()
		void ItemPickup(int ItemNum);
	
	//セッターゲッター
	UFUNCTION()
		void SetItemUse(bool value) { bIsItemUse = value; }

	UFUNCTION()
		FString GetRole(ENetRole role);
	

	
};
