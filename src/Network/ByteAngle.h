#pragma once

#include <cmath>
#include <cstdint>

class ByteAngle {
public:
    /**
     * Packs an angle in degrees into the protocol's one-byte rotation.
     * @param degrees Angle in degrees, any range.
     * @return Angle in 1/256 turns.
     */
    static std::uint8_t fromDegrees(float degrees)
    {
        return static_cast<std::uint8_t>(std::lround(degrees / (360.0F / 256.0F)) & 0xFF);
    }
};
