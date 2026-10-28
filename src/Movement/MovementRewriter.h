#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

#include "MoveActorDeltaPacket.h"
#include "PlayerState.h"

class MovementRewriter {
public:
    /**
     * Creates a rewriter for the wire layout of the given protocol.
     * @param protocolVersion Network protocol version of the running server.
     */
    explicit MovementRewriter(int protocolVersion);

    /**
     * Starts tracking a player so their movement packets get rewritten.
     * @param runtimeId Actor runtime id of the player.
     * @param x Current x position.
     * @param y Current y position.
     * @param z Current z position.
     * @param pitch Current pitch in degrees.
     * @param yaw Current yaw in degrees.
     * @return Nothing.
     */
    void trackPlayer(std::uint64_t runtimeId, float x, float y, float z, float pitch, float yaw);

    /**
     * Stops tracking a player.
     * @param runtimeId Actor runtime id of the player.
     * @return Nothing.
     */
    void untrackPlayer(std::uint64_t runtimeId);

    /**
     * Converts a player movement packet into a MoveActorAbsolute payload.
     * @param packetId Id of the outgoing packet.
     * @param payload Outgoing payload without the header.
     * @param recipientRuntimeId Runtime id of the player receiving the packet.
     * @return The MoveActorAbsolute payload, or an empty view if the packet must pass through unchanged.
     */
    std::string_view rewrite(int packetId, std::string_view payload, std::uint64_t recipientRuntimeId);

    /**
     * Gets the MoveActorDelta layout picked for the running protocol.
     * @return The layout.
     */
    MoveActorDeltaPacket::Layout deltaLayout() const;

private:
    /**
     * Rewrites a MovePlayer packet.
     * @param payload MovePlayer payload.
     * @param recipientRuntimeId Runtime id of the receiving player.
     * @return The MoveActorAbsolute payload, or empty to pass through.
     */
    std::string_view rewriteMovePlayer(std::string_view payload, std::uint64_t recipientRuntimeId);

    /**
     * Rewrites a MoveActorDelta packet.
     * @param payload MoveActorDelta payload.
     * @param recipientRuntimeId Runtime id of the receiving player.
     * @return The MoveActorAbsolute payload, or empty to pass through.
     */
    std::string_view rewriteMoveDelta(std::string_view payload, std::uint64_t recipientRuntimeId);

    /**
     * Finds the tracked state of a player.
     * @param runtimeId Actor runtime id.
     * @return The state, or nullptr if the actor is not a tracked player.
     */
    PlayerState *find(std::uint64_t runtimeId);

    MoveActorDeltaPacket::Layout deltaLayout_;
    std::unordered_map<std::uint64_t, PlayerState> players_;
    std::string buffer_;
};
