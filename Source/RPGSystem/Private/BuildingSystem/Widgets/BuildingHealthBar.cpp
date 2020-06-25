// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingHealthBar.h"

#define INVTEXT(x) FText::FromString(TEXT(x))



void UBuildingHealthBar::UpdateHealth(const int32& NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
	HealthBar->SetPercent((float)CurrentHealth / MaxHealth);
	HealthText->SetText(FText::Format(INVTEXT("{0} | {1}"), FText::AsNumber(CurrentHealth), FText::AsNumber(MaxHealth)));
}

void UBuildingHealthBar::SetState(EBuildingState NewState)
{
	State = NewState;
	if (State == EBuildingState::VE_Building)
		HealthBar->SetFillColorAndOpacity(FColor::FromHex("864803FF"));
	else
		HealthBar->SetFillColorAndOpacity(FColor::FromHex("23A418FF"));
}

void UBuildingHealthBar::InitBar(const int32& NewValue)
{
	MaxHealth = NewValue;
	SetState(EBuildingState::VE_Building);
	UpdateHealth(0);
}
