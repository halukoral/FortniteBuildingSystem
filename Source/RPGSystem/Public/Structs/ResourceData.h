#pragma once

#include "ResourceData.generated.h"

//If you want this to appear in BP, make sure to use this USTRUCT(BlueprintType)
USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GhostBuildingColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* MeshMaterial;

	void SetName(const FText NewName) { Name = NewName; }
	void SetIcon(UTexture2D* NewIcon) { Icon = NewIcon; }
	void SetLinearColor(FLinearColor Color) { GhostBuildingColor = Color; }
	void SetMeshMaterial(UMaterial* NewMaterial) { MeshMaterial = NewMaterial; }

	FText GetName() { return Name; }
	UTexture2D* GetIcon() { return Icon; }
	FLinearColor GetColor() { return GhostBuildingColor; }
	UMaterial* GetMeshMaterial() { return MeshMaterial; }

	FResourceData()
	{
		Name = FText::FromString("NoName");
		Icon = nullptr;
		GhostBuildingColor = FLinearColor::Blue;
		//MeshMaterial = NewObject<UMaterial>();
		// TODO : Every time hot reload editor will crash
	}
};