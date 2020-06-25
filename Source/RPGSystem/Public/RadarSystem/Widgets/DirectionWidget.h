// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Public/Structs/DirectionInfo.h"
#include "Components/TextBlock.h"
#include "DirectionWidget.generated.h"


UCLASS()
class RPGSYSTEM_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

public:
	void SetDirectionInfo(FDirectionInfo NewDirectionInfo) { DirectionInfo = NewDirectionInfo; }
	FDirectionInfo GetDirectionInfo() { return DirectionInfo; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FDirectionInfo DirectionInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", BindWidget))
	UTextBlock* Text;
	
};
