#pragma once
#include "StringFunctionLibrary.h"

#define DDD_LOG(CategoryName, Verbosity, Format, ...) \
		UE_LOG(CategoryName, Verbosity, TEXT("[Frame: %04llu] %s"), GFrameCounter, *FString::Printf(Format, ##__VA_ARGS__))

#define DDD_FUNC_LOG(CategoryName, Verbosity, Format, ...) \
		UE_LOG(CategoryName, Verbosity, TEXT("[Frame: %04llu] %s(%d): %s"), GFrameCounter, ANSI_TO_TCHAR(__FUNCTION__), __LINE__, *FString::Printf(Format, ##__VA_ARGS__))

#define DDD_NET_LOG(CategoryName, Verbosity, OwnerActor, Format, ...) \
		UE_LOG(CategoryName, Verbosity, TEXT("[Frame: %04llu] <%s:%s/%s> %s(%d): %s"), \
			GFrameCounter, *GetNameSafe(OwnerActor), IsValid(OwnerActor) ? *FStringFunctionLibrary::EnumToString(OwnerActor->GetLocalRole()) : TEXT("Invalid"), IsValid(OwnerActor) ? *FStringFunctionLibrary::EnumToString(OwnerActor->GetRemoteRole()) : TEXT("Invalid"), ANSI_TO_TCHAR(__FUNCTION__), __LINE__, *FString::Printf(Format, ##__VA_ARGS__))
