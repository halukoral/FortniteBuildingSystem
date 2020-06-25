// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Widgets/ResourceWidget.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "BuildingSystem/Resource/Resource.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ActorComponent.h"
#include "ResourceManager.generated.h"

class UBuildingManager;
class ARPGSystemCharacter;
class UMainWidget;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPGSYSTEM_API UResourceManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Getter Functions
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedByIndex() { return SelectedResourceIndex; }

	UFUNCTION(BlueprintCallable)
	TMap<TSubclassOf<AResource>, int32> GetResources() { return Resources; }

	/**
	* Add Resource and its amount to Resources(Tmap)
	* @param Resource that we want to add
	* @param Amount amount of the given resource
	*/
	UFUNCTION(BlueprintCallable)
	void AddResource(TSubclassOf<AResource> Resource, int32 Amount);

	/**
	* Remove Resource and its amount from Resources(Tmap)
	* @param Resource that we want to remove
	* @param Amount amount of the given resource
	*/
	UFUNCTION(BlueprintCallable)
	void RemoveResource(TSubclassOf<AResource> Resource, int32 Amount);

	/**
	* Initialize resource widgets with given resource on the UI 
	* @param Resource which we want to show on UI
	*/
	UFUNCTION(BlueprintCallable)
	void InitResourceWidget(TSubclassOf<AResource> Resource);

	/** Initialize whole Resource Widgets on UI */
	UFUNCTION(BlueprintCallable)
	void InitResourceWidgets();

	/**
	* Update our resources in the UI when any 
	* change happen to our Resources
	* @param Resource which is the resource 
	* that we are looking for on the UI
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateResourceWidget(TSubclassOf<AResource> Resource);

	/**
	* Get the amount of the resource
	* @param Resource which is the resource that we are looking for
	* @return Amount of Resource
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetResourceAmount(TSubclassOf<AResource> Resource);

	/** This function will just return what resource is currently selected */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AResource> GetSelectedResource();

	/** 
	* Get Resource Widget for specified Resource  
	* @param Resource The Resource that we are looking on UI
	* @param Widget If the widget found get reference for it otherwise nullptr
	* @return If widget for specified Resource find return true otherwise return false
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetWidgetForResource(TSubclassOf<AResource> Resource, UResourceWidget*& Widget);

	/** Get Resource Widget for Currently Selected Resource */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UResourceWidget* CurrentlySelectedWidget();


	UFUNCTION(BlueprintCallable)
	void HighlightSelectedWidget();

	UFUNCTION(BlueprintCallable)
	void SelectNextResourceWidget();

	UFUNCTION(BlueprintCallable)
	void OnResourceSwitched();
		
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<AResource>, int32> Resources;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ARPGSystemCharacter* Player;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBuildingManager* BuildingManager;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UResourceWidget*> ResourceWidgets;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMainWidget* MainWidget;

	int minValue = 0;
	int maxValue = 999;
	int32 SelectedResourceIndex;
};
