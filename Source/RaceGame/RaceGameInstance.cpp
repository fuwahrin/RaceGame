// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameInstance.h"
#include "Engine/Engine.h"

//自信を返す
URaceGameInstance* URaceGameInstance::GetInstance()
{
	URaceGameInstance* instance = nullptr;

	if (GEngine)
	{
		//ViewPoertを経由してinstanceを取得
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<URaceGameInstance>(context->OwningGameInstance);
	}

	return instance;
}
