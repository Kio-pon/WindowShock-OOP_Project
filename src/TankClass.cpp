#include "../include/TankClass.hpp"

void configureTank(Player& player, TankType type)
{
    player.clearBarrels();
    float r = player.getRadius();

    switch (type)
    {
    case TankType::Basic:
        // 1 Barrel
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        break;

    case TankType::Twin:
        // 2 Parallel Barrels
        // Offset is lateral.
        // Barrel 1: Offset -0.5 * width? No, offset is distance from center.
        // Let's say width is 0.8*r.
        // We want them side by side.
        player.addBarrel(r * 2.2f, r * 0.8f, -r * 0.5f, 0.0f);
        player.addBarrel(r * 2.2f, r * 0.8f, r * 0.5f, 0.0f);
        break;

    case TankType::Sniper:
        // 1 Long Barrel
        player.addBarrel(r * 3.0f, r * 0.8f, 0.0f, 0.0f);
        // TODO: Increase view range (needs to be handled in main/camera)
        break;

    case TankType::MachineGun:
        // 1 Trapezoid Barrel (Visualized as wide barrel for now)
        // Or we can add a second wider barrel at base?
        // For simple rects, just make it wider?
        player.addBarrel(r * 2.0f, r * 1.2f, 0.0f, 0.0f);
        // Spread logic needs to be in Player::createBullet or main
        break;

    case TankType::FlankGuard:
        // 1 Front, 1 Back
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);   // Front
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 180.0f); // Back
        break;
    }
}
