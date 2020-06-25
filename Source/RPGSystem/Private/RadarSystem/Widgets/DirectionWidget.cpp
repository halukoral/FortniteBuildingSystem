// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectionWidget.h"

void UDirectionWidget::NativeConstruct()
{
	FString NameStr = DirectionInfo.GetName().ToString();
	int32 NameLength = NameStr.Len();

	FText NewText = FText::FromString(NameStr.LeftChop(NameLength - 2).ToUpper());
	Text->SetText(NewText);
}
