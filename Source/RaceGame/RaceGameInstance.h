// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RaceGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RACEGAME_API URaceGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//プレイヤーの数を数える変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Instance")
	int32 PlayerCounter;

	static URaceGameInstance* GetInstance();
	

};
