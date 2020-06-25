#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/Stat/StatManager.h"
#include "BuildingStat.generated.h"

UCLASS()
class RPGSYSTEM_API UBuildingStat : public UStatManager
{
	GENERATED_BODY()

public:
	UBuildingStat();

	UFUNCTION(BlueprintCallable)
	void InitManager(ABuilding* InitBuilding);

	UFUNCTION(BlueprintCallable)
	void SetMax(EStats Stat, int32 NewValue, bool bUpdate);

	virtual void UpdateStat(EStats Stat);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	ABuilding * Building;
};
