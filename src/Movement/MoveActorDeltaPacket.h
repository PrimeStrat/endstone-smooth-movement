#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "PlayerState.h"

class MoveActorDeltaPacket {
public:
    static constexpr int ID = 111;

    enum class Layout {
        Flags,
        Optionals,
    };

    /**
     * Picks the wire layout used by a protocol version.
     * @param protocolVersion Network protocol version.
     * @return Optionals from protocol 2168 (1.26.40), Flags before.
     */
    static Layout layoutFor(int protocolVersion);

    /**
     * Reads a MoveActorDelta payload. A trailing tick field (1.26.50+) is ignored.
     * @param payload Payload without the header.
     * @param layout Wire layout of the running protocol.
     * @return The packet, or nothing if the payload is too short.
     */
    static std::optional<MoveActorDeltaPacket> read(std::string_view payload, Layout layout);

    /**
     * Copies the changed fields into a player's state.
     * @param state State to update.
     * @return Nothing.
     */
    void applyTo(PlayerState &state) const;

    std::uint64_t runtimeId;
    std::optional<float> x;
    std::optional<float> y;
    std::optional<float> z;
    std::optional<std::uint8_t> pitch;
    std::optional<std::uint8_t> yaw;
    std::optional<std::uint8_t> headYaw;
    std::uint8_t flags;
};
