#include "ExperienceWidget.h"
#include "ExperienceComponent.h"
#include "Utils/Utils.h"
#include "Kismet/GameplayStatics.h"
#include "RPGSystemCharacter.h"

void UExperienceWidget::UpdateExp()
{
	if (UUtils::Test(3, ExpComponent, (__FUNCTION__), __LINE__) == false) return;
	ExpBar->SetPercent((float)ExpComponent->iCurrentExp / ExpComponent->iExpForNextLevel);
	ExpText->SetText(FText::Format(INVTEXT("{0} | {1}"), FText::AsNumber(ExpComponent->iCurrentExp), FText::AsNumber(ExpComponent->iExpForNextLevel)));
}

void UExperienceWidget::UpdateLevel()
{
	if (UUtils::Test(3, ExpComponent, (__FUNCTION__), __LINE__) == false) return;
	LevelText->SetText(FText::Format(INVTEXT("{0}"), FText::AsNumber(ExpComponent->iCurrentLevel)));
}
