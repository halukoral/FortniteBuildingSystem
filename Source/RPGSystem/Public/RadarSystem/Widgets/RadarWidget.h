// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DirectionWidget.h"
#include "QuestWidget.h"
#include "LandmarkWidget.h"
#include "EnemyWidget.h"
#include "Layout/Margin.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "RadarWidget.generated.h"

UCLASS()
class RPGSYSTEM_API URadarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	URadarWidget(const FObjectInitializer & ObjectInitializer);

	//////////////////////////////////////////////////////////////////////////
	/// Add Functions
	UFUNCTION(BlueprintCallable)
	void AddDirectionWidget(FDirectionInfo Info);

	UFUNCTION(BlueprintCallable)
	void AddEnemyWidget();

	UFUNCTION(BlueprintCallable)
	void AddQuestWidget(EMarkerTypes Type);

	UFUNCTION(BlueprintCallable)
	void AddLandmarkWidget(UTexture2D* IconTexture);

	UFUNCTION(BlueprintCallable)
	void HighlightRadar(bool bHighlight);

	//////////////////////////////////////////////////////////////////////////
	/// Getter Functions
	TArray<ULandmarkWidget*> GetLandmarkWidgets() { return LandmarkWidgets; }

	TArray<UDirectionWidget*> GetDirectionWidgets() { return DirectionWidgets; }

	TArray<UEnemyWidget*> GetEnemyWidgets() { return EnemyWidgets; }

	TArray<UQuestWidget*> GetQuestWidgets() { return QuestWidgets; }
	//////////////////////////////////////////////////////////////////////////
	
	UFUNCTION(BlueprintCallable)
	void DeleteEnemy(int32 Index);

private:
	// The image widget allows you to display a Slate Brush, or texture or material in the UI.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* Radar;

	// Overlay widgets allow for layering several widgets on top of each other. 
	// Each slot of an overlay represents a layer that can contain one widget. 
	// The slots will be rendered on top of each other in the order they are declared in code.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UOverlay* RadarOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetAnimation* BlendOuts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UDirectionWidget*> DirectionWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UEnemyWidget*> EnemyWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UQuestWidget*> QuestWidgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ULandmarkWidget*> LandmarkWidgets;

	UPROPERTY()
	UDirectionWidget*	W_Direction;

	UPROPERTY()
	UQuestWidget*		W_Quest;

	UPROPERTY()
	ULandmarkWidget*	W_Landmark;

	UPROPERTY()
	UEnemyWidget*		W_Enemy;

	UPROPERTY()
	TSubclassOf<UDirectionWidget>	W_DirectionClass;

	UPROPERTY()
	TSubclassOf<UQuestWidget>		W_QuestClass;

	UPROPERTY()
	TSubclassOf<ULandmarkWidget>	W_LandmarkClass;

	UPROPERTY()
	TSubclassOf<UEnemyWidget>		W_EnemyClass;
};
