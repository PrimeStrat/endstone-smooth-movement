#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "PlayerState.h"

class MovePlayerPacket {
public:
    static constexpr int ID = 19;

    /**
     * Reads the fields up to the ridden runtime id. Later fields differ by version and are not needed.
     * @param payload MovePlayer payload without the header.
     * @return The packet, or nothing if the payload is too short.
     */
    static std::optional<MovePlayerPacket> read(std::string_view payload);

    /**
     * Copies this movement into a player's state.
     * @param state State to update.
     * @return Nothing.
     */
    void applyTo(PlayerState &state) const;

    std::uint64_t runtimeId;
    float x;
    float y;
    float z;
    float pitch;
    float yaw;
    float headYaw;
    std::uint8_t mode;
    bool onGround;
    std::uint64_t riddenRuntimeId;
};
