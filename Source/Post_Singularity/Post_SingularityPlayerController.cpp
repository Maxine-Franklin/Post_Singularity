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
	Speed = 3;
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

void APost_SingularityPlayerController::SetPos(int NewPos[2])
{
	Pos[0] = NewPos[0];
	Pos[1] = NewPos[1];
	return;
}

int APost_SingularityPlayerController::GetPos(int val)
{
	return Pos[val];
}

void APost_SingularityPlayerController::BeginPlay()
{
	//Obtain all actors in scene required to render hex grid
	AActor* HexOwner = nullptr; //The master actor that holds all hex tiles
	AActor* RefPlane = nullptr; //The plane that the tiles are being project onto
	AActor* HexTile = nullptr; //The static mesh used for the hex tile's themselves
	FString ActorNames[3] = { "Hex_Tile_MKII_2", "HexMaster", "PlaneMain" }; //The names of each of these actors in the scene
	//NOTE: I know this is not the most efficent solution nor does it open itself to future proofing, but due to time constaints, I have to use this
	for (TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator) //Iterates through every actor in the scene
	{
		if (ActorIterator->GetName() == ActorNames[0]) //If the actor is the hex tile mesh then...
		{
			HexTile = *ActorIterator; //Stores the hex tile
		}
		else if (ActorIterator->GetName() == ActorNames[1]) //If the actor is the hex master actor then...
		{
			HexOwner = *ActorIterator; //Stores the hex master actor
		}
		else if (ActorIterator->GetName() == ActorNames[2]) //If the actor is the reference plane then...
		{
			RefPlane = *ActorIterator; //Stores the reference plane
		}
	}
	//Sets the related variables inside HexSystems
	HexSys.SetHexOwner(HexOwner);
	HexSys.SetHexTile(HexTile);
	HexSys.SetRefPlane(RefPlane);

	//Generates the hex tiles
	HexSys.GenerateNewHexGrid();

	//Obtains player starting position on the grid
	FHitResult Hit; 
	FVector TracePoint = GetPawn()->GetActorLocation(); //Obtains player's starting world position
	GetWorld()->LineTraceSingleByChannel(Hit, TracePoint, TracePoint.DownVector, ECC_Visibility); //Does a linetrace down 1uu from the player's starting point
	Pos[0] = FCString::Atoi(*Hit.GetComponent()->GetName().RightChop(29)); //Obtains the column
	Pos[1] = HexSys.GetTileByXCoord(Hit, Pos[0]); //Obtains the row
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
		if (MouseEvents == true)
		{
			// Trace to see what is under the mouse cursor
			FHitResult Hit;
			GetHitResultUnderCursor(ECC_Visibility, false, Hit);

			if (Hit.bBlockingHit)
			{
				if (Hit.GetActor()->ActorHasTag(TEXT("HexRenderer")) == true)
				{
					MouseEvents = false;
					if (FailedMove[0] != -1)
					{
						HexSys.RevertTileMaterial(FailedMove[0], FailedMove[1]);
						FailedMove[0] = -1;
					}
					//Obtains the column and row that has been clicked on
					int x = FCString::Atoi(*Hit.GetComponent()->GetName().RightChop(29)); //Obtains the column that has been clicked on
					int y = HexSys.GetTileByXCoord(Hit, Pos[0]); //Obtains the row that has been clicked on
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("START: ") + FString::FromInt(Pos[0]) + TEXT(", ") + FString::FromInt(Pos[1]));
					TArray<Coords> CoordLog = HexSys.CalculateMovement(x, y, Speed, Pos[0], Pos[1]);
					if (CoordLog[0].x == -1)
					{
						HexSys.ChangeTileMaterial(3, x, y);
						FailedMove[0] = x;
						FailedMove[1] = y;
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FAILED MOVE"));
					}
					else
					{
						for (int i = 0; i < Speed; i++)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("STEP: ") + FString::FromInt(CoordLog[i].x) + TEXT(", ") + FString::FromInt(CoordLog[i].y));
							FVector NewDest = HexSys.NewDest(CoordLog[i].x, CoordLog[i].y);
							NewDest.Z = GetPawn()->GetActorLocation().Z;
							//HexSys.RevertTileMaterial(CoordLog[i].x, CoordLog[i].y);
							if (i > 0)
							{
								if (CoordLog[i].x != CoordLog[i - 1].x && CoordLog[i].y != CoordLog[i - 1].y)
								{
									break;
								}
							}
							SetNewMoveDestination(NewDest, true);
						}
						Pos[0] = x;
						Pos[1] = y;
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("END: ") + FString::FromInt(x) + TEXT(", ") + FString::FromInt(y));
					}
					_sleep(65);
					MouseEvents = true;
				}
			}
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
		SetNewMoveDestination(HitResult.ImpactPoint, false);
	}
}

void APost_SingularityPlayerController::SetNewMoveDestination(const FVector DestLocation, bool Loop)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		//if ((Distance > 120.0f))
		//{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		//}
	}
	if (Loop == true)
	{
		return;
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
