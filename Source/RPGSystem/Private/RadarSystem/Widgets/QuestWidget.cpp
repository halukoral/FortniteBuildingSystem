// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestWidget.h"

#define INVTEXT(x) FText::FromString(TEXT(x))

UQuestWidget::UQuestWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>
		MainTexture(TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_MainQuest.Icon_MainQuest'"));
	if (MainTexture.Object != nullptr)
		Main = MainTexture.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D>
		SideTexture(TEXT("Texture2D'/Game/RadarSystem/Textures/Icon_SideQuest.Icon_SideQuest'"));
	if (SideTexture.Object != nullptr)
		Side = SideTexture.Object;
}

void UQuestWidget::NativeConstruct()
{
	if (Type == EMarkerTypes::VE_MainQuest)
	{
		if (Main != nullptr)
			QuestIcon->SetBrushFromTexture(Main, true);
		Arrow->SetColorAndOpacity(FColor::FromHex("9C5B19FF"));
	}
	else
	{
		if (Side != nullptr)
			QuestIcon->SetBrushFromTexture(Side, true);

		Arrow->SetColorAndOpacity(FColor::FromHex("1F4FB9FF"));
	}
}

void UQuestWidget::SetArrowDirection(bool bUp)
{
	if (bUp)
		Arrow->SetRenderScale(FVector2D(1.f, 1.f));
	else
		Arrow->SetRenderScale(FVector2D(1.f, -1.f));
}

void UQuestWidget::UpdateDistance(int32 Distance)
{
	if (Distance > MaxDisplayedDistance)
		DistanceText->SetText(FText::Format(INVTEXT("{0}+"), FText::AsNumber(MaxDisplayedDistance)));
	else
		DistanceText->SetText(FText::Format(INVTEXT("{0}m"), FText::AsNumber(FMath::Clamp<int32>(Distance, 0, MaxDisplayedDistance))));
}
