// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingStat.h"




UBuildingStat::UBuildingStat()
{
	Stats.Add(EStats::VE_Health, FStatValues());
}

void UBuildingStat::InitManager(ABuilding* InitBuilding)
{
	if (UUtils::Test(3, InitBuilding, (__FUNCTION__), __LINE__) == false) return;
	Building = InitBuilding;
	SetStat(EStats::VE_Health, GetStat(EStats::VE_Health).GetCurrent(), false);
}

void UBuildingStat::SetMax(EStats Stat, int32 NewMax, bool bUpdate)
{
	TArray<EStats> Key;
	Stats.GetKeys(Key);

	if (Key.Contains(Stat) && Stat != EStats::VE_Empty)
	{
		int32 Current = FMath::Clamp(GetStat(Stat).GetCurrent(), GetStat(Stat).GetMin(), NewMax);

		Stats.Add(Stat, FStatValues(GetStat(Stat).GetMin(), Current, NewMax));

		if (bUpdate)
			UpdateStat(Stat);
	}
}

void UBuildingStat::UpdateStat(EStats Stat)
{
	if (UUtils::Test(3, Building, (__FUNCTION__), __LINE__) == false) return;

	if (Stat == EStats::VE_Health && Building->GetInPlayerSight())
		Building->GetHealthBar()->UpdateHealth(GetStat(EStats::VE_Health).GetCurrent());
}
