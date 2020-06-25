// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource_Metal.h"

AResource_Metal::AResource_Metal()
{
	ResourceData.SetName(FText::FromString("Metal"));
	
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		IconTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Resources/Icon_Metal.Icon_Metal'"));
	if (IconTexture.Object != nullptr)
		ResourceData.SetIcon(IconTexture.Object);
	
	ResourceData.SetLinearColor(FColor::FromHex("1280FFFF"));

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/BuildingSystem/Materials/M_Ice.M_Ice'"));
	if (Material.Object != nullptr)
		ResourceData.SetMeshMaterial(Material.Object);
}
