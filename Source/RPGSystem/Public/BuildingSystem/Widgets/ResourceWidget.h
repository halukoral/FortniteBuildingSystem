// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "../Resource/Resource.h"
#include "Blueprint/UserWidget.h"
#include "ResourceWidget.generated.h"

UCLASS()
class RPGSYSTEM_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SelectResource(bool bSelect, bool bIgnoreCheckObject);
	
	UTextBlock* GetAmount() { return Amount; }
	TSubclassOf<AResource> GetResource() { return Resource; }
	
	void SetResource(TSubclassOf<AResource> NewResource) { Resource = NewResource; }

protected:
	virtual void NativeConstruct();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	TSubclassOf<AResource> Resource;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bSelected;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UBorder* ResourceBorder;

};
