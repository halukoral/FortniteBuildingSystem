// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource_Stone.h"




AResource_Stone::AResource_Stone()
{
	ResourceData.SetName(FText::FromString("Stone"));
	
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		IconTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Resources/Icon_Stone.Icon_Stone'"));
	if (IconTexture.Object != nullptr)
		ResourceData.SetIcon(IconTexture.Object);
	
	ResourceData.SetLinearColor(FColor::FromHex("4D2C17FF"));

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/BuildingSystem/Materials/M_Stone.M_Stone'"));
	if (Material.Object != nullptr)
		ResourceData.SetMeshMaterial(Material.Object);
}
