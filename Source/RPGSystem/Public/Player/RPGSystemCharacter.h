// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/Building/BuildingManager.h"
#include "BuildingSystem/Resource/ResourceManager.h"
#include "Utils/Utils.h"
#include "UI/MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Character.h"
#include "BuildingSystem/Building/Building.h"
#include "RPGSystemCharacter.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EPlayerStates : uint8
{
	VE_Combat 		UMETA(DisplayName = "Combat"),
	VE_Construction UMETA(DisplayName = "Construction"),
};

UCLASS(config=Game)
class ARPGSystemCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	ARPGSystemCharacter();

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location) { Jump(); }

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location) { StopJumping(); }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// Getter Functions
	UFUNCTION(BlueprintCallable)
	UMainWidget* GetMainWidget() { return MainWidget; }

	UFUNCTION(BlueprintCallable)
	EPlayerStates GetPlayerStates() { return PlayerStates; }

	UFUNCTION(BlueprintCallable)
	UResourceManager* GetResourceManager() { return ResourceManager; }

	UFUNCTION(BlueprintCallable)
	ABuilding* GetCurrentlySeenBuilding() { return CurrentlySeenBuilding; }

	// Setter Functions
	UFUNCTION(BlueprintCallable)
	void SetCurrentlySeenBuilding(ABuilding* NewBuilding) { CurrentlySeenBuilding = NewBuilding; }

	UFUNCTION(BlueprintCallable)
	FTwoVectors  GetLineTracePoints() const;

	/* Switch Player States between Construction and Combat	*/
	UFUNCTION(BlueprintCallable)
	void ChangeState(EPlayerStates NewState);

	/* Change Character HUD when Player switch states*/
	UFUNCTION(BlueprintCallable)
	void UpdateStateDisplay();

	UFUNCTION(BlueprintCallable)
	void SightTrace();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player State", meta = (AllowPrivateAccess = "true"))
	EPlayerStates PlayerStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UBuildingManager* BuildingManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	UResourceManager* ResourceManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Build Data", meta = (AllowPrivateAccess = "true"))
	ABuilding* CurrentlySeenBuilding;

	// UI Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	UMainWidget* MainWidget;

	UPROPERTY()
	UTexture2D* CrossHairCom;
	
	UPROPERTY()
	UTexture2D* CrossHairConst;

	UPROPERTY()
	UTexture2D* IconCombat;

	UPROPERTY()
	UTexture2D* IconConstruct;

	UPROPERTY()
	FTimerHandle Handle;
};

