// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource_Ice.h"

AResource_Ice::AResource_Ice()
{
	ResourceData.SetName(FText::FromString("Ice"));

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		IconTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Resources/Icon_Snow.Icon_Snow'"));
	if (IconTexture.Object != nullptr)
		ResourceData.SetIcon(IconTexture.Object);

	ResourceData.SetLinearColor(FColor::FromHex("1280FFFF"));

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("Material'/Game/BuildingSystem/Materials/M_Ice.M_Ice'"));
	if (Material.Object != nullptr)
		ResourceData.SetMeshMaterial(Material.Object);
}
