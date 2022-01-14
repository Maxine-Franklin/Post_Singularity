// Fill out your copyright notice in the Description page of Project Settings.


#include "HexRenderer.h"

/*
:Notes
	Hex Modification per move
	Row
	x = +153

	Collum
	x = +-76.5
	y = -115

:Useful Functions
	FString::FromInt(int32) //Convert int to string
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, <string>);
*/

// Sets default values for this component's properties
UHexRenderer::UHexRenderer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexRenderer::BeginPlay()
{
	Super::BeginPlay();

	//-Determine Grid Size (x,y)
	Owner = GetOwner(); //Get's the actor the script it attached to
	FVector Scale;
	Scale.X = 100 * RefPlane->GetActorScale().X; //Stores the actors size (x axis) [scale multiplied by 100 as default plane size is 100uu]
	Scale.Y = 100 * RefPlane->GetActorScale().Y; //Stores the actors size (y axis) [scale multiplied by 100 as default plane size is 100uu]

	int HexCount[2]; //Used to store y, x size of grid (stored in y, x for loop reasons)
	float* _Scale[2] = { &Scale.Y, &Scale.X }; //Temporary pointer to store actor y, x size
	for (int i = 0; i < 2; i++)
	{
		HexCount[i] = (*_Scale[i] / HexGap[i]); //Calculates how many hex tiles can fit on the actor
		if (fmod(*_Scale[i], HexGap[i]) < (HexGap[i] / 2)) //Checks if any hex tiles would be partially outside of the actor's bounds
		{
			HexCount[i] -= 1; //Decreases hex tile count accordingly
		}
		delete _Scale[i]; //Deletes the scale pointer as it is no longer needed
		_Scale[i] = NULL; //Set's scale pointer to NULL to prevent any crashes in case of double delete
	}

	//-Spawns Hex Tiles;
	HexMaster.Add(TileInstancer(Owner, HexTile)); //Adds a new row of hex tiles to the grid
	Owner->AddInstanceComponent(HexMaster[0].Tile); //Adds the new row to the Actor
	//FTransform HexPos;

	FTransform HexStartPos;
	HexStartPos.SetLocation(FVector(RefPlane->GetActorLocation().X + ((HexCount[1]/2) * HexGap[1]), RefPlane->GetActorLocation().Y - ((HexCount[0]/2) * HexGap[0]), RefPlane->GetActorLocation().Z + 0.1f)); //Obtains and stores the center of the plane
	FTransform* HexPos = &HexStartPos;
	/*for (int y = 0; y < HexCount[0]; y++)
	{
		HexMaster.Add(TileInstancer(Owner, HexTile)); //Adds a new row of hex tiles to the grid
		Owner->AddInstanceComponent(HexMaster[y].Tile); //Adds the new row to the Actor
		for (int x = 0; x < HexCount[1]; x++)
		{
			HexMaster[0].Tile->AddInstance(*HexPos); //Spawns a hex tile
		}
	}*/

	HexMaster.Add(TileInstancer(Owner, HexTile)); //Adds a new row of hex tiles to the grid
	Owner->AddInstanceComponent(HexMaster[0].Tile); //Adds the new row to the Actor

	HexMaster[0].Tile->AddInstance(*HexPos); //Spawns a hex tile

	HexStartPos.SetLocation(FVector(RefPlane->GetActorLocation().X + ((HexCount[1] / 2) * HexGap[1]), RefPlane->GetActorLocation().Y - ((HexCount[0] / 2) * HexGap[0]) + HexGap[0], RefPlane->GetActorLocation().Z + 0.1f)); //Obtains and stores the center of the plane
	HexMaster[0].Tile->AddInstance(HexStartPos);
}


// Called every frame
void UHexRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}