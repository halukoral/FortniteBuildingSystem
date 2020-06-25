#pragma once

#include "DirectionInfo.generated.h"

USTRUCT(BlueprintType)
struct FDirectionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WorldRotation;

	void SetName(const FText NewValue) { Name = NewValue; }
	void SetRotation(const int32 NewValue) { WorldRotation = NewValue; }

	FText GetName() { return Name; }

	int32 GetWorldRot() { return WorldRotation; }

	FDirectionInfo()
	{
		Name = FText::GetEmpty();
		WorldRotation = 0;
	}

	FDirectionInfo(const FText NewName, const int32 NewWorldRot)
	{
		Name = NewName;
		WorldRotation = NewWorldRot;
	}
};