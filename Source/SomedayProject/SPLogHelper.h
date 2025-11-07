#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSPDefault, Log, All);

#define LOG_INFO(LogCategory, Format, ...) UE_LOG(LogCategory, Log, Format, ##__VA_ARGS__)
#define LOG_WARNING(LogCategory, Format, ...) UE_LOG(LogCategory, Warning, Format, ##__VA_ARGS__)
#define LOG_ERROR(LogCategory, Format, ...) UE_LOG(LogCategory, Error, Format, ##__VA_ARGS__)

