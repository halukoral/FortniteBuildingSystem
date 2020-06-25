// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/ResourceData.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "Resource.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Resource", meta = (BlueprintSpawnableComponent))
class RPGSYSTEM_API AResource : public AActor
{
	GENERATED_BODY()
public:	
	FResourceData GetResourceData() { return ResourceData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FResourceData ResourceData;
	
};
