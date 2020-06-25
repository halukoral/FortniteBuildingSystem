#include "BuildingManager.h"
#include "../Player/RPGSystemCharacter.h"


UBuildingManager::UBuildingManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBuildingManager::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ARPGSystemCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCamera = Player->GetFollowCamera();
	ResourceManager = GetOwner()->FindComponentByClass<UResourceManager>();
	UUtils::Test(5, Player, PlayerCamera, ResourceManager, (__FUNCTION__), __LINE__);
}

FBuildingData UBuildingManager::GetCurrentBuildingData()
{
	return Buildings[SelectedBuildingIndex]->GetDefaultObject<ABuilding>()->GetBuildingData();
}

void UBuildingManager::SelectBuildingByIndex(int32 Index)
{
	if (Index >= 0 && Index < Buildings.Num())
	{
		if (UUtils::Test(3, Player, (__FUNCTION__), __LINE__) == false) return;

		if (Player->GetPlayerStates() == EPlayerStates::VE_Combat)
		{
			SelectedBuildingIndex = Index;
			Player->ChangeState(EPlayerStates::VE_Construction);
		}
		else
		{
			if (Index != SelectedBuildingIndex)
			{
				SelectCurrentBuilding(false);
				SelectedBuildingIndex = Index;
				SelectCurrentBuilding(true);
			}
		}
	}
}

void UBuildingManager::SelectCurrentBuilding(bool bIsSelect)
{
	UBuildingWidget* Widget = nullptr;
	if (HighlightSelectedWidget(Buildings[SelectedBuildingIndex], Widget))
	{
		if (UUtils::Test(3, Widget, (__FUNCTION__), __LINE__) == false) return;

		Widget->SelectBuilding(bIsSelect);
		if (bIsSelect)
			OnStartBuilding();
	}
}

FVector UBuildingManager::ToGridLocation(FVector In)
{
	return FVector(
		FMath::RoundToInt(In.X / XGridSize) * XGridSize,
		FMath::RoundToInt(In.Y / YGridSize) * YGridSize,
		FMath::RoundToInt(In.Z / ZGridSize) * ZGridSize
	);
}

int32 UBuildingManager::ToSnapRotation(float In)
{
	return FMath::RoundToInt(In / RotationSnap) * RotationSnap;
}

FVector UBuildingManager::GetPlayerLookAtLocation(float ForwardOffset)
{
	if (UUtils::Test(4, Player, PlayerCamera, (__FUNCTION__), __LINE__) == false) return FVector::ZeroVector;

	return Player->GetActorLocation() + (PlayerCamera->GetForwardVector() * ForwardOffset);
}

void UBuildingManager::GetBuildingPositionAndRotation(FVector& Loc, FRotator& Rot)
{
	if (UUtils::Test(3, PlayerCamera, (__FUNCTION__), __LINE__) == false) return;

	FVector PlayerLookAt = GetPlayerLookAtLocation(ForwardBuildingOffset);
	Loc = ToGridLocation(PlayerLookAt) + LocationOffset;

	Rot.Yaw = ToSnapRotation(PlayerCamera->GetComponentRotation().Yaw) + RotationOffset;
}

void UBuildingManager::OnStartBuilding()
{
	if (bIsCurrentlyBuilding)
		ChangeCurrentBuilding();
	else
		StartBuilding();
	bIsCurrentlyBuilding = true;
}

void UBuildingManager::StartBuilding()
{
	GetBuildingPositionAndRotation(Location, Rotation);
	CurrentBuilding = GetWorld()->SpawnActorDeferred<ABuilding>(Buildings[SelectedBuildingIndex], FTransform(Rotation, Location), GetOwner(), GetOwner()->GetInstigator());
	if (CurrentBuilding)
	{
		CurrentBuilding->SetResource(ResourceManager->GetSelectedResource());
		CurrentBuilding->SetMesh(ResourceManager->GetSelectedResource());
		UGameplayStatics::FinishSpawningActor(CurrentBuilding, FTransform(Rotation, Location));
	}

	if (UUtils::Test(4, CurrentBuilding, ResourceManager, (__FUNCTION__), __LINE__) == false) return;

	CurrentBuilding->SetCanBeBuilt(CanBuilding());

	GetWorld()->GetTimerManager().SetTimer(BuildingTimerHandle, this, &UBuildingManager::Building, BuildingUpdateInterval, true);
}

