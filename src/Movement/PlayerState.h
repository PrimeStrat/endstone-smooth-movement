#pragma once

#include <cstdint>

struct PlayerState {
    std::uint64_t runtimeId;
    float x;
    float y;
    float z;
    std::uint8_t pitch;
    std::uint8_t yaw;
    std::uint8_t headYaw;
    std::uint8_t flags;
};
