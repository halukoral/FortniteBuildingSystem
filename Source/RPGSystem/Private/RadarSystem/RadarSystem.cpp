// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarSystem.h"
#include "../Utils/Utils.h"

// Sets default values for this component's properties
URadarSystem::URadarSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Directions.Add(FDirectionInfo(FText::FromString("N"), 0));
	Directions.Add(FDirectionInfo(FText::FromString("E"), 90));
	Directions.Add(FDirectionInfo(FText::FromString("S"), 180));
	Directions.Add(FDirectionInfo(FText::FromString("W"), -90));
}

// Called when the game starts
void URadarSystem::BeginPlay()
{
	Super::BeginPlay();
	PlayerRef = Cast<ARPGSystemCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(PlayerRef)
		PlayerCameraRef = PlayerRef->GetFollowCamera();
}

float URadarSystem::RotToCircleDegree(float Rot)
{
	if (Rot < 0.0f)
		return FGenericPlatformMath::Fmod((Rot + 360.f), 360.f);

	return Rot;
}

float URadarSystem::GetDeltaClockwise(float A, float B, bool bClockwise)
{
	float degA = FGenericPlatformMath::Fmod(A, 360.f);
	float degB = FGenericPlatformMath::Fmod(B, 360.f);

	if (bClockwise)
	{
		if (degA > degB)
			return 360.f - (degA - degB);
		else if (degA < degB)
			return (degB - degA);
		else
			return 0.f;
	}
	else
	{
		if (degA > degB)
			return (degA - degB);
		else if (degA < degB)
			return 360.f - (degB - degA);
		else
			return 0.f;
	}
}

void URadarSystem::RotToTranslation(FRotator A, FRotator B, bool& bClockwise, bool& bInRadarSight, FVector2D& Translation)
{
	float CircleDegreeA = RotToCircleDegree(A.Yaw);
	float CircleDegreeB = RotToCircleDegree(B.Yaw);

	float DeltaClockwise = GetDeltaClockwise(CircleDegreeA, CircleDegreeB, true);
	float Delta = GetDeltaClockwise(CircleDegreeA, CircleDegreeB, false);

	if (DeltaClockwise > Delta)
	{
		if (Delta / 90.f <= 1.0f)
		{
			bClockwise = false;
			bInRadarSight = true;
			Translation = FVector2D(
				-1.f * FMath::Lerp(0.f, MaxWidgetTranslation, Delta / 90.f),
				0.f
			);
		}
		else
		{
			bClockwise = false;
			bInRadarSight = false;
			Translation = FVector2D(0.f, 0.f);
		}
	}
	else if (DeltaClockwise <= Delta)
	{
		if (DeltaClockwise / 90.f <= 1.f)
		{
			bClockwise = true;
			bInRadarSight = true;
			Translation = FVector2D(
				FMath::Lerp(0.f, MaxWidgetTranslation, DeltaClockwise / 90.f),
				0.f
			);
		}
		else
		{
			bClockwise = true;
			bInRadarSight = false;
			Translation = FVector2D(0.f, 0.f);
		}
	}
}

int32 URadarSystem::DistanceToPlayer(AActor* Actor)
{
	if (UUtils::Test(3, PlayerRef, (__FUNCTION__), __LINE__) == false) return 0;

	float DistanceX = Actor->GetActorLocation().X - PlayerRef->GetActorLocation().X;
	float DistanceY = Actor->GetActorLocation().Y - PlayerRef->GetActorLocation().Y;

	FVector Distance = FVector(DistanceX, DistanceY, 0.f);
	return Distance.Size() / UnitsPerMeter;
}

void URadarSystem::OnBecomeVisible()
{
	UpdateDirectionWidgets();
	UpdateQuestDistance();
	UpdateQuestPosition();
	UpdateLandmarkPositions();
	UpdateDistancesForEnemies();
	UpdatePositionsForEnemies();
	UpdateQuestElevation();
}

void URadarSystem::SetVisibility(bool bIsVisible, bool bCanAnimate)
{
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__) == false) return;

	if (bIsVisible != bVisible && bCanChangeVisibility)
	{
		bCanChangeVisibility = false;
		bVisible = bIsVisible;
		if (bCanAnimate)
		{
			FTimerHandle    handle;
			if (bVisible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				VisibleAnimation();
				GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
					bCanChangeVisibility = true;
				}, DelayTime, false);
			}
			else
			{
				HideAnimation();
				// We can delay just like BP DELAY function
				GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
					RadarWidget->SetVisibility(ESlateVisibility::Hidden);
					bCanChangeVisibility = true;
				}, DelayTime, false);
			}
		}
		else
		{
			if (bVisible)
			{
				OnBecomeVisible();
				RadarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				RadarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			bCanChangeVisibility = true;
		}
	}
}

