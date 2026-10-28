#include "MoveActorDeltaPacket.h"

#include "../Network/PacketReader.h"

MoveActorDeltaPacket::Layout MoveActorDeltaPacket::layoutFor(int protocolVersion)
{
    return protocolVersion >= 2168 ? Layout::Optionals : Layout::Flags;
}

std::optional<MoveActorDeltaPacket> MoveActorDeltaPacket::read(std::string_view payload, Layout layout)
{
    PacketReader reader(payload);
    MoveActorDeltaPacket packet{};
    packet.runtimeId = reader.varU64();
    if (layout == Layout::Optionals) {
        if (reader.boolean()) packet.x = reader.f32();
        if (reader.boolean()) packet.y = reader.f32();
        if (reader.boolean()) packet.z = reader.f32();
        if (reader.boolean()) packet.pitch = reader.u8();
        if (reader.boolean()) packet.yaw = reader.u8();
        if (reader.boolean()) packet.headYaw = reader.u8();
        const int onGround = reader.boolean();
        const int forceMove = reader.boolean();
        const int forceMoveLocal = reader.boolean();
        const int forceCompletion = reader.boolean();
        packet.flags = static_cast<std::uint8_t>(onGround | forceMove << 1 | forceMoveLocal << 2 | forceCompletion << 3);
    }
    else {
        const std::uint16_t has = reader.u16();
        if (has & 0x01) packet.x = reader.f32();
        if (has & 0x02) packet.y = reader.f32();
        if (has & 0x04) packet.z = reader.f32();
        if (has & 0x08) packet.pitch = reader.u8();
        if (has & 0x10) packet.yaw = reader.u8();
        if (has & 0x20) packet.headYaw = reader.u8();
        packet.flags = static_cast<std::uint8_t>((has >> 6) & 0x07);
    }
    if (!reader.ok()) {
        return std::nullopt;
    }
    return packet;
}

void MoveActorDeltaPacket::applyTo(PlayerState &state) const
{
    state.x = x.value_or(state.x);
    state.y = y.value_or(state.y);
    state.z = z.value_or(state.z);
    state.pitch = pitch.value_or(state.pitch);
    state.yaw = yaw.value_or(state.yaw);
    state.headYaw = headYaw.value_or(state.headYaw);
    state.flags = flags;
}
