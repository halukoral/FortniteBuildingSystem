// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BillboardComponent.h"
#include "Landmark.generated.h"

UCLASS()
class RPGSYSTEM_API ALandmark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandmark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTexture2D* GetIconRadar() { return IconOnRadar; }

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UBillboardComponent * BillboardComponent;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UTexture2D* IconOnRadar;
};
