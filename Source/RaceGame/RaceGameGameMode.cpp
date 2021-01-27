// Copyright Epic Games, Inc. All Rights Reserved.

#include "RaceGameGameMode.h"
#include "RaceGamePawn.h"
#include "RaceGameHud.h"

ARaceGameGameMode::ARaceGameGameMode()
{
	DefaultPawnClass = ARaceGamePawn::StaticClass();
	HUDClass = ARaceGameHud::StaticClass();
}
