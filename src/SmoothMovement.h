#pragma once

#include <memory>

#include <endstone/endstone.hpp>

#include "Movement/MovementRewriter.h"

class SmoothMovement : public endstone::Plugin {
public:
    /**
     * Picks the packet layout for the running protocol and starts listening.
     * @return Nothing.
     */
    void onEnable() override;

    /**
     * Replaces player movement packets with MoveActorAbsolute.
     * @param event The outgoing packet event.
     * @return Nothing.
     */
    void onPacketSend(endstone::PacketSendEvent &event);

    /**
     * Starts tracking a joining player.
     * @param event The join event.
     * @return Nothing.
     */
    void onPlayerJoin(endstone::PlayerJoinEvent &event);

    /**
     * Stops tracking a leaving player.
     * @param event The quit event.
     * @return Nothing.
     */
    void onPlayerQuit(endstone::PlayerQuitEvent &event);

private:
    /**
     * Seeds the rewriter with a player's current position and rotation.
     * @param player Player to track.
     * @return Nothing.
     */
    void track(const endstone::Player &player);

    std::unique_ptr<MovementRewriter> rewriter_;
};
