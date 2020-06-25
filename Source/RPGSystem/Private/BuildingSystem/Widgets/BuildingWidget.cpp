// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingWidget.h"

void UBuildingWidget::SelectBuilding(bool bSelect)
{
	if (bSelect != bSelected)
	{
		bSelected = bSelect;
		if (bSelected)
		{
			Outline->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			IconSpacer->SetSize(FVector2D(1.f, 5.f));
		}
		else
		{
			Outline->SetVisibility(ESlateVisibility::Hidden);
			IconSpacer->SetSize(FVector2D(1.f, 10.f));
		}
	}
}

void UBuildingWidget::NativeConstruct()
{
	HotkeyText->SetText(Building->GetDefaultObject<ABuilding>()->GetBuildingData().GetHotkey().GetDisplayName());
	Icon->SetBrushFromTexture(Building->GetDefaultObject<ABuilding>()->GetBuildingData().GetIcon(), true);
	SelectBuilding(false);
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
