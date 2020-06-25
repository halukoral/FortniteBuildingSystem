// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MainWidget.h"
#include "Utils/Utils.h"
#include "Structs/StatValues.h"
#include "Player/RPGSystemCharacter.h"
#include "BuildingSystem/Widgets/StatWidget.h"
#include "Components/ActorComponent.h"
#include "StatManager.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGSYSTEM_API UStatManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatManager();
	
protected:
	virtual void BeginPlay() override;

public:	
	void AddStat(EStats Stat);
	
	/**
	 * Update UI elements
	 * @param Stat Stat that has to be updated 
	 */ 
	virtual void UpdateStat(EStats Stat);

	/**
	* Get the stat bar for given Stat ex: Health Bar or Shield Bar
	* @param Stat This is for select which widget we want
	* @return StatWidget for specific Stat
	*/
	UStatWidget* GetBarForStat(EStats Stat);
	
	/**
	* Get StatValues from Stat
	* @param Stat Which is the Stat we have to find
	* @return StatValue Structure which has min, 
	*  current and max value of that stat
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FStatValues GetStat(EStats Stat);

	/**
	* Set only CurrentValue of Stat
	* @param Stat Which is the Stat we have to find on TMap
	* @param NewValue New CurrentValue that we want to set it to
	* @param bIsUpdate Whether we want to update display or not
	* @return StatValue Structure which has min,
	*  current and max value of that stat
	*/
	void SetStat(const EStats Stat, const int32 NewValue, bool bIsUpdate = true);

	/**
	* Modify CurrentValue of Stat
	* @param Stat Which has to be modified
	* @param NewValue Amount that we want to modify it by
	*/
	UFUNCTION(BlueprintCallable)
	void ModifyStat(const EStats Stat, const int32 NewValue);

	/**
	* Usually Game Designer can enter mistakenly bigger CurrentValue than 
	* MaxValue or vice versa so we have to prevent it and set all values correctly 
	*/
	UFUNCTION(BlueprintCallable)
	void SetupStats();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EStats, FStatValues> Stats;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMainWidget* MainWidget;
};
