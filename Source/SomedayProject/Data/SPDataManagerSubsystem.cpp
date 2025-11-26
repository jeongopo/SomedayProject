// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SPDataManagerSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/DataTable.h"

USPDataManagerSubsystem::USPDataManagerSubsystem()
{
}

void USPDataManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    LoadAllDataTables();
}

void USPDataManagerSubsystem::LoadAllDataTables()
{
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    FARFilter Filter;
    Filter.PackagePaths.Add("/Game/Data");
    Filter.ClassPaths.Add(UDataTable::StaticClass()->GetClassPathName());
    Filter.bRecursivePaths = true;

    TArray<FAssetData> AssetList;
    AssetRegistryModule.Get().GetAssets(Filter, AssetList);

    for (const FAssetData& Asset : AssetList)
    {
        UDataTable* DataTable = Cast<UDataTable>(Asset.GetAsset());
        if (DataTable)
        {
			// 데이터 테이블 이름에 따라 맵에 저장
			FString AssetName = Asset.AssetName.ToString();
			if (AssetName.Contains("MonsterData"))
			{
				DataTableMap.Add(ESPDataTableType::MonsterData, DataTable);
			}
        }
    }
}
