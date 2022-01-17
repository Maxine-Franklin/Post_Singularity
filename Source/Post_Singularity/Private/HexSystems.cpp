// Fill out your copyright notice in the Description page of Project Settings.

#include "HexSystems.h"
#include "..\Public\HexSystems.h"

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

HexSystems::HexSystems()
{
}

HexSystems::~HexSystems()
{
}

AActor* HexSystems::GetHexOwner()
{
	return Owner;
}

void HexSystems::SetHexOwner(AActor* NewOwner)
{
	Owner = NewOwner;
	return;
}

UStaticMesh* HexSystems::GetHexTile()
{
	return HexTile;
}

void HexSystems::SetHexTile(AActor* NewHexTile)
{
	//Uses the component list of the Actor HexTile to obtain it's mesh
	TArray<UStaticMeshComponent*> Components;
	NewHexTile->GetComponents<UStaticMeshComponent>(Components);

	HexTile = Components[0]->GetStaticMesh();
	return;
}

AActor* HexSystems::GetRefPlane()
{
	return RefPlane;
}

void HexSystems::SetRefPlane(AActor* NewRefPlane)
{
	RefPlane = NewRefPlane;
	return;
}

void HexSystems::GenerateNewHexGrid()
{
	//-Determine Grid Size (x,y)
	FVector Scale;
	Scale.X = 100 * RefPlane->GetActorScale().X; //Stores the actors size (x axis) [scale multiplied by 100 as default plane size is 100uu]
	Scale.Y = 100 * RefPlane->GetActorScale().Y; //Stores the actors size (y axis)

	int HexCount[2]; //Used to store y, x size of grid (stored in y, x for loop reasons)
	float* refScale[2] = { &Scale.Y, &Scale.X }; //Temporary pointer to store actor's y, x size
	for (int i = 0; i < 2; i++)
	{
		HexCount[i] = (*refScale[i] / HexGap[i]); //Calculates how many hex tiles can fit on the actor
		if (fmod(*refScale[i], HexGap[i]) < (HexGap[i] / 2)) //If hex tiles spawned would be greater than the size of the actor then..
		{
			HexCount[i] -= 1; //Decreases hex tile count by one
		}
		//Clean Up
		delete refScale[i];
		refScale[i] = NULL;
	}

	//-Spawns Hex Tiles;
	HexStartPos.SetLocation(FVector(RefPlane->GetActorLocation().X + ((HexCount[1] / 2) * HexGap[1]), RefPlane->GetActorLocation().Y - ((HexCount[0] / 2) * HexGap[0]), RefPlane->GetActorLocation().Z + 0.1f)); //Obtains and stores the center of the plane
	//int* yHexCount = &HexCount[0]; //Uses a pointer for HexCount[0] as the value requires changing in the generating process - Adding in a future system
	for (int x = 0; x <= HexCount[1]; x++) //For each column
	{
		HexMaster.Add(TileInstancer(Owner, HexTile)); //Adds a new row of hex tiles to the array
		Owner->AddInstanceComponent(HexMaster[x].Tile); //Adds the new row to the Actor
		FVector HexPos = HexStartPos.GetLocation(); //Stores the coordinates of the first hex tile
		HexPos.X -= (x * HexGap[1]); //Set's HexPos' X coordinate to the current row's position
		if (x % 2 == 1) //if it is an odd numbered column then...
		{
			HexPos.Y += HexGap[2]; //Adjusts the row's begining position to prevent overlap with previous column
			//*yHexCount -= 1; //Decreases the amount of tiles on the next row by one - Adding in a future system
		}
		//else { *yHexCount += 1; } else... increases amount of tiles on the next row by one - Adding in a future system
		for (int y = 0; y < HexCount[0]; y++) //For each tile on the current row
		{
			HexMaster[x].Tile->AddInstance(FTransform(HexPos)); //Spawns a hex tile
			HexPos.Y += HexGap[0]; //Increments the next spawning position by one unit
		}
	}
	HexMaster.Add(TileInstancer(Owner, HexTile)); //Empty row that will hold temporary instances later
	Owner->AddInstanceComponent(HexMaster[HexCount[1]+1].Tile); //Adds the new row to the Actor
	return;
}

