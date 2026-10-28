#pragma once

#include <string>
#include <string_view>

#include "PlayerState.h"

class MoveActorAbsolutePacket {
public:
    static constexpr int ID = 18;

    /**
     * Writes a MoveActorAbsolute payload for a player's state.
     * @param state State to write.
     * @param out Buffer that is cleared and reused.
     * @return View over the written payload.
     */
    static std::string_view write(const PlayerState &state, std::string &out);
};
