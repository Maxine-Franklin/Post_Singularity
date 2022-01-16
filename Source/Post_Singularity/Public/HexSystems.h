// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include "Engine/Engine.h"
#include "TileInstancer.h"
#include "CoreMinimal.h"

/**
 * 
 */
class POST_SINGULARITY_API HexSystems
{
public:
	// Sets default values for this component's properties
	TArray<TileInstancer> HexMaster;

	HexSystems();
	~HexSystems();
	AActor* GetHexOwner();
	void SetHexOwner(AActor* NewOwner);
	UStaticMesh* GetHexTile();
	void SetHexTile(AActor* NewHexTile);
	AActor* GetRefPlane();
	void SetRefPlane(AActor* NewRefPlane);


	void GenerateNewHexGrid();
	void GetTileByYCoord(FHitResult Hit);

private:
	AActor* Owner;
	float HexGap[3] = { 153.0f, 115.0f, 76.5f }; //Distance between Hex tiles HexGap[0] for Row, HexGap[1] for collum x, HexGap[2] for collum y
	UPROPERTY(EditAnywhere, Category = "Hex Tile Asset") //Makes the below variable editable in the editor
		UStaticMesh* HexTile; //Stores a reference to the hex tile static mesh for later instancing
	UPROPERTY(EditAnywhere, Category = "Reference Plane") //Makes the below variable editable in the editor
		AActor* RefPlane; //Stores a reference to the hex tile static mesh for later instancing

};