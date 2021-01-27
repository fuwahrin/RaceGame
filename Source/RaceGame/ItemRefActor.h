// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/DataTable.h"

#include "ItemRefActor.generated.h"


//�N���X�̑O���錾
//class UTexture2D;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Attack,
	Slip,
	SppedUp,

	ItemTypeCount
};

//�O���ǂݍ��݂Ŏg�p����\����
USTRUCT(BlueprintType)
struct FItemStructCpp : public FTableRowBase
{

	GENERATED_USTRUCT_BODY()
public:
	//�A�C�e��ID
	UPROPERTY(EditAnyWhere, BlueprintReadonly)
		int32 ID;

	UPROPERTY(EditAnyWhere, BlueprintReadonly)
		float SpawnPosX;

	UPROPERTY(EditAnyWhere, BlueprintReadonly)
		float SpawnScale;

	UPROPERTY(EditAnyWhere, BlueprintReadonly)
		EItemType Type;

	UPROPERTY(EditAnyWhere, BlueprintReadonly)
		class UTexture2D* Icon;



};

UCLASS()
class RACEGAME_API AItemRefActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemRefActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
