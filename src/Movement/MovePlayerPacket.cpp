#include "MovePlayerPacket.h"

#include "../Network/ByteAngle.h"
#include "../Network/PacketReader.h"

std::optional<MovePlayerPacket> MovePlayerPacket::read(std::string_view payload)
{
    PacketReader reader(payload);
    MovePlayerPacket packet{};
    packet.runtimeId = reader.varU64();
    packet.x = reader.f32();
    packet.y = reader.f32();
    packet.z = reader.f32();
    packet.pitch = reader.f32();
    packet.yaw = reader.f32();
    packet.headYaw = reader.f32();
    packet.mode = reader.u8();
    packet.onGround = reader.boolean();
    packet.riddenRuntimeId = reader.varU64();
    if (!reader.ok()) {
        return std::nullopt;
    }
    return packet;
}

void MovePlayerPacket::applyTo(PlayerState &state) const
{
    const bool teleport = mode == 1 || mode == 2;
    state.x = x;
    state.y = y;
    state.z = z;
    state.pitch = ByteAngle::fromDegrees(pitch);
    state.yaw = ByteAngle::fromDegrees(yaw);
    state.headYaw = ByteAngle::fromDegrees(headYaw);
    state.flags = static_cast<std::uint8_t>((onGround ? 1 : 0) | (teleport ? 2 : 0));
}
