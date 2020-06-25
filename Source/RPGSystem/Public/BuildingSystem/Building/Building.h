#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/BuildingData.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Structs/BuildingResourceVersion.h"
#include "BuildingSystem/Resource/Resource.h"
#include "BuildingSystem/Resource/Resource_Wood.h"
#include "BuildingSystem/Resource/Resource_Stone.h"
#include "BuildingSystem/Resource/Resource_Metal.h"
#include "BuildingSystem/Widgets/BuildWidget.h"
#include "BuildingSystem/Widgets/BuildingHealthBar.h"
#include "Components/BoxComponent.h"
#include <WidgetComponent.h>
#include "Building.generated.h"

class UBuildingStat;

UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = "Building", meta = (BlueprintSpawnableComponent))
class RPGSYSTEM_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnOverlappingBegin();

	UFUNCTION()
	void OnOverlappingEnd();

	virtual void BeginPlay() override;

	// Getter Functions
	FBuildingData GetBuildingData() { return BuildingData; }

	UFUNCTION(BlueprintCallable)
	UBuildingHealthBar* GetHealthBar() { return HealthBarWidget; }

	bool GetbBuilt() { return bBuilt; }

	bool GetInPlayerSight() { return bInPlayerSight; }

	TMap< TSubclassOf<AResource>, FBuildingResourceVersion> GetResourceVersions() { return ResourceVersions; }

	// Setter Functions
	UFUNCTION(BlueprintCallable)
	void SetResource(TSubclassOf<AResource> NewResource) { Resource = NewResource; }

	UFUNCTION(BlueprintImplementableEvent)
	void SetMesh(TSubclassOf<AResource> Resource);

	void SetCanBeBuilt(bool bCanBeBuiltNew);

	
	UFUNCTION(BlueprintCallable)
	void UpdateResource(TSubclassOf<AResource> NewResource);

	void UpdateGhostMaterial();

	void OnEnterPlayerSight();

	void OnLeavePlayerSight();

	void OnBuild(TSubclassOf<AResource> Resource);

	UFUNCTION(BlueprintCallable)
	void Building();

	void OnBuildingEnd();

	void ChangeResource(TSubclassOf<AResource> NewResource);

	bool IsOverlapping() { return bOverlapping; }

	UFUNCTION(BlueprintCallable)
	void DestroyBuilding() { Destroy(); }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building", meta = (AllowPrivateAccess = "true"))
	FBuildingData BuildingData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TSubclassOf<AResource> Resource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building", meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	TMap< TSubclassOf<AResource>, FBuildingResourceVersion> ResourceVersions;

	UMaterialInstance* MeshMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	USceneComponent* SceneComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UBuildWidget* BuildWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UBuildingHealthBar* HealthBarWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* BuildWidgetComponent;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UBuildingStat* BuildingStat;

	bool bBuilt;
	bool bCanBeBuilt;
	bool bOverlapping;
	bool bInPlayerSight = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	float BuildingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	float BuildingUpdateInterval = 0.25f;

	TSet<ABuilding*> OverlappingBuildings;

	FTimerHandle BuildingTimerHandle;

};
