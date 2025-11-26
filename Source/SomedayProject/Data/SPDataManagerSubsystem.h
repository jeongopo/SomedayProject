// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SPDataManagerSubsystem.generated.h"

enum class ESPDataTableType : uint8
{
	None,
	MonsterData,
	MAX
};

/**
 * 
 */
UCLASS()
class SOMEDAYPROJECT_API USPDataManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	USPDataManagerSubsystem();

	void Initialize (FSubsystemCollectionBase& Collection) override;
	void LoadAllDataTables();

public:
	UDataTable* GetDataTable(ESPDataTableType TableType) const
	{
		if (DataTableMap.Contains(TableType))
		{
			return DataTableMap[TableType];
		}
		return nullptr;
	}

	template <typename T>
	T* FindDataRowFromDataTable(ESPDataTableType TableType, const FString& InKey) const
	{
		UDataTable* DataTable = GetDataTable(TableType);
		if (DataTable)
		{
			FName RowName(*InKey);
			T* DataRow = DataTable->FindRow<T>(RowName, TEXT(""));
			if (DataRow)
			{
				return DataRow;
			}
		}
		return nullptr;
	}

protected:
	TMap<ESPDataTableType, UDataTable*> DataTableMap;
};
