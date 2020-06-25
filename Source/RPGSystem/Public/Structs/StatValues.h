#pragma once

#include "StatValues.generated.h"

//////////////////////////////////////////////////////////////////////////
/// Stat Manager
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EStats : uint8
{
	VE_Empty 	UMETA(DisplayName = "Empty"),
	VE_Shield 	UMETA(DisplayName = "Shield"),
	VE_Health	UMETA(DisplayName = "Health")
};

USTRUCT(BlueprintType)
struct FStatValues
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxValue;

	void SetMin(const int32 NewValue) { MinValue = NewValue; }
	void SetCurrent(const int32 NewValue) { CurrentValue = NewValue; }
	void SetMax(const int32 NewValue) { MaxValue = NewValue; }

	int32 GetMin() { return MinValue; }

	int32 GetCurrent() { return CurrentValue; }

	int32 GetMax() { return MaxValue; }

	FStatValues()
	{
		MinValue = 0;
		CurrentValue = 50;
		MaxValue = 100;
	}

	FStatValues(int32 min, int32 current, int32 max)
	{
		MinValue = min;
		CurrentValue = current;
		MaxValue = max;
	}
};

