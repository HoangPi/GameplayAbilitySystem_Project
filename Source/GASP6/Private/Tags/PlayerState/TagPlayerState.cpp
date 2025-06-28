#include "Tags/PlayerState/TagPlayerState.h"

namespace MyTags
{
    namespace PlayerState
    {
        UE_DEFINE_GAMEPLAY_TAG(combat, "player_state.combat")
        UE_DEFINE_GAMEPLAY_TAG(guard, "player_state.guard")
        UE_DEFINE_GAMEPLAY_TAG(perfect_guard, "player_state.perfect_guard")
        UE_DEFINE_GAMEPLAY_TAG(manual_guard, "player_state.manual_guard")
        UE_DEFINE_GAMEPLAY_TAG(down, "player_state.down")
    }

}