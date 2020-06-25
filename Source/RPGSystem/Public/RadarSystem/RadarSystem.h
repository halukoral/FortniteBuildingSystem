// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Landmark.h"
#include "../Item/SecretItem.h"
#include "../Enemy/Enemy.h"
#include "../Player/RPGSystemCharacter.h"
#include "Public/Structs/DirectionInfo.h"
#include "Public/Structs/MarkerInfo.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ActorComponent.h"
#include "Widgets/RadarWidget.h"
#include "Widgets/EnemyWidget.h"
#include "RadarSystem.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGSYSTEM_API URadarSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URadarSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//////////////////////////////////////////////////////////////////////////
	/// Helper Functions
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float RotToCircleDegree(float Rot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDeltaClockwise(float A, float B, bool bClockwise);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void RotToTranslation(FRotator A, FRotator B, bool& bClockwise, bool& bInRadarSight, FVector2D& Translation);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 DistanceToPlayer(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void OnBecomeVisible();

	UFUNCTION(BlueprintCallable)
	void SetVisibility(bool bIsVisible, bool bCanAnimate);

	UFUNCTION(BlueprintCallable)
	void ToggleVisibility(bool bCanAnimate);

	UFUNCTION(BlueprintNativeEvent)
	void VisibleAnimation();
	virtual void VisibleAnimation_Implementation() { }

	UFUNCTION(BlueprintNativeEvent)
	void HideAnimation();
	virtual void HideAnimation_Implementation() { }

	//////////////////////////////////////////////////////////////////////////
	/// Player's Functions
	UFUNCTION(BlueprintCallable)
	void OnPlayerMoved();

	UFUNCTION(BlueprintCallable)
	void OnPlayerTurned();

	//////////////////////////////////////////////////////////////////////////
	/// Landmark's Functions
	UFUNCTION(BlueprintCallable)
	void AddLandmarks();

	UFUNCTION(BlueprintCallable)
	void UpdateLandmarkPositions();

	//////////////////////////////////////////////////////////////////////////
	/// Direction's Functions
	UFUNCTION(BlueprintCallable)
	void AddDirections();

	UFUNCTION(BlueprintCallable)
	void UpdateDirectionWidgets();

	//////////////////////////////////////////////////////////////////////////
	/// Enemy's Functions
	UFUNCTION(BlueprintCallable)
	bool AddEnemy(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemoveEnemy(AEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void UpdatePositionForEnemy(AEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void UpdateDistanceForEnemy(AEnemy* Enemy);

	UFUNCTION(BlueprintCallable)
	void UpdatePositionsForEnemies();

	UFUNCTION(BlueprintCallable)
	void UpdateDistancesForEnemies();

	UFUNCTION(BlueprintCallable)
	void OnEnemyMoved(AEnemy* Enemy);

	//////////////////////////////////////////////////////////////////////////
	/// Quest's Functions
	UFUNCTION(BlueprintCallable)
	void AddQuest(FMarkerInfo Info);

	UFUNCTION(BlueprintCallable)
	void UpdateQuestDistance();

	UFUNCTION(BlueprintCallable)
	void UpdateQuestPosition();

	UFUNCTION(BlueprintCallable)
	void UpdateQuestElevation();

	//////////////////////////////////////////////////////////////////////////
	/// Secret Item's Functions
	UFUNCTION(BlueprintCallable)
	void OnEnterSecretRange(ASecretItem* SecretItem);

	UFUNCTION(BlueprintCallable)
	void OnLeaveSecretRange(ASecretItem* SecretItem);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FDirectionInfo> Directions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FMarkerInfo> QuestMarkers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FMarkerInfo> DefaultQuestMarkers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ALandmark*> Landmarks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AEnemy*> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASecretItem*> SecretItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MaxWidgetTranslation = 320.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float UnitsPerMeter = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DefaultElevationRange = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ARPGSystemCharacter* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCameraRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bHideQuestMarkerWhenOutOfSight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanChangeVisibility = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	URadarWidget* RadarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DelayTime = 1.f;
};
