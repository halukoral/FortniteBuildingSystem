// Fill out your copyright notice in the Description page of Project Settings.

#include "Building.h"
#include "BuildingStat.h"


// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BuildingMesh->SetupAttachment(SceneComponent);
	BuildingMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = SceneComponent;

	ResourceVersions.Add(AResource_Metal::StaticClass(), FBuildingResourceVersion(30, 300, 2.f));
	ResourceVersions.Add(AResource_Stone::StaticClass(), FBuildingResourceVersion(20, 200, 1.5f));
	ResourceVersions.Add(AResource_Wood::StaticClass(), FBuildingResourceVersion(10, 100, 1.f));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Box->SetBoxExtent(FVector(90.f, 90.f, 90.f));
	Box->SetCollisionProfileName("OverlapAll");
	Box->SetupAttachment(SceneComponent);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABuilding::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ABuilding::OnOverlapEnd);

	BuildingStat = CreateDefaultSubobject<UBuildingStat>(TEXT("Stat"));
	BuildingStat->AddStat(EStats::VE_Health);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	
	if (UUtils::Test(4, BuildWidgetComponent, HealthBarWidgetComponent,(__FUNCTION__), __LINE__) == false) return;

	DynamicMaterialInstance = BuildingMesh->CreateDynamicMaterialInstance(0, BuildingMesh->GetMaterial(0));
	UpdateGhostMaterial();
	BuildWidget = Cast<UBuildWidget>(BuildWidgetComponent->GetUserWidgetObject());
	HealthBarWidget = Cast<UBuildingHealthBar>(HealthBarWidgetComponent->GetUserWidgetObject());

	if (UUtils::Test(8, Resource, BuildWidgetComponent, BuildWidget, HealthBarWidgetComponent, HealthBarWidget, BuildingMesh, (__FUNCTION__), __LINE__) == false) return;


	FBuildingResourceVersion* VersionInfo = ResourceVersions.Find(Resource);
	BuildWidget->Update(Resource, *VersionInfo);

	BuildingStat->InitManager(this);
	BuildWidgetComponent->SetVisibility(true, false);


}

void ABuilding::UpdateGhostMaterial()
{
	if (UUtils::Test(3, Resource, (__FUNCTION__), __LINE__) == false) return;
	DynamicMaterialInstance->SetVectorParameterValue(FName(TEXT("Color")), Resource->GetDefaultObject<AResource>()->GetResourceData().GetColor());
}

void ABuilding::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building != nullptr && Building->GetbBuilt())
			{
				OverlappingBuildings.Add(Building);
				if (!bOverlapping)
					OnOverlappingBegin();
			}
		}
	}
}

void ABuilding::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(OtherComp);
		if (Mesh != nullptr)
		{
			ABuilding* Building = Cast<ABuilding>(OtherActor);
			if (Building && Building->GetbBuilt())
			{
				if (OverlappingBuildings.Contains(Building))
				{
					OverlappingBuildings.Remove(Building);
					if (OverlappingBuildings.Array().Num() <= 0)
						OnOverlappingEnd();

				}
			}
		}
	}
}

void ABuilding::OnOverlappingBegin()
{
	if (UUtils::Test(4, BuildWidgetComponent, BuildingMesh, (__FUNCTION__), __LINE__) == false) return;

	bOverlapping = true;
	BuildWidgetComponent->SetVisibility(false, false);
	BuildingMesh->SetVisibility(false, false);
}

void ABuilding::OnOverlappingEnd()
{
	if (UUtils::Test(4, BuildWidgetComponent, BuildingMesh, (__FUNCTION__), __LINE__) == false) return;

	bOverlapping = false;
	BuildWidgetComponent->SetVisibility(true, false);
	BuildingMesh->SetVisibility(true, false);
}

void ABuilding::UpdateResource(TSubclassOf<AResource> NewResource)
{
	if (UUtils::Test(3, BuildWidget, (__FUNCTION__), __LINE__) == false) return;

	if (UKismetSystemLibrary::IsValidClass(NewResource) && NewResource != Resource)
	{
		Resource = NewResource;
		UpdateGhostMaterial();
		BuildWidget->Update(Resource, *ResourceVersions.Find(Resource));
	}
}

