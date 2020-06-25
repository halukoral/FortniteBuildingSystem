// Fill out your copyright notice in the Description page of Project Settings.

#include "MainWidget.h"

UMainWidget::UMainWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
	// Find what class you want 
	static ConstructorHelpers::FClassFinder<UResourceWidget> W_ResourceAsset(TEXT("/Game/BuildingSystem/Widgets/W_Resource"));
	if (W_ResourceAsset.Class != nullptr)
		W_ResourceClass = W_ResourceAsset.Class;
	static ConstructorHelpers::FClassFinder<UBuildingWidget> W_BuildingAsset(TEXT("/Game/BuildingSystem/Widgets/W_Building"));
	if (W_BuildingAsset.Class != nullptr)
		W_BuildingClass = W_BuildingAsset.Class;
}

void UMainWidget::SetCrossHair(UImage* NewCrossHair)
{
	CrossHair = NewCrossHair;
}

TArray<UResourceWidget*> UMainWidget::GenerateResourceWidget(UResourceManager* ResourceManager)
{
	auto Resources = ResourceManager->GetResources();
	TArray<TSubclassOf<AResource>> Keys;
	Resources.GetKeys(Keys);
	int i = 0;
	for (auto Key : Keys)
	{
		W_Resource = CreateWidget<UResourceWidget>(this, W_ResourceClass);
		if (W_Resource != nullptr)
		{
			W_Resource->SetResource(Key);
			ResourceWidgets.Add(W_Resource);
			auto Slot = ResourceBox->AddChildToHorizontalBox(W_Resource);
			if (i > 0)
				Slot->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
			else
				Slot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
		}
		++i;
	}
	return ResourceWidgets;
}

TArray<UBuildingWidget*> UMainWidget::GenerateBuildingWidgets(UBuildingManager* BuildingManager)
{
	auto Buildings = BuildingManager->GetBuildings();
	int i = 0;
	for (auto Building : Buildings)
	{
		W_Building = CreateWidget<UBuildingWidget>(this, W_BuildingClass);
		if (W_Building != nullptr)
		{
			W_Building->SetBuilding(Building);
			BuildingWidgets.Add(W_Building);
			auto Slot = BuildingBox->AddChildToHorizontalBox(W_Building);
			if (i > 0)
				Slot->SetPadding(FMargin(10.f, 0.f, 0.f, 0.f));
			else
				Slot->SetPadding(FMargin(0.f, 0.f, 0.f, 0.f));
		}
		++i;
	}
	return BuildingWidgets;
}

void UMainWidget::UpdateCurrentResource(UResourceManager* ResourceManager)
{
	UTexture2D* Texture = ResourceManager->GetSelectedResource()->GetDefaultObject<AResource>()->GetResourceData().GetIcon();
	CurrentResourceIcon->SetBrushFromTexture(Texture);
	int32 _Amount = ResourceManager->GetResourceAmount(ResourceManager->GetSelectedResource());
	CurrentResourceAmount->SetText(FText::AsNumber(_Amount));
}

void UMainWidget::ShowCurrentResource(bool bShowResource)
{
	if(bShowResource)
		CurrentResourceBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		CurrentResourceBox->SetVisibility(ESlateVisibility::Hidden);
}
