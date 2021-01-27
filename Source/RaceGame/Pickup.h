// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

private:
	//レベル上で拾われるメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
public:

	//拾われるメッシュを返す
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return PickupMesh; };

	//このアイテムを拾えるかどうかの確認用
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	//アイテムが拾えるかどうかを設定する
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	//アイテム回収時に呼び出される
	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
	void WasCollected();
	virtual void WasCollected_Implementation();


protected:

	//このアイテムが拾えるかどうか
	UPROPERTY()
	bool bIsActive;
	
};
