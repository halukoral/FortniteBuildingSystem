// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyWidget.h"


#define INVTEXT(x) FText::FromString(TEXT(x))

void UEnemyWidget::UpdateDistance(int32 Distance)
{
	if (Distance > MaxDisplayedDistance)
		DistanceText->SetText(FText::Format(INVTEXT("{0}+"), FText::AsNumber(MaxDisplayedDistance)));
	else
		DistanceText->SetText(FText::Format(INVTEXT("{0}m"), FText::AsNumber(Distance)));
}
