#pragma once

class FStringFunctionLibrary
{
public:
	template <typename T>
	static inline FString EnumToString(T EnumValue)
	{
		const UEnum* EnumPtr = StaticEnum<T>();
		if (!EnumPtr)
		{
			return FString();
		}

		return EnumPtr->GetNameStringByValue((int64)EnumValue);
	}

	static inline FString BoolToString(bool bValue)
	{
		return bValue ? TEXT("true") : TEXT("false");
	}
};
