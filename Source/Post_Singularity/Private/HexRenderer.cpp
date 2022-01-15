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

	//CustomMeshComponent = CreateDefaultSubobject(TEXT("Test Component"));
	//CustomMeshComponent = CreateDefaultSubobject(TEXT("Test Component"));
	//CustomMeshComponent->AttachTo(RootComponent);
	//CustomMeshComponent->OnBeginCursorOver.__Internal_AddDynamic(RefPlane, &HexMaster::CustomOnBeginMouseOver());
	//CustomMeshComponent->OnClicked.Add(&HexMaster::CustomOnBeginMouseOver("Tstr"));
	// ...
}


void UHexRenderer::Tester()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Mouse Over"));
	return;
}

// Called when the game starts
void UHexRenderer::BeginPlay()
{
	Super::BeginPlay();

	//-Determine Grid Size (x,y)
	Owner = GetOwner(); //Get's the actor the script is attached to
	//RefPlane->OnBeginCursorOver.Add(Post_SingularityPlayerControler::OnCursorOver());

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

	FTransform HexStartPos;
	HexStartPos.SetLocation(FVector(RefPlane->GetActorLocation().X + ((HexCount[1]/2) * HexGap[1]), RefPlane->GetActorLocation().Y - ((HexCount[0]/2) * HexGap[0]), RefPlane->GetActorLocation().Z + 0.1f)); //Obtains and stores the center of the plane
	//int* yHexCount = &HexCount[0]; //Uses a pointer for HexCount[0] as the value requires changing in the generating process
	for (int x = 0; x <= HexCount[1]; x++) //For each column
	{
		HexMaster.Add(TileInstancer(Owner, HexTile)); //Adds a new row of hex tiles to the grid
		Owner->AddInstanceComponent(HexMaster[x].Tile); //Adds the new row to the Actor
		FVector HexPos = HexStartPos.GetLocation(); //Stores the coordinates of the first hex tile
		HexPos.X -= (x * HexGap[1]); //Set's HexPos' X coordinate to the current row's position
		if (x % 2 == 1) //if it is an odd numbered column then...
		{
			HexPos.Y += HexGap[2]; //Adjusts the row's begining position to prevent overlap with previous column
			//*yHexCount -= 1; //Decreases the amount of tiles on the next row by one - add system after adding collision tiles
		}
		//else { *yHexCount += 1; } else... increases amount of tiles on the next row by one - add system after adding collision tiles
		for (int y = 0; y < HexCount[0]; y++) //For each tile of the current row
		{
			//HexMaster[x].Tile->Name = (x & y);
			//HexMaster[x].Tile->Rename(new TCHAR(x & y));
			HexMaster[x].Tile->AddInstance(FTransform(HexPos)); //Spawns a hex tile
			//HexMaster[x].Tile[y].OnBeginCursorOver;
			//HexMaster[x].Tile[y].OnBeginCursorOver.Add()
			HexPos.Y += HexGap[0]; //Increments the next spawning position by one unit
		}
		//HexMaster[x].Tile->SetCustomData(0, { 1.0f });
	}

	//if (HexMaster[0].Tile[0].Bounds  (HexStartPos.GetLocation()))
	//{
	//
	//}
}

// Called every frame
void UHexRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}