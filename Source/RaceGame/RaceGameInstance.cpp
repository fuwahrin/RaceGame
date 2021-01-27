// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameInstance.h"
#include "Engine/Engine.h"

//���M��Ԃ�
URaceGameInstance* URaceGameInstance::GetInstance()
{
	URaceGameInstance* instance = nullptr;

	if (GEngine)
	{
		//ViewPoert���o�R����instance���擾
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<URaceGameInstance>(context->OwningGameInstance);
	}

	return instance;
}
