// Fill out your copyright notice in the Description page of Project Settings.


#include "TileInstancer.h"

TileInstancer::TileInstancer(AActor* Owner, UStaticMesh* TileMesh)
{
	Tile = NewObject<UInstancedStaticMeshComponent>(Owner);
	//Tile->Rename(new TCHAR(*"HexCollumn"));
	Tile->RegisterComponent();
	Tile->SetStaticMesh(TileMesh);
	Owner->AddInstanceComponent(Tile);
}

TileInstancer::~TileInstancer()
{
}

void TileInstancer::SetUp(AActor* Owner, UStaticMesh* TileMesh)
{
	Tile = NewObject<UInstancedStaticMeshComponent>(Owner);
	Tile->RegisterComponent();
	Tile->SetStaticMesh(TileMesh);
	Owner->AddInstanceComponent(Tile);
	return;
}