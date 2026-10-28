#include "SmoothMovement.h"

#include "Movement/MoveActorAbsolutePacket.h"
#include "Movement/MovePlayerPacket.h"

void SmoothMovement::onEnable()
{
    const int protocol = getServer().getProtocolVersion();
    rewriter_ = std::make_unique<MovementRewriter>(protocol);
    const bool optionals = rewriter_->deltaLayout() == MoveActorDeltaPacket::Layout::Optionals;
    getLogger().info("Minecraft {} (protocol {}), MoveActorDelta layout: {}", getServer().getMinecraftVersion(),
                     protocol, optionals ? "optionals" : "flags");
    for (auto *player : getServer().getOnlinePlayers()) {
        track(*player);
    }
    registerEvent(&SmoothMovement::onPacketSend, *this, endstone::EventPriority::Highest, true);
    registerEvent(&SmoothMovement::onPlayerJoin, *this);
    registerEvent(&SmoothMovement::onPlayerQuit, *this);
}

void SmoothMovement::onPacketSend(endstone::PacketSendEvent &event)
{
    const int id = event.getPacketId();
    if (id != MovePlayerPacket::ID && id != MoveActorDeltaPacket::ID) {
        return;
    }
    auto *recipient = event.getPlayer();
    if (!recipient) {
        return;
    }
    const auto absolute = rewriter_->rewrite(id, event.getPayload(), recipient->getRuntimeId());
    if (absolute.empty()) {
        return;
    }
    event.cancel();
    recipient->sendPacket(MoveActorAbsolutePacket::ID, absolute);
}

void SmoothMovement::onPlayerJoin(endstone::PlayerJoinEvent &event)
{
    track(event.getPlayer());
}

void SmoothMovement::onPlayerQuit(endstone::PlayerQuitEvent &event)
{
    rewriter_->untrackPlayer(event.getPlayer().getRuntimeId());
}

void SmoothMovement::track(const endstone::Player &player)
{
    const auto loc = player.getLocation();
    rewriter_->trackPlayer(player.getRuntimeId(), loc.getX(), loc.getY(), loc.getZ(), loc.getPitch(), loc.getYaw());
}
