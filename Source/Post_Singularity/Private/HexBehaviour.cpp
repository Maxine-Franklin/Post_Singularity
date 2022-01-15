// Fill out your copyright notice in the Description page of Project Settings.


#include "HexBehaviour.h"

// Sets default values for this component's properties
UHexBehaviour::UHexBehaviour()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UHexBehaviour::SetSelf(AActor* Tile)
{
	Self = Tile;
	return;
}

// Called when the game starts
void UHexBehaviour::BeginPlay()
{
	Super::BeginPlay();
	Self = GetOwner();
	// ...
	
}


// Called every frame
void UHexBehaviour::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