int HexSystems::GetTileByXCoord(FHitResult Hit, int x)
{
	//Obtains y starting coordinate
	FVector StartPos = HexStartPos.GetLocation();
	float yBound = StartPos.Y;
	if (x % 2 == 1) //If on an odd numbered column then...
	{
		yBound += HexGap[2]; //Increments start pos to account for difference between odd and even rows
	}

	//Obtains y coord
	int y = -1; //Starts y at negative 1 as it will become 0 on the first iteration through the loop
	do
	{
		y++; //Increments the value of y by 1
		yBound += HexGap[0]; //Increments y
	} while ((yBound - HexGap[0]/2) < Hit.Location.Y); //While the left most part of the current hex tile is less than the y position of the mouse...

	return y;
}

TArray<Coords> HexSystems::CalculateMovement(int xTarget, int yTarget, int Speed, int xCPos, int yCPos)
{
	TArray<Coords> CoordLog;
	int yChange;
	int StepsTaken = 0;
	do
	{
		yChange = Change(yCPos, yTarget);
		if (yCPos != yTarget && xCPos != xTarget)
		{
			float xCP = xCPos;
			xCP /= 2;
			if (fmod(xCP, 1) == 0)
			{
				yChange -= 1;
			}
			else
			{
				yChange += 1;
			}
		}
		if (yChange > 1 || yChange < -1)
		{
			yChange = floor(yChange / 2);
		}
		yCPos += yChange;
		xCPos += Change(xCPos, xTarget);
		CoordLog.Add(Coords());
		CoordLog[StepsTaken].x = xCPos;
		CoordLog[StepsTaken].y = yCPos;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("STEP: ") + FString::FromInt(CoordLog[StepsTaken].x) + TEXT(", ") + FString::FromInt(CoordLog[StepsTaken].y));

		StepsTaken++;
	} while (/*(xCPos != xTarget && yCPos != yTarget) || */StepsTaken != Speed);
	if (xCPos != xTarget || yCPos != yTarget)
	{
		CoordLog[0].x = -1;
	}
	else
	{
		for (int i = 0; i < Speed; i++)
		{
			if (i > 0)
			{
				if (CoordLog[i].x != CoordLog[i - 1].x && CoordLog[i].y != CoordLog[i - 1].y)
				{
					break;
				}
			}
			//ChangeTileMaterial(2, CoordLog[i].x, CoordLog[i].y);
		}
	}
	return CoordLog;
}

FVector HexSystems::NewDest(int x, int y)
{
	FVector HexPos = HexStartPos.GetLocation(); //Logs Hex Starting Position
	HexPos.X -= x * HexGap[1];
	HexPos.Y += y * HexGap[0];
	if (x % 2 == 1) { HexPos.Y += HexGap[2]; } //If hex tile is on an odd numbered row then... Adjust accordingly
	return HexPos;
}

FTransform HexSystems::ChangeSetUp(int Direction, int x, int y)
{
	FVector StartPos = HexStartPos.GetLocation(); //Logs Hex Starting Position
	FVector HexPos = FVector(StartPos.X - (x * HexGap[1]), StartPos.Y + (y * HexGap[0]), Direction); //Logs new position under the map in a vector
	if (x % 2 == 1) { HexPos.Y += HexGap[2]; } //If hex tile is on an odd numbered row then... Adjust accordingly
	FTransform NewTrans = HexStartPos; //Logs hex starting scale and rotation
	NewTrans.SetLocation(HexPos);
	return NewTrans;
}

void HexSystems::ChangeTileMaterial(int index, int x, int y)
{
	//Moves current hex tile to underneath the map
	HexMaster[x].Tile->UpdateInstanceTransform(y, ChangeSetUp(-1, x, y), true, true, true); //Moves the indicated hex tile underneath the map

	//Adds new temporary hex tile with chosen material
	HexMaster[HexMaster.Num() - 1].Tile->AddInstance(ChangeSetUp(1, x, y)); //Adds the hex tile in place of the previous one
	HexMaster[HexMaster.Num() - 1].Tile->SetMaterial(0, HexTile->GetMaterial(index)); //Changes new hex tile material to be the chosen material
	return;
}

void HexSystems::RevertTileMaterial(int x, int y)
{
	//if (HexMaster[HexMaster.Num() - 1].Tile->Instance
	HexMaster[HexMaster.Num() - 1].Tile->ClearInstances();

	HexMaster[x].Tile->UpdateInstanceTransform(y, ChangeSetUp(1, x, y), true, true, true);
	return;
}

int HexSystems::Change(int CPos, int TPos)
{
	if (CPos < TPos) { return 1; }
	else if (CPos > TPos) { return -1; }
	else { return 0; }
	return 0;
}
