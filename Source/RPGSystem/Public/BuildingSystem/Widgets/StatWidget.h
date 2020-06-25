// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public/Math/Color.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "../Structs/StatValues.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

UCLASS()
class RPGSYSTEM_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Called when current value of Stat changed
	 * @param NewCurrentValue New value of Current (UTextBlock)
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateStat(int32 NewCurrentValue);

	/**
	* Called when we want to init bar
	* @param StatValue Struct that contains values 
	* for MinValue, CurrentValue and MaxValue
	*/
	UFUNCTION(BlueprintCallable)
	void InitBar(FStatValues StatValue);

protected:
	virtual void NativeConstruct();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* StatIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FLinearColor BarColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UProgressBar* Bar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* Current;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UImage* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MinValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 MaxValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bInit;
	
};
