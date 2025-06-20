#pragma once

#include "NativeGameplayTags.h"

#include "Tags/Ability/Requirement/TagAbilityRequirement.h"
#include "Tags/Ability/Lockon/TagLockon.h"
#include "Tags/Ability/Movement/TagMovement.h"

namespace MyTags
{
    namespace Ability
    {
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(requirement)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(lockon)
        UE_DECLARE_GAMEPLAY_TAG_EXTERN(movement)
    }
}