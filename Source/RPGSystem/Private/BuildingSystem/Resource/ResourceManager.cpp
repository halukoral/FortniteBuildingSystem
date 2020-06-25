// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceManager.h"
#include "Player/RPGSystemCharacter.h"
#include "BuildingManager.h"
#include "UI/MainWidget.h"

// Sets default values for this component's properties
UResourceManager::UResourceManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UResourceManager::BeginPlay()
{
	Super::BeginPlay();
	BuildingManager = GetOwner()->FindComponentByClass<UBuildingManager>();
}

int32 UResourceManager::GetResourceAmount(TSubclassOf<AResource> Resource)
{
	if (Resources.Find(Resource) != nullptr)
		return *Resources.Find(Resource);
	else
		return -1;
}

void UResourceManager::AddResource(TSubclassOf<AResource> Resource, int32 Amount)
{
	if (Amount > 0 && IsValid(Resource))
	{
		if (Resources.Find(Resource) != nullptr)
		{
			int32 NewAmount = *Resources.Find(Resource) + Amount;
			Resources.Add(Resource, FMath::Clamp(NewAmount, minValue, maxValue));
		}
		else
		{
			Resources.Add(Resource, FMath::Clamp(Amount, minValue, maxValue));
		}
		UpdateResourceWidget(Resource);
	}
}

void UResourceManager::RemoveResource(TSubclassOf<AResource> Resource, int32 Amount)
{
	if (Amount > 0 && IsValid(Resource))
	{
		if (Resources.Find(Resource) != nullptr)
		{
			int32 NewAmount = *Resources.Find(Resource) - Amount;
			Resources.Add(Resource, FMath::Clamp(NewAmount, minValue, maxValue));
			UpdateResourceWidget(Resource);
		}
	}
}

void UResourceManager::InitResourceWidget(TSubclassOf<AResource> Resource)
{
	if (UUtils::Test(4, MainWidget, Player, (__FUNCTION__), __LINE__) == false) return;

	int32 _Amount = GetResourceAmount(Resource);
	UResourceWidget* Widget;
	if (GetWidgetForResource(Resource, Widget))
	{
		Widget->GetAmount()->SetText(FText::AsNumber(_Amount));
		if (Resource == GetSelectedResource() && Player->GetPlayerStates() == EPlayerStates::VE_Construction)
		{
			MainWidget->UpdateCurrentResource(this);
		}
	}
}

void UResourceManager::InitResourceWidgets()
{
	TArray<TSubclassOf<AResource>> Keys;
	Resources.GetKeys(Keys);
	for (auto Key : Keys)
		InitResourceWidget(Key);
}

void UResourceManager::UpdateResourceWidget(TSubclassOf<AResource> Resource)
{
	if (UUtils::Test(4, Resource, Player, (__FUNCTION__), __LINE__) == false) return;

	UResourceWidget* Widget;
	if (GetWidgetForResource(Resource, Widget))
	{
		int32 _Amount = GetResourceAmount(Resource);
		
		if (UUtils::Test(5, Widget, MainWidget, BuildingManager, (__FUNCTION__), __LINE__) == false) return;

		Widget->GetAmount()->SetText(FText::AsNumber(_Amount));
		if (Resource == GetSelectedResource() && Player->GetPlayerStates() == EPlayerStates::VE_Construction)
		{
			MainWidget->UpdateCurrentResource(this);
			BuildingManager->OnResourceValueModified(Resource);
		}
	}
}

TSubclassOf<AResource> UResourceManager::GetSelectedResource()
{
	TArray<TSubclassOf<AResource>> Keys;
	Resources.GetKeys(Keys);
	return Keys[SelectedResourceIndex];
}

bool UResourceManager::GetWidgetForResource(TSubclassOf<AResource> NewResource, UResourceWidget*& Widget)
{
	for (auto Resource : ResourceWidgets)
	{
		if (Resource->GetResource() == NewResource)
		{
			Widget = Resource;
			return true;
		}
	}
	return false;
}

UResourceWidget* UResourceManager::CurrentlySelectedWidget()
{
	TSubclassOf<AResource> Resource = GetSelectedResource();
	UResourceWidget* ResourceWidget;
	GetWidgetForResource(Resource, ResourceWidget);
	return ResourceWidget;
}

void UResourceManager::HighlightSelectedWidget()
{
	CurrentlySelectedWidget()->SelectResource(true, false);
}

void UResourceManager::SelectNextResourceWidget()
{
	CurrentlySelectedWidget()->SelectResource(false, false);
	TArray<TSubclassOf<AResource>> ResourceKeys;
	Resources.GetKeys(ResourceKeys);
	
	if (SelectedResourceIndex < ResourceKeys.Num() - 1)
		++SelectedResourceIndex;
	else
		SelectedResourceIndex = 0;

	HighlightSelectedWidget();
	MainWidget->UpdateCurrentResource(this);
	OnResourceSwitched();
}

void UResourceManager::OnResourceSwitched()
{
	if (UUtils::Test(3, BuildingManager, (__FUNCTION__), __LINE__) == false) return;

	BuildingManager->OnResourceChanged();
}
