// Copyright Epic Games, Inc. All Rights Reserved.

#include "Post_SingularityPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Post_SingularityCharacter.h"
#include "Engine/World.h"

APost_SingularityPlayerController::APost_SingularityPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableMouseOverEvents = true;
	Speed = 6;
}

void APost_SingularityPlayerController::SetSpeed(int val)
{
	Speed = val;
	return;
}

int APost_SingularityPlayerController::GetSpeed()
{
	return Speed;
}

void APost_SingularityPlayerController::BeginPlay()
{
	//Obtain all actors in scene required to render hex grid
	AActor* HexOwner = nullptr; //The master actor that holds all hex tiles
	AActor* RefPlane = nullptr; //The plane that the tiles are being project onto
	AActor* HexTile = nullptr; //The static mesh used for the hex tile's themselves
	FString ActorNames[3] = {"Hex_Tile_MKII_2", "HexMaster", "PlaneMain"}; //The names of each of these actors in the scene
	//NOTE: I know this is not the most efficent solution nor does it open itself to future proofing, but due to time constaints, I have to use this
	for (TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator) //Iterates through every actor in the scene
	{
		if (ActorIterator->GetName() == ActorNames[0]) //If the actor is the hex tile mesh then...
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("wut?"));
			HexTile = *ActorIterator; //Stores the hex tile
		}
		else if (ActorIterator->GetName() == ActorNames[1]) //If the actor is the hex master actor then...
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("The?"));
			HexOwner = *ActorIterator; //Stores the hex master actor
		}
		else if (ActorIterator->GetName() == ActorNames[2]) //If the actor is the reference plane then...
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("golf?"));
			RefPlane = *ActorIterator; //Stores the reference plane
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HexTile->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HexOwner->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, RefPlane->GetName());
	//HexSys.SetRendOwner(FindObject);
	//HexSys.GenerateNewHexGrid();
}

void APost_SingularityPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void APost_SingularityPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &APost_SingularityPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &APost_SingularityPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &APost_SingularityPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &APost_SingularityPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &APost_SingularityPlayerController::OnResetVR);
}

void APost_SingularityPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void APost_SingularityPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (APost_SingularityCharacter* MyPawn = Cast<APost_SingularityCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			if (Hit.GetActor()->ActorHasTag(TEXT("HexRenderer")) == true)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (Hit.GetActor()->GetName()));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (Hit.GetComponent()->GetName()));
				//AActor* HexRend = Hit.GetActor();
				//HexRend.Component
				//UObject* HexRend = Hit.GetActor()->GetDefaultSubobjectByName(TEXT("HexRenderer"));
				//HexRend->CallFunction(Hit.GetActor(), &UHexRenderer::GetTileByYCoord(Hit))
				//UHexRenderer GetTileByYCoord(Hit);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (Hit.GetComponent()->GetComponentInstanceData()->));
			}
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void APost_SingularityPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void APost_SingularityPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APost_SingularityPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void APost_SingularityPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}