void ABuilding::SetCanBeBuilt(bool bCanBeBuiltNew)
{
	if (UUtils::Test(3, BuildWidget, (__FUNCTION__), __LINE__) == false) return;

	bCanBeBuilt = bCanBeBuiltNew;
	BuildWidget->SetCanBeBuilt(bCanBeBuilt);
}

void ABuilding::OnEnterPlayerSight()
{
	if (UUtils::Test(5, HealthBarWidget, HealthBarWidgetComponent, BuildingStat, (__FUNCTION__), __LINE__) == false) return;

	bInPlayerSight = true;
	if (bBuilt)
	{
		HealthBarWidget->UpdateHealth(BuildingStat->GetStat(EStats::VE_Health).GetCurrent());
		HealthBarWidgetComponent->SetVisibility(true, false);
	}
}

void ABuilding::OnLeavePlayerSight()
{
	if (UUtils::Test(3, HealthBarWidgetComponent, (__FUNCTION__), __LINE__) == false) return;

	bInPlayerSight = false;
	if (bBuilt)
		HealthBarWidgetComponent->SetVisibility(false, false);
}

void ABuilding::OnBuild(TSubclassOf<AResource> Resource)
{
	if (UUtils::Test(8, BuildingMesh, BuildWidgetComponent, BuildingStat, HealthBarWidget, HealthBarWidgetComponent, Resource, (__FUNCTION__), __LINE__) == false) return;

	bBuilt = true;
	BuildingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BuildingMesh->SetMaterial(0, Resource->GetDefaultObject<AResource>()->GetResourceData().GetMeshMaterial());
	DynamicMaterialInstance = BuildingMesh->CreateDynamicMaterialInstance(0, BuildingMesh->GetMaterial(0));

	BuildWidgetComponent->DestroyComponent();

	BuildingStat->SetMax(EStats::VE_Health, ResourceVersions.Find(Resource)->GetMaxHealth(), false);
	BuildingStat->SetStat(EStats::VE_Health, 0, false);
	BuildingTime = ResourceVersions.Find(Resource)->GetBuildingTime();
	
	HealthBarWidget->InitBar(BuildingStat->GetStat(EStats::VE_Health).GetMax());
	HealthBarWidgetComponent->SetVisibility(true, false);

	GetWorld()->GetTimerManager().SetTimer(BuildingTimerHandle, this, &ABuilding::Building, BuildingUpdateInterval, true);

	//FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, [this]() { int32 Temp = BuildingTime; }, BuildingTime + BuildingUpdateInterval, false);
	//OnBuildingEnd();
}

void ABuilding::Building()
{
	if (UUtils::Test(3, BuildingStat, (__FUNCTION__), __LINE__) == false) return;

	int32 Value = FGenericPlatformMath::CeilToFloat(BuildingStat->GetStat(EStats::VE_Health).GetMax() / FMath::FloorToInt(BuildingTime + BuildingUpdateInterval) );
	
	BuildingStat->ModifyStat(EStats::VE_Health, Value);

	if (BuildingStat->GetStat(EStats::VE_Health).GetCurrent() == BuildingStat->GetStat(EStats::VE_Health).GetMax())
		OnBuildingEnd();
}

void ABuilding::OnBuildingEnd()
{
	if (UUtils::Test(3, HealthBarWidget, (__FUNCTION__), __LINE__) == false) return;

	GetWorld()->GetTimerManager().ClearTimer(BuildingTimerHandle);
	HealthBarWidget->SetState(EBuildingState::VE_Built);
}

void ABuilding::ChangeResource(TSubclassOf<AResource> NewResource)
{
	if (UUtils::Test(3, NewResource, (__FUNCTION__), __LINE__) == false) return;

	if (Resource != NewResource)
	{
		Resource = NewResource;
		UpdateGhostMaterial();
		BuildWidget->Update(Resource, *ResourceVersions.Find(Resource));
	}
}
