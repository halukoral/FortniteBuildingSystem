// Fill out your copyright notice in the Description page of Project Settings.

#include "StatManager.h"
#include <Kismet/GameplayStatics.h>


// Sets default values for this component's properties
UStatManager::UStatManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UStatManager::AddStat(EStats Stat)
{
	Stats.Add(Stat, FStatValues());
}

// Called when the game starts
void UStatManager::BeginPlay()
{
	Super::BeginPlay();
}

void UStatManager::UpdateStat(EStats Stat)
{
	UStatWidget* StatWidget = GetBarForStat(Stat);
	if (UUtils::Test(3, StatWidget, (__FUNCTION__), __LINE__) == false) return;
	StatWidget->UpdateStat(GetStat(Stat).GetCurrent());
}

UStatWidget* UStatManager::GetBarForStat(EStats Stat)
{
	if (UUtils::Test(3, MainWidget, (__FUNCTION__), __LINE__) == false) return nullptr;
	
	if (Stat == EStats::VE_Shield)
		return MainWidget->GetShieldBar();
	else if (Stat == EStats::VE_Health)
		return MainWidget->GetHealthBar();
	
	return nullptr;
}

FStatValues UStatManager::GetStat(EStats Stat)
{
	return *(Stats.Find(Stat));
}

void UStatManager::SetStat(const EStats Stat, const int32 NewValue, bool bIsUpdate)
{
	if (Stat != EStats::VE_Empty && Stats.Contains(Stat))
	{
		int32 min = GetStat(Stat).GetMin();
		int32 max = GetStat(Stat).GetMax();
		int32 current = FMath::Clamp(NewValue, min, max);

		Stats.Add(Stat, FStatValues(min, current, max));

		if (bIsUpdate)
			UpdateStat(Stat);
	}
}

void UStatManager::ModifyStat(const EStats Stat, const int32 NewValue)
{
	if (NewValue != 0)
		SetStat(Stat, GetStat(Stat).CurrentValue + NewValue, true);
}

void UStatManager::SetupStats()
{
	TArray<EStats> Keys;
	if (Stats.GetKeys(Keys) != 0)
	{
		for (auto Key : Keys)
		{
			SetStat(Key, GetStat(Key).GetCurrent(), false);
			if (UUtils::Test(3, GetBarForStat(Key), (__FUNCTION__), __LINE__) == false) return;
			GetBarForStat(Key)->InitBar(GetStat(Key));
		}
	}
}

