// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexRenderer.generated.h"


using namespace std;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POST_SINGULARITY_API UHexRenderer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexRenderer();
private:
	AActor* Owner;
	float HexGap[3] = { 153.0f, 76.5f, 115.0f }; //Distance between Hex tiles HexGap[0] for Row, HexGap[1] for collum x, HexGap[2] for collum y
	UPROPERTY(EditAnywhere, Category = "Hex Tile Asset") //Makes the below variable editable in the editor
		UStaticMesh* HexTile; //Stores a reference to the hex tile static mesh for later instancing
	UPROPERTY(EditAnywhere, Category = "Reference Plane") //Makes the below variable editable in the editor
		AActor* RefPlane; //Stores a reference to the hex tile static mesh for later instancing

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
