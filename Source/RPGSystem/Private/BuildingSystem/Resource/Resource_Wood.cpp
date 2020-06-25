// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource_Wood.h"




AResource_Wood::AResource_Wood()
{
	ResourceData.SetName(FText::FromString("Wood"));
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> 
		IconTexture(TEXT("Texture2D'/Game/BuildingSystem/Textures/Resources/Icon_Wood.Icon_Wood'"));
	if(IconTexture.Object != nullptr)
		ResourceData.SetIcon(IconTexture.Object);
	
	ResourceData.SetLinearColor(FColor::FromHex("2C4F25FF"));

	static ConstructorHelpers::FObjectFinder<UMaterial>
		Material(TEXT("/Game/BuildingSystem/Materials/M_Wood.M_Wood"));
	if(Material.Object != nullptr)
		ResourceData.SetMeshMaterial(Material.Object);
}
