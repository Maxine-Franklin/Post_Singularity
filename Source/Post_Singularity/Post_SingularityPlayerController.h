// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexRenderer.h"
#include "HexSystems.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "Post_SingularityPlayerController.generated.h"

UCLASS()
class APost_SingularityPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APost_SingularityPlayerController();

	void SetSpeed(int val);
	int GetSpeed();

	void SetPos(int NewPos[2]);
	int GetPos(int val);

protected:
	int Speed;
	int Pos[2]; //Position on the hex grid (x, y)
	int FailedMove[2] = {-1, -1}; //Logs position of failed move (x, y)
	bool MouseEvents = true; //Needed to disable the player mouse
	//UHexRenderer HexRend;

	//UPROPERTY(EditAnywhere, Category = "Hex Grid") //Makes the below variable editable in the editor
	//AActor* HexSpawner;

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation, bool Loop);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

private:
	//HexRenderer HexRend;
	HexSystems HexSys;
};


