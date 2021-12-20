// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "GuardianOfTowersGameMode.h"
#include "GuardianOfTowersHUD.h"
#include "GuardianOfTowersCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGuardianOfTowersGameMode::AGuardianOfTowersGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGuardianOfTowersHUD::StaticClass();
}
