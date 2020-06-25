// Fill out your copyright notice in the Description page of Project Settings.

#include "StatWidget.h"


void UStatWidget::UpdateStat(int32 NewStat)
{
	if (bInit)
	{
		CurrentValue = NewStat;
		Current->SetText(FText::AsNumber(CurrentValue));
		float Percent = (float)(CurrentValue - MinValue) / (MaxValue - MinValue);
		Bar->SetPercent(Percent);
	}
}

void UStatWidget::InitBar(FStatValues StatValue)
{
	MinValue = StatValue.GetMin();
	CurrentValue = StatValue.GetCurrent();
	MaxValue = StatValue.GetMax();
	bInit = true;

	Max->SetText(FText::AsNumber(MaxValue));
	UpdateStat(CurrentValue);
}

void UStatWidget::NativeConstruct()
{
	Icon->SetBrushFromTexture(StatIcon);
	Bar->SetFillColorAndOpacity(BarColor);
}
