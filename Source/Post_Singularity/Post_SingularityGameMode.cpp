// Copyright Epic Games, Inc. All Rights Reserved.

#include "Post_SingularityGameMode.h"
#include "Post_SingularityPlayerController.h"
#include "Post_SingularityCharacter.h"
#include "UObject/ConstructorHelpers.h"

APost_SingularityGameMode::APost_SingularityGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APost_SingularityPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}