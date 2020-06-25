#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/BuildingData.h"
#include "BuildingSystem/Widgets/BuildingWidget.h"
#include "BuildingSystem/Resource/ResourceManager.h"
#include "Public/TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Building.h"
#include "BuildingManager.generated.h"

class ARPGSystemCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPGSYSTEM_API UBuildingManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuildingManager();
	virtual void BeginPlay() override;
	
	FBuildingData GetCurrentBuildingData();

	UFUNCTION(BlueprintCallable)
	void SelectBuildingByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SelectCurrentBuilding(bool bIsSelect);

	/** Projecting any location in the world onto a grid */
	FVector ToGridLocation(FVector In);

	/** Buildings are always rotated so that they are facing
	 *  the same rotation as our player however it snaps 90 degrees
	 *  so you are not able to reach any rotations in between */
	int32 ToSnapRotation(float In);

	FVector GetPlayerLookAtLocation(float ForwardOffset);

	void GetBuildingPositionAndRotation(FVector& Loc, FRotator& Rot);

	void OnStartBuilding();

	void OnStopBuilding();

	void StartBuilding();

	void ChangeCurrentBuilding();

	void OnResourceChanged();

	void OnResourceValueModified(TSubclassOf<AResource> Resource);
		
	UFUNCTION(BlueprintCallable)
	void PlaceCurrentBuilding();

	UFUNCTION(BlueprintCallable)
	void Building();

	bool CanBuilding();
	bool CanBuilding(TSubclassOf<AResource>& Resource, int32& Amount);

	UFUNCTION(BlueprintCallable)
	TArray< TSubclassOf<ABuilding> > GetBuildings() { return Buildings; }

	int32 GetForwardBuildingOffset() { return ForwardBuildingOffset; }

	FVector GetLocationOffset() { return LocationOffset; }

	/** Get a widget for specified building so that we can highlight the widget 
	 *  of the building that's currently selected */
	bool HighlightSelectedWidget(TSubclassOf<ABuilding> Building, UBuildingWidget*& BuildingWidget);

private:
	/** List of the buildings that the player can build */
	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	TArray< TSubclassOf<ABuilding> > Buildings;

	UPROPERTY(BlueprintReadOnly, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UResourceManager* ResourceManager = nullptr;

	/** Building Widgets that we need access to in order to update them */
	UPROPERTY(BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	TArray<UBuildingWidget*> BuildingWidgets;

	UPROPERTY(BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	ARPGSystemCharacter* Player;

	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 XGridSize = 400;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 YGridSize = 400;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 ZGridSize = 400;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 RotationSnap = 90;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 ForwardBuildingOffset = 350;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	int32 RotationOffset = 90;

	UPROPERTY(EditAnywhere, Category = "Build Data| Grid")
	FVector LocationOffset = FVector(0.f, 0.f, 30.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	ABuilding* CurrentBuilding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	float BuildingUpdateInterval = 0.3f;

	bool bIsCurrentlyBuilding;

	int32 SelectedBuildingIndex;

	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;

	FTimerHandle BuildingTimerHandle;
};
