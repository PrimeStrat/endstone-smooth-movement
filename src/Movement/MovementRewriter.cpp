#include "MovementRewriter.h"

#include "../Network/ByteAngle.h"
#include "MoveActorAbsolutePacket.h"
#include "MovePlayerPacket.h"

MovementRewriter::MovementRewriter(int protocolVersion) : deltaLayout_(MoveActorDeltaPacket::layoutFor(protocolVersion))
{
    buffer_.reserve(32);
}

void MovementRewriter::trackPlayer(std::uint64_t runtimeId, float x, float y, float z, float pitch, float yaw)
{
    const std::uint8_t yawByte = ByteAngle::fromDegrees(yaw);
    players_[runtimeId] = {runtimeId, x, y, z, ByteAngle::fromDegrees(pitch), yawByte, yawByte, 0};
}

void MovementRewriter::untrackPlayer(std::uint64_t runtimeId)
{
    players_.erase(runtimeId);
}

MoveActorDeltaPacket::Layout MovementRewriter::deltaLayout() const
{
    return deltaLayout_;
}

std::string_view MovementRewriter::rewrite(int packetId, std::string_view payload, std::uint64_t recipientRuntimeId)
{
    switch (packetId) {
    case MovePlayerPacket::ID:
        return rewriteMovePlayer(payload, recipientRuntimeId);
    case MoveActorDeltaPacket::ID:
        return rewriteMoveDelta(payload, recipientRuntimeId);
    default:
        return {};
    }
}

std::string_view MovementRewriter::rewriteMovePlayer(std::string_view payload, std::uint64_t recipientRuntimeId)
{
    const auto packet = MovePlayerPacket::read(payload);
    PlayerState *state = packet ? find(packet->runtimeId) : nullptr;
    if (!state) {
        return {};
    }
    packet->applyTo(*state);
    if (packet->runtimeId == recipientRuntimeId || packet->riddenRuntimeId != 0) {
        return {};
    }
    return MoveActorAbsolutePacket::write(*state, buffer_);
}

std::string_view MovementRewriter::rewriteMoveDelta(std::string_view payload, std::uint64_t recipientRuntimeId)
{
    const auto packet = MoveActorDeltaPacket::read(payload, deltaLayout_);
    PlayerState *state = packet ? find(packet->runtimeId) : nullptr;
    if (!state) {
        return {};
    }
    packet->applyTo(*state);
    if (packet->runtimeId == recipientRuntimeId) {
        return {};
    }
    return MoveActorAbsolutePacket::write(*state, buffer_);
}

PlayerState *MovementRewriter::find(std::uint64_t runtimeId)
{
    const auto it = players_.find(runtimeId);
    return it == players_.end() ? nullptr : &it->second;
}
