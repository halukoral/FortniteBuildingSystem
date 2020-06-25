// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Utils/Utils.h"
#include "BuildingSystem/Resource/Resource.h"
#include "BuildingSystem/Resource/ResourceManager.h"
#include "BuildingSystem/Building/BuildingManager.h"
#include "RadarSystem/Widgets/RadarWidget.h"
#include "BuildingSystem/Widgets/StatWidget.h"
#include "BuildingSystem/Widgets/BuildingWidget.h"
#include "BuildingSystem/Widgets/ResourceWidget.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class RPGSYSTEM_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainWidget(const FObjectInitializer & ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	URadarWidget* GetRadarWidget() { return Radar; }

	UFUNCTION(BlueprintCallable)
	UStatWidget* GetShieldBar() { return ShieldBar; }

	UFUNCTION(BlueprintCallable)
	UStatWidget* GetHealthBar() { return HealthBar; }

	UFUNCTION(BlueprintCallable)
	UImage* GetCrossHair() { return CrossHair; }

	UFUNCTION(BlueprintCallable)
	UImage* GetStateIcon() { return StateIcon; }

	UFUNCTION(BlueprintCallable)
	UHorizontalBox* GetBuildingBox() { return BuildingBox; }

	UFUNCTION(BlueprintCallable)
	void SetCrossHair(UImage* NewCrossHair);

	UFUNCTION(BlueprintCallable)
	TArray<UResourceWidget*> GenerateResourceWidget(UResourceManager* ResourceManager);

	UFUNCTION(BlueprintCallable)
	TArray<UBuildingWidget*> GenerateBuildingWidgets(UBuildingManager* BuildingManager);

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentResource(UResourceManager* ResourceManager);

	UFUNCTION(BlueprintCallable)
	void ShowCurrentResource(bool bShowResource);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	URadarWidget* Radar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UStatWidget* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UStatWidget* ShieldBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UHorizontalBox* BuildingBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* CrossHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* CurrentResourceAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	USizeBox* CurrentResourceBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* CurrentResourceIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* StateIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UHorizontalBox* ResourceBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UResourceWidget*> ResourceWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UBuildingWidget*> BuildingWidgets;

	//////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UResourceWidget* W_Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBuildingWidget* W_Building;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UResourceWidget> W_ResourceClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBuildingWidget> W_BuildingClass;
};
