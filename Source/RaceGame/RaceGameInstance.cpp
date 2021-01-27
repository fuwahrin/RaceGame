// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameInstance.h"
#include "Engine/Engine.h"

//Ž©M‚ð•Ô‚·
URaceGameInstance* URaceGameInstance::GetInstance()
{
	URaceGameInstance* instance = nullptr;

	if (GEngine)
	{
		//ViewPoert‚ðŒo—R‚µ‚Äinstance‚ðŽæ“¾
		FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
		instance = Cast<URaceGameInstance>(context->OwningGameInstance);
	}

	return instance;
}
