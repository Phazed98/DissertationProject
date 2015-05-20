// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ModelBuilder.h"
#include "ModelBuilderGameMode.h"
#include "ModelBuilderHUD.h"
#include "ModelBuilderCharacter.h"

AModelBuilderGameMode::AModelBuilderGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AModelBuilderHUD::StaticClass();
}
