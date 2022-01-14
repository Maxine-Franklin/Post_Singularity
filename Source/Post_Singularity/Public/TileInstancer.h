// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class POST_SINGULARITY_API TileInstancer
{
public:
	UInstancedStaticMeshComponent* Tile;

	TileInstancer(AActor* Owner, UStaticMesh* TileMesh);
	~TileInstancer();

	void SetUp(AActor* Owner, UStaticMesh* TileMesh);
};