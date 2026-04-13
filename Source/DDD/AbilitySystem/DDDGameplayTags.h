#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * DDD 프로젝트 Native Gameplay Tag 선언.
 * 이 헤더를 include 한 뒤 TAG_XXX 변수를 바로 사용한다.
 *
 * 사용 예:
 *   Tags.AddTag(TAG_State_Minus);
 *   if (Tags.HasTag(TAG_Bullet_Damage)) { ... }
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Minus)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Modulo)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Multiply)

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Bullet_Damage)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Bullet_Minus)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Bullet_Modulo)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Bullet_Multiply)
