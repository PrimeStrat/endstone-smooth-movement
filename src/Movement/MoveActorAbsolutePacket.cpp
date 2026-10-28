#include "MoveActorAbsolutePacket.h"

#include "../Network/PacketWriter.h"

std::string_view MoveActorAbsolutePacket::write(const PlayerState &state, std::string &out)
{
    PacketWriter(out)
        .varU64(state.runtimeId)
        .u8(state.flags)
        .f32(state.x)
        .f32(state.y)
        .f32(state.z)
        .u8(state.pitch)
        .u8(state.yaw)
        .u8(state.headYaw);
    return out;
}
