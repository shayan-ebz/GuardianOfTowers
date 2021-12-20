// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GuardianOfTowersHUD.generated.h"

UCLASS()
class AGuardianOfTowersHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGuardianOfTowersHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

