// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TrainingRange.h"
#include "TrainingRangeGameMode.h"
#include "TrainingRangeHUD.h"
#include "TrainingRangeCharacter.h"

ATrainingRangeGameMode::ATrainingRangeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATrainingRangeHUD::StaticClass();
}
