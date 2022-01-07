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
	Scale.X = 100 * Owner->GetActorScale().X; //Stores the actors size (x axis) [scale multiplied by 100 as default plane size is 100uu]
	Scale.Y = 100 * Owner->GetActorScale().Y; //Stores the actors size (y axis) [scale multiplied by 100 as default plane size is 100uu]

	int HexCount[2]; //Used to store y, x size of grid (stored in y, x for loop reasons)
	float * _Scale[2] = { &Scale.Y, &Scale.X }; //Temporary pointer to store actor y, x size
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
	
	//vector<vector<int>> tstr;
	//tstr[0] = vector<int>(1);
	//tstr[0][0] = 2;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(Hex[0][0]));
	/*
	USTRUCT()
		struct FEncapsule
	{
		TArray<int> Item;
	};

	TArray<FEncapsule> MyArray;

	MyArray.Add(FEncapsule());
	MyArray[0].Item.Add(12);
	MyArray[0].Item.Add(15);

	TArray<int> tstr;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(MyArray[0].Item[0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(MyArray[0].Item[1]));
	*/

	UInstancedStaticMeshComponent * HexSpawn = NewObject<UInstancedStaticMeshComponent>(Owner);
	HexSpawn->RegisterComponent();
	HexSpawn->SetStaticMesh(HexTile);
	//HexSpawn->SetMaterial(0, HexTile->GetMaterial(0));
	Owner->AddInstanceComponent(HexSpawn);
	FTransform HexPos;
	FVector OwnerLocation = Owner->GetActorLocation();
	OwnerLocation.Z += 1;
	HexPos.SetLocation(OwnerLocation);
	HexSpawn->AddInstance(HexPos); //-Not sure if this will work as the script is an actor component
	/*TArray<int> tstr;
	int tstr2[2];
	tstr.Add(tstr2[])
	tstr[0].Push(2);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(tstr[0][0]));*/
}


// Called every frame
void UHexRenderer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

