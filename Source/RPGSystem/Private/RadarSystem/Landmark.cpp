// Fill out your copyright notice in the Description page of Project Settings.

#include "Landmark.h"


// Sets default values
ALandmark::ALandmark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BillboardComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = BillboardComponent;
	SetActorScale3D(FVector(6.f, 6.f, 6.f));

#if WITH_EDITORONLY_DATA
	IconOnRadar = CreateEditorOnlyDefaultSubobject<UTexture2D>(TEXT("Icon"));
#endif 
}

// Called when the game starts or when spawned
void ALandmark::BeginPlay()
{
	Super::BeginPlay();
}

void ALandmark::OnConstruction(const FTransform& Transform)
{
	if(IconOnRadar != nullptr)
		BillboardComponent->SetSprite(IconOnRadar);
}
