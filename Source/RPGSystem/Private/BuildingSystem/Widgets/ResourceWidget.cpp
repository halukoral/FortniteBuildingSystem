// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceWidget.h"


void UResourceWidget::SelectResource(bool bSelect, bool bIgnoreCheckObject)
{
	if (bSelect != bSelected || bIgnoreCheckObject)
	{
		bSelected = bSelect;
		FLinearColor Color = bSelected ? FColor::FromHex("1A2DB34C") : FColor::FromHex("0000004C");
		ResourceBorder->SetBrushColor(Color);

		Color = bSelected ? FColor::FromHex("FFFFFFCC") : FColor::FromHex("FFFFFFCC");
		Amount->SetColorAndOpacity(Color);
	}
}

void UResourceWidget::NativeConstruct()
{
	Icon->SetBrushFromTexture(Resource->GetDefaultObject<AResource>()->GetResourceData().GetIcon());
	this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