void URadarSystem::ToggleVisibility(bool bCanAnimate)
{
	SetVisibility(!bVisible, bCanAnimate);
}

void URadarSystem::OnPlayerMoved()
{
	if (bVisible)
	{
		UpdateQuestDistance();
		UpdateQuestPosition();
		UpdateLandmarkPositions();
		UpdateDistancesForEnemies();
		UpdatePositionsForEnemies();
		UpdateQuestElevation(); 
	}
}

void URadarSystem::OnPlayerTurned()
{
	if (bVisible)
	{
		UpdateDirectionWidgets();
		UpdateQuestPosition();
		UpdateLandmarkPositions();
		UpdatePositionsForEnemies();
	}
}

void URadarSystem::AddDirections()
{
	for(FDirectionInfo Direction : Directions)
	{
		if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
			RadarWidget->AddDirectionWidget(Direction);
	}
}

void URadarSystem::AddQuest(FMarkerInfo Info)
{
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
	{
		QuestMarkers.Add(Info);
		RadarWidget->AddQuestWidget(Info.GetQuestType());
	}
}

void URadarSystem::AddLandmarks()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandmark::StaticClass(), FoundActors);
	for (auto Actor : FoundActors)
	{
		ALandmark* Landmark = Cast<ALandmark>(Actor);
		if (!Landmarks.Contains(Landmark))
		{
			Landmarks.Add(Landmark);
			if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
				RadarWidget->AddLandmarkWidget(Landmark->GetIconRadar());
		}
	}
}

bool URadarSystem::AddEnemy(AActor* Actor)
{
	AEnemy* _Enemy = Cast<AEnemy>(Actor);
	if (_Enemy != nullptr)
	{
		if (!Enemies.Contains(_Enemy))
		{
			Enemies.Add(_Enemy);
			if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
			{
				RadarWidget->AddEnemyWidget();
				UpdatePositionForEnemy(_Enemy);
				UpdateDistanceForEnemy(_Enemy);
				return true;
			}
			// TO DO: Call InitRadar if radarwidget Check fails 
			// Because When the game begins Player already in sight of enemy
		}
		return false;
	}
	return false;
}

void URadarSystem::RemoveEnemy(AEnemy* Enemy)
{
	int32 EnemyIndex = Enemies.Find(Enemy);
	if (EnemyIndex >= 0)
	{
		Enemies.Remove(Enemy);
		if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
			RadarWidget->DeleteEnemy(EnemyIndex);
	}
}

void URadarSystem::OnEnemyMoved(AEnemy* Enemy)
{
	UpdateDistanceForEnemy(Enemy);
	UpdatePositionForEnemy(Enemy);
}

void URadarSystem::UpdateQuestDistance()
{
	int i = 0;
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
	{
		for (UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			FVector QuestLocation = QuestMarkers[i].GetQuestLocation();
			FVector PlayerLocation = PlayerRef->GetActorLocation();
			FVector DifferenceLocation = QuestLocation - PlayerLocation;
			DifferenceLocation.Z = 0.f;
			float DifferenceLength = DifferenceLocation.Size();
			QuestWidget->UpdateDistance(DifferenceLength / UnitsPerMeter);
			++i;
		}
	}
}

void URadarSystem::UpdateQuestPosition()
{
	int i = 0;
	if (UUtils::Test(4, PlayerRef, RadarWidget, (__FUNCTION__), __LINE__))
	{
		for (UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			FRotator CameraRotation = PlayerCameraRef->GetComponentRotation();
			FVector Start = PlayerRef->GetActorLocation();
			FVector Target = QuestMarkers[i].GetQuestLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);
			
			if (bHideQuestMarkerWhenOutOfSight)
			{
				if (IsInRadarSight)
				{
					QuestWidget->SetRenderTranslation(TranslationResult);
					if (!QuestWidget->IsVisible())
						QuestWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				else
				{
					if (QuestWidget->IsVisible())
						QuestWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			else
			{
				if (IsInRadarSight)
				{
					QuestWidget->SetRenderTranslation(TranslationResult);
				}
				else
				{
					if (IsClockwise)
						QuestWidget->SetRenderTranslation(FVector2D(MaxWidgetTranslation, 0.f));
					else
						QuestWidget->SetRenderTranslation(FVector2D(MaxWidgetTranslation * -1, 0.f));
				}
			}
			++i;
		}
	}
}

void URadarSystem::UpdateQuestElevation()
{
	int i = 0;
	if (UUtils::Test(4, PlayerRef, RadarWidget, (__FUNCTION__), __LINE__))
	{
		for (UQuestWidget* QuestWidget : RadarWidget->GetQuestWidgets())
		{
			float QuestElevation = QuestMarkers[i].GetQuestLocation().Z;
			float PlayerElevation = PlayerRef->GetActorLocation().Z;

			if (QuestElevation > PlayerElevation + DefaultElevationRange)
			{
				QuestWidget->SetArrowDirection(true);
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else if (QuestElevation < PlayerElevation - DefaultElevationRange)
			{
				QuestWidget->SetArrowDirection(false);
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				QuestWidget->GetArrow()->SetVisibility(ESlateVisibility::Hidden);
			}
			++i;
		}
	}
}

void URadarSystem::OnEnterSecretRange(ASecretItem* SecretItem)
{
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__) == false) return;

	if (!SecretItems.Contains(SecretItem))
	{
		SecretItems.Add(SecretItem);
		if (SecretItems.Num() >= 1)
			RadarWidget->HighlightRadar(true);
	}
}

void URadarSystem::OnLeaveSecretRange(ASecretItem* SecretItem)
{
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__) == false) return;

	if (SecretItems.Contains(SecretItem))
	{
		SecretItems.Remove(SecretItem);
		if (SecretItems.Num() == 0)
			RadarWidget->HighlightRadar(false);
	}
}

void URadarSystem::UpdatePositionForEnemy(AEnemy* Enemy)
{
	int32 EnemyIndex = Enemies.Find(Enemy);
	if (EnemyIndex >= 0)
	{
		FRotator CameraRotation = PlayerCameraRef->GetComponentRotation();
		FVector Start = PlayerRef->GetActorLocation();
		FVector Target = Enemy->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
		bool IsClockwise;
		bool IsInRadarSight;
		FVector2D TranslationResult;
		RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);
		
		if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
		{
			UEnemyWidget* EnemyWidget = RadarWidget->GetEnemyWidgets()[EnemyIndex];
			if (IsInRadarSight)
			{
				EnemyWidget->SetRenderTranslation(TranslationResult);
				if (!EnemyWidget->IsVisible())
					EnemyWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if (EnemyWidget->IsVisible())
					EnemyWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void URadarSystem::UpdateDistanceForEnemy(AEnemy* Enemy)
{
	int32 EnemyIndex = Enemies.Find(Enemy);
	if (EnemyIndex >= 0)
	{
		if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
		{
			UEnemyWidget* EnemyWidget = RadarWidget->GetEnemyWidgets()[EnemyIndex];
			if (EnemyWidget != nullptr) 
			{
				EnemyWidget->UpdateDistance(DistanceToPlayer(Enemy));
			}
		}
	}
}

void URadarSystem::UpdatePositionsForEnemies()
{
	for (auto Enemy : Enemies)
		UpdatePositionForEnemy(Enemy);
}

void URadarSystem::UpdateDistancesForEnemies()
{
	for (auto Enemy : Enemies)
		UpdateDistanceForEnemy(Enemy);
}

void URadarSystem::UpdateLandmarkPositions()
{
	int i = 0;
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
	{
		for (ULandmarkWidget* LandmarkWidget : RadarWidget->GetLandmarkWidgets())
		{
			FRotator CameraRotation = PlayerCameraRef->GetComponentRotation();
			FVector Start = PlayerRef->GetActorLocation();
			FVector Target = Landmarks[i]->GetActorLocation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, LookAtRotation, IsClockwise, IsInRadarSight, TranslationResult);
			if (IsInRadarSight)
			{
				LandmarkWidget->SetRenderTranslation(TranslationResult);
				if (!LandmarkWidget->IsVisible())
					LandmarkWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if (LandmarkWidget->IsVisible())
					LandmarkWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			++i;
		}
	}
}

void URadarSystem::UpdateDirectionWidgets()
{
	int i = 0;
	if (UUtils::Test(3, RadarWidget, (__FUNCTION__), __LINE__))
	{
		for (UDirectionWidget* DirectionWidget : RadarWidget->GetDirectionWidgets())
		{
			FRotator CameraRotation = PlayerCameraRef->GetComponentRotation();
			float DirectionYaw = Directions[i].GetWorldRot();
			bool IsClockwise;
			bool IsInRadarSight;
			FVector2D TranslationResult;
			RotToTranslation(CameraRotation, FRotator(0.f, DirectionYaw, 0.f), IsClockwise, IsInRadarSight, TranslationResult);
			if (IsInRadarSight)
			{
				DirectionWidget->SetRenderTranslation(TranslationResult);
				if (!DirectionWidget->IsVisible())
					DirectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			}
			else
			{
				if (DirectionWidget->IsVisible())
					DirectionWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			++i;
		}
	}
}

