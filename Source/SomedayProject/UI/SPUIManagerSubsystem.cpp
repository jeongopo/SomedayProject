// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPUIManagerSubsystem.h"
#include "SPUIManagerSubsystem.h"
#include "Hero/SPHeroCharacter.h"
#include "Core/SPDefaultData.h"
#include "Blueprint/UserWidget.h"
#include "SomedayProjectGameMode.h"
#include "UI/SPUIDataTable.h"
#include "SPLogHelper.h"

USPUIManagerSubsystem::USPUIManagerSubsystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Game/UI/UITable"));
	if (DataTableAsset.Succeeded())
	{
		UIDataTable = DataTableAsset.Object;
	}
}

void USPUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USPUIManagerSubsystem::InitUIData()
{
	if (!UIDataTable)
	{
		UE_LOG(LogSPDefault, Warning, TEXT("UI DataTable is null in UIManagerSubsystem"));
		return;
	}
	
	for (const auto& Row : UIDataTable->GetRowMap())
	{
		FSPUIDataTable* UIData = (FSPUIDataTable*)Row.Value;
		if (UIData && UIData->WidgetClass)
		{
			UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), UIData->WidgetClass);
			if (WidgetInstance)
			{
				WidgetMap.Add(UIData->WidgetKey, WidgetInstance);
				if (UIData->bShowByDefault)
				{
					WidgetInstance->AddToViewport(UIData->ZOrder);
				}
			}
		}
	}
}
