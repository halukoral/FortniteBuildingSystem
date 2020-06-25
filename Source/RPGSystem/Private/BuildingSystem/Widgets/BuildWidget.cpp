// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildWidget.h"

#define INVTEXT(x) FText::FromString(TEXT(x))

void UBuildWidget::Update(TSubclassOf<AResource> Resource, FBuildingResourceVersion VersionInfo)
{
	HealthText->SetText(FText::Format(INVTEXT("{0} HP"), VersionInfo.GetMaxHealth()));
	ResourceAmountText->SetText(FText::AsNumber(VersionInfo.GetRequiredAmount()));
	ResourceIcon->SetBrushFromTexture(Resource->GetDefaultObject<AResource>()->GetResourceData().GetIcon(), true);

}

void UBuildWidget::SetCanBeBuilt(bool NewValue)
{
	if (NewValue != bCanBeBuilt)
	{
		bCanBeBuilt = NewValue;
		if (bCanBeBuilt)
		{
			ResourceIcon->SetColorAndOpacity(FColor::FromHex("FFFFFFFF"));
			ResourceAmountText->SetColorAndOpacity(FSlateColor(FColor::FromHex("FFFFFFE6")));
		}
		else
		{
			ResourceIcon->SetColorAndOpacity(FColor::FromHex("FF393FFF"));
			ResourceAmountText->SetColorAndOpacity(FSlateColor(FColor::FromHex("FF393FFF")));
		}
	
	}
}
