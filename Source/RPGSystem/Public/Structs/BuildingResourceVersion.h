#pragma once

#include "BuildingResourceVersion.generated.h"

USTRUCT(BlueprintType)
struct FBuildingResourceVersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BuildingTime = 2.0f;

	void SetRequiredAmount(const int32 NewRequiredAmount) { RequiredAmount = NewRequiredAmount; }
	void SetMaxHealth(const int32 NewMaxHealth) { MaxHealth = NewMaxHealth; }
	void SetBuildingTime(const float NewBuildingTime) { BuildingTime = NewBuildingTime; }

	int32 GetRequiredAmount() { return RequiredAmount; }
	int32 GetMaxHealth() { return MaxHealth; }
	float GetBuildingTime() { return BuildingTime; }

	FBuildingResourceVersion()
	{
		RequiredAmount = 10;
		MaxHealth = 100;
		BuildingTime = 2.f;
	}

	FBuildingResourceVersion(const int32 _RequiredAmount, const int32 _MaxHealth, const float _BuildingTime)
	{
		RequiredAmount = _RequiredAmount;
		MaxHealth = _MaxHealth;
		BuildingTime = _BuildingTime;
	}
};