#pragma once

#include "NativeGameplayTags.h"

#include "Tags/Ability/Requirement/TagAbilityRequirement.h"
#include "Tags/Ability/Lockon/TagLockon.h"
#include "Tags/Ability/Movement/TagMovement.h"
#include "Tags/Ability/Attacked/TagAttacked.h"

namespace MyTags
{
    namespace Ability
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(requirement)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(lockon)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(movement)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(attacked)
    }
}