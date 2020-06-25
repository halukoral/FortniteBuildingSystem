#pragma once

#include "BuildingData.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EBuildingState : uint8
{
	VE_Building 	UMETA(DisplayName = "Building"),
	VE_Built	 	UMETA(DisplayName = "Built"),
};

USTRUCT(BlueprintType)
struct FBuildingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FKey Hotkey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	void SetName(const FText NewName) { Name = NewName; }
	void SetIcon(UTexture2D* NewIcon) { Icon = NewIcon; }
	void SetHotkey(FKey NewHotkey) { Hotkey = NewHotkey; }

	FText GetName() { return Name; }
	UTexture2D* GetIcon() { return Icon; }
	FKey GetHotkey() { return Hotkey; }

	FBuildingData()
	{
		Name = FText::FromString("NoName");
		Icon = nullptr;
		Hotkey = EKeys::F1;
	}

	FBuildingData(FText NewName, UTexture2D* NewIcon, FKey NewHotkey)
	{
		Name = NewName;
		Icon = NewIcon;
		Hotkey = NewHotkey;
	}
};