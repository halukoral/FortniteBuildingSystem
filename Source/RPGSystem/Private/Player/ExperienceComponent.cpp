#include "ExperienceComponent.h"
#include "Player/RPGSystemCharacter.h"
#include "UI/MainWidget.h"
#include "Utils/Utils.h"

UExperienceComponent::UExperienceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UExperienceComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UExperienceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UExperienceComponent::OnLevelUp()
{
	iCurrentExp -= iExpForNextLevel;
	iExpForNextLevel *= NextExpMultiplier;
	++iCurrentLevel;

	if (UUtils::Test(3, MainWidget, (__FUNCTION__), __LINE__) == false) return;
	MainWidget->GetExpWidget()->UpdateLevel();

}

void UExperienceComponent::AddExperiencePoints(int Amount)
{
	iCurrentExp += Amount;

	if (iCurrentExp >= iExpForNextLevel)
	{
		OnLevelUp();
	}

	if (UUtils::Test(3, MainWidget, (__FUNCTION__), __LINE__) == false) return;
	MainWidget->GetExpWidget()->UpdateExp();

}

