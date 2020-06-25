// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "BuildingSystem/Resource/Resource.h"
#include "Structs/BuildingResourceVersion.h"
#include "BuildWidget.generated.h"


UCLASS()
class RPGSYSTEM_API UBuildWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Update(TSubclassOf<AResource> Resource, FBuildingResourceVersion VersionInfo);
	
	UFUNCTION(BlueprintCallable)
	void SetCanBeBuilt(bool NewValue);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* ResourceAmountText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* ResourceIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	bool bCanBeBuilt = true;
	
};
