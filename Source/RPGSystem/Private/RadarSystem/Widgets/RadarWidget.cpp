// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarWidget.h"

URadarWidget::URadarWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	// Find what class you want 
	static ConstructorHelpers::FClassFinder<UDirectionWidget> W_DirectionAsset(TEXT("/Game/RadarSystem/Widgets/W_Direction"));
	if (W_DirectionAsset.Class != nullptr)
		W_DirectionClass = W_DirectionAsset.Class;

	static ConstructorHelpers::FClassFinder<UQuestWidget> W_QuestAsset(TEXT("/Game/RadarSystem/Widgets/W_Quest"));
	if (W_QuestAsset.Class != nullptr)
		W_QuestClass = W_QuestAsset.Class;

	static ConstructorHelpers::FClassFinder<ULandmarkWidget> W_LandmarkAsset(TEXT("/Game/RadarSystem/Widgets/W_Landmark"));
	if (W_LandmarkAsset.Class != nullptr)
		W_LandmarkClass = W_LandmarkAsset.Class;

	static ConstructorHelpers::FClassFinder<UEnemyWidget> W_EnemyAsset(TEXT("/Game/RadarSystem/Widgets/W_Enemy"));
	if (W_EnemyAsset.Class != nullptr)
		W_EnemyClass = W_EnemyAsset.Class;

}

void URadarWidget::AddDirectionWidget(FDirectionInfo Info)
{
	W_Direction = CreateWidget<UDirectionWidget>(this, W_DirectionClass);
	if (W_Direction != nullptr)
	{
		W_Direction->SetDirectionInfo(Info);
		FString NameStr = W_Direction->GetDirectionInfo().GetName().ToString();
		int32 rot = W_Direction->GetDirectionInfo().GetWorldRot();

		DirectionWidgets.Add(W_Direction);
		UOverlaySlot* Slot = RadarOverlay->AddChildToOverlay(W_Direction);
		Slot->SetHorizontalAlignment(HAlign_Center);
		Slot->SetVerticalAlignment(VAlign_Bottom);
	}
}

void URadarWidget::AddEnemyWidget()
{
	W_Enemy = CreateWidget<UEnemyWidget>(this, W_EnemyClass);
	if (W_Enemy != nullptr)
	{
		EnemyWidgets.Add(W_Enemy);
		UOverlaySlot* Slot = RadarOverlay->AddChildToOverlay(W_Enemy);
		Slot->SetHorizontalAlignment(HAlign_Center);
		Slot->SetVerticalAlignment(VAlign_Center);
		W_Enemy->SetPadding(FMargin(0.f, 0.f, 0.f, 7.f));
	}
}

void URadarWidget::AddQuestWidget(EMarkerTypes Type)
{
	W_Quest = CreateWidget<UQuestWidget>(this, W_QuestClass);
	if (W_Quest != nullptr)
	{
		W_Quest->SetType(Type);
		QuestWidgets.Add(W_Quest);
		UOverlaySlot* Slot = RadarOverlay->AddChildToOverlay(W_Quest);
		Slot->SetHorizontalAlignment(HAlign_Center);
		Slot->SetVerticalAlignment(VAlign_Center);
		W_Quest->SetPadding(FMargin(0.f, 5.f, 0.f, 0.f));
	}
}

void URadarWidget::AddLandmarkWidget(UTexture2D* IconTexture)
{
	W_Landmark = CreateWidget<ULandmarkWidget>(this, W_LandmarkClass);
	if (W_Landmark != nullptr)
	{
		W_Landmark->SetLandmarkIcon(IconTexture);
		LandmarkWidgets.Add(W_Landmark);
		UOverlaySlot* Slot = RadarOverlay->AddChildToOverlay(W_Landmark);
		Slot->SetHorizontalAlignment(HAlign_Center);
		Slot->SetVerticalAlignment(VAlign_Center);
	}
}

void URadarWidget::HighlightRadar(bool bHighlight)
{
	if (bHighlight)
		Radar->SetColorAndOpacity(FColor::FromHex("FFA700FF"));
	else
		Radar->SetColorAndOpacity(FColor::FromHex("FFFFFFB3"));
}

void URadarWidget::DeleteEnemy(int32 Index)
{
	UEnemyWidget* EnemyWidget = EnemyWidgets[Index];
	EnemyWidget->RemoveFromParent();
	EnemyWidgets.Remove(EnemyWidget);
}