void UBuildingManager::OnStopBuilding()
{
	if (bIsCurrentlyBuilding)
	{
		if (UUtils::Test(3, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return;
		bIsCurrentlyBuilding = false;
		SelectCurrentBuilding(false);
		if (IsValid(CurrentBuilding))
		{
			GetWorld()->GetTimerManager().ClearTimer(BuildingTimerHandle);
			CurrentBuilding->Destroy();
			CurrentBuilding = nullptr;
		}
	}
}

void UBuildingManager::ChangeCurrentBuilding()
{
	if (UUtils::Test(3, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return;
	GetWorld()->GetTimerManager().ClearTimer(BuildingTimerHandle);
	if (IsValid(CurrentBuilding))
	{
		CurrentBuilding->Destroy();
		CurrentBuilding = nullptr;
		StartBuilding();
	}
}

void UBuildingManager::OnResourceChanged()
{
	if (UUtils::Test(3, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return;
	if (bIsCurrentlyBuilding)
	{
		CurrentBuilding->ChangeResource(ResourceManager->GetSelectedResource());
		CurrentBuilding->SetCanBeBuilt(CanBuilding());
		CurrentBuilding->SetMesh(ResourceManager->GetSelectedResource());
	}
}

void UBuildingManager::OnResourceValueModified(TSubclassOf<AResource> Resource)
{
	if (UUtils::Test(5, Resource, ResourceManager, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return;

	if (Resource == ResourceManager->GetSelectedResource() && bIsCurrentlyBuilding)
		CurrentBuilding->SetCanBeBuilt(CanBuilding());
}

void UBuildingManager::PlaceCurrentBuilding()
{
	if (UUtils::Test(5, Player, CurrentBuilding, ResourceManager, (__FUNCTION__), __LINE__) == false) return;

	TSubclassOf<AResource> _Resource;
	int32 _Amount = 0;
	bool _bCanBuild = CanBuilding(_Resource, _Amount);

	if (!CurrentBuilding->IsOverlapping() && _bCanBuild)
	{
		if (IsValid(Player->GetCurrentlySeenBuilding()))
		{
			if(CurrentBuilding != Player->GetCurrentlySeenBuilding())
				Player->GetCurrentlySeenBuilding()->OnLeavePlayerSight();
		}
		Player->SetCurrentlySeenBuilding(CurrentBuilding);
		ResourceManager->RemoveResource(_Resource, _Amount);
		GetWorld()->GetTimerManager().ClearTimer(BuildingTimerHandle);
		CurrentBuilding->OnBuild(_Resource);
		StartBuilding();
	}

}

void UBuildingManager::Building()
{
	if (UUtils::Test(3, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return;
	GetBuildingPositionAndRotation(Location, Rotation);
	CurrentBuilding->SetActorLocationAndRotation(Location, Rotation);
}

bool UBuildingManager::CanBuilding()
{
	if (UUtils::Test(4, ResourceManager, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return false;
	
	FBuildingResourceVersion* Version = CurrentBuilding->GetResourceVersions().Find(ResourceManager->GetSelectedResource());
	int32 _Amount = ResourceManager->GetResourceAmount(ResourceManager->GetSelectedResource());

	return _Amount >= Version->GetRequiredAmount();
}

bool UBuildingManager::CanBuilding(TSubclassOf<AResource>& Resource, int32& Amount)
{
	if (UUtils::Test(4, ResourceManager, CurrentBuilding, (__FUNCTION__), __LINE__) == false) return false;
	
	FBuildingResourceVersion* Version = CurrentBuilding->GetResourceVersions().Find(ResourceManager->GetSelectedResource());
	int32 _Amount = ResourceManager->GetResourceAmount(ResourceManager->GetSelectedResource());

	Resource = ResourceManager->GetSelectedResource();
	Amount = Version->GetRequiredAmount();
	return _Amount >= Version->GetRequiredAmount();
}

bool UBuildingManager::HighlightSelectedWidget(TSubclassOf<ABuilding> Building, UBuildingWidget*& BuildingWidget)
{
	for (auto Widget : BuildingWidgets)
	{
		if (UUtils::Test(3, Widget, (__FUNCTION__), __LINE__) == false) return false;

		if ( Widget->GetBuilding() == Building && IsValid(Widget) )
		{
			BuildingWidget = Widget;
			return true;
		}
	}
	return false;
}

