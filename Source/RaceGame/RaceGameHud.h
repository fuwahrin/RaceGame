// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RaceGameHud.generated.h"

UCLASS(config = Game)
class ARaceGameHud : public AHUD
{
	GENERATED_BODY()

public:
	ARaceGameHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
