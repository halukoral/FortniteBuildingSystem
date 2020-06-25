// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Structs/MarkerInfo.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "QuestWidget.generated.h"


UCLASS()
class RPGSYSTEM_API UQuestWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuestWidget(const FObjectInitializer & ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetArrowDirection(bool bUp);

	UFUNCTION(BlueprintCallable)
	void UpdateDistance(int32 Distance);
	
	void SetType(EMarkerTypes NewValue) { Type = NewValue; }

	UImage* GetArrow() { return Arrow; }

protected:
	virtual void NativeConstruct();

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* QuestIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* DistanceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMarkerTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxDisplayedDistance = 999;

	UTexture2D* Main;
	UTexture2D* Side;
};
