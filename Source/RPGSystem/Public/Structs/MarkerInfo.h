#pragma once

#include "MarkerInfo.generated.h"

//////////////////////////////////////////////////////////////////////////
/// Radar System
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EMarkerTypes : uint8
{
	VE_MainQuest 	UMETA(DisplayName = "Main Quest"),
	VE_SideQuest 	UMETA(DisplayName = "Side Quest"),
};

USTRUCT(BlueprintType)
struct FMarkerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMarkerTypes QuestType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	void SetQuestType(const EMarkerTypes NewValue) { QuestType = NewValue; }
	void SetQuestLocation(const FVector NewValue) { Location = NewValue; }


	EMarkerTypes GetQuestType() { return QuestType; }

	FVector GetQuestLocation() { return Location; }

	FMarkerInfo()
	{
		QuestType = EMarkerTypes::VE_MainQuest;
		Location = FVector::ZeroVector;
	}

	FMarkerInfo(const EMarkerTypes NewTypes, const FVector NewQuestLocation)
	{
		QuestType = NewTypes;
		Location = NewQuestLocation;
	}
};