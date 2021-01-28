// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class AMyRaceGamePawn;
class AMyPawn;

//アイテム取得時に呼ばれる動的デリゲート
DECLARE_DYNAMIC_DELEGATE(FItemPickupDispather);


UCLASS()
class RACEGAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	//------------------コンポーネント
	
	//あたり判定のコリジョン
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		class USphereComponent  *ItemColision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent *ItemMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = DisPlay, meta = (AllowPrivateAccess = "true"))
		class URotatingMovementComponent *RotationMovement;

protected:
	//------------------メソッド
	
	//BeginOverlap
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
						  AActor* OtherActor,
						  UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex,
						  bool bFromSweep,
						  const FHitResult&SweepResult);

	//アイテムの表示・非表示を設定しているメソッド
	UFUNCTION()
		void ItemVisibility(bool bIsVisible);

	UFUNCTION()
		void ItemRespawn();

	//アイテムが当たった時に発生するイベント
	virtual void ItemHit(AMyRaceGamePawn *HitPawn);
	virtual void ItemHit(AMyPawn* HitPawn);

	

protected:
	//------------------変数
	

	//アイテムのリスポーンが可能かどうか
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemSetting", meta = (AllowPrivateAccess = "true"))
		bool bIsItemRespawn;

	//付与するアイテムの番号
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ItemSetting", meta = (AllowPrivateAccess = "true"))
		int32 SelectItemNumber;

	//アイテム再出現のタイムハンドラ
	FTimerHandle RespawnHandler;

public:
	//----イベントディスパッチャー呼び出し用（呼び出し元：MyRaceGamePawn ItemPickup）
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	FItemPickupDispather ItemPickupDispather;


};
