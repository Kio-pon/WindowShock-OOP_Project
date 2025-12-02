#include "../include/TankClass.hpp"
#include "../include/Player.hpp"
#include <vector>
#include <cmath>

std::vector<TankType> getAvailableUpgrades(TankType current)
{
    std::vector<TankType> upgrades;

    switch (current)
    {
    case TankType::Basic:
        upgrades = {TankType::Twin, TankType::Sniper, TankType::MachineGun, TankType::FlankGuard};
        break;

    // Tier 2 -> Tier 3
    case TankType::Twin:
        upgrades = {TankType::TripleShot, TankType::QuadTank, TankType::TwinFlank};
        break;
    case TankType::Sniper:
        upgrades = {TankType::Assassin, TankType::Overseer, TankType::Hunter, TankType::Trapper};
        break;
    case TankType::MachineGun:
        upgrades = {TankType::Destroyer, TankType::Gunner};
        break;
    case TankType::FlankGuard:
        upgrades = {TankType::TriAngle, TankType::QuadTank, TankType::TwinFlank, TankType::Auto3};
        break;

    // Tier 3 -> Tier 4
    case TankType::TripleShot:
        upgrades = {TankType::Triplet, TankType::PentaShot, TankType::SpreadShot};
        break;
    case TankType::QuadTank:
        upgrades = {TankType::OctoTank, TankType::Auto5};
        break;
    case TankType::TwinFlank:
        upgrades = {TankType::TripleTwin, TankType::Battleship};
        break;
    case TankType::Assassin:
        upgrades = {TankType::Ranger, TankType::Stalker};
        break;
    case TankType::Overseer:
        upgrades = {TankType::Overlord, TankType::Necromancer, TankType::Manager, TankType::Overtrapper, TankType::Battleship};
        break;
    case TankType::Hunter:
        upgrades = {TankType::Predator, TankType::Streamliner};
        break;
    case TankType::Trapper:
        upgrades = {TankType::TriTrapper, TankType::GunnerTrapper, TankType::Overtrapper, TankType::MegaTrapper, TankType::AutoTrapper};
        break;
    case TankType::Destroyer:
        upgrades = {TankType::Hybrid, TankType::Annihilator, TankType::Skimmer, TankType::Rocketeer, TankType::Glider};
        break;
    case TankType::Gunner:
        upgrades = {TankType::AutoGunner, TankType::GunnerTrapper, TankType::Streamliner};
        break;
    case TankType::TriAngle:
        upgrades = {TankType::Booster, TankType::Fighter};
        break;
    case TankType::Auto3:
        upgrades = {TankType::Auto5, TankType::AutoGunner};
        break;
    case TankType::Smasher:
        upgrades = {TankType::Landmine, TankType::AutoSmasher, TankType::Spike};
        break;
    
    // Special case: Basic Tank can upgrade to Smasher at lvl 30 (Tier 3 equivalent) or Auto Tank at lvl 45 (Tier 4 equivalent)
    // For our simplified 5/10/15 system, we might want to offer these earlier or differently.
    // Let's stick to the tree structure. If they stay Basic, maybe offer Smasher at Tier 3?
    // For now, let's assume they must pick a Tier 2 to proceed, or we add Smasher to Basic's list if we want to support the "skip" mechanic.
    // The prompt implies a strict tree. Let's stick to the explicit paths.
    
    default:
        break;
    }

    return upgrades;
}

void configureTank(Player& player, TankType type)
{
    player.clearBarrels();
    player.currentTankType = type;
    float r = player.getRadius();

    // Helper for adding barrels: Length, Width, Offset, Angle
    // Note: Dimensions are relative to radius. 
    // Default Basic Tank: 2.2f, 0.8f, 0.0f, 0.0f

    switch (type)
    {
    case TankType::Basic:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        break;

    // --- Tier 2 ---
    case TankType::Twin:
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 0.0f);  // Right
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 0.0f); // Left
        break;
    case TankType::Sniper:
        player.addBarrel(r * 2.5f, r * 0.8f, 0.0f, 0.0f);
        // TODO: Increase FOV, Bullet Speed
        break;
    case TankType::MachineGun:
        // Trapezoid shape is hard with just rects, using a wide barrel for now
        player.addBarrel(r * 1.8f, r * 1.2f, 0.0f, 0.0f);
        // TODO: Increase Fire Rate, Spread
        break;
    case TankType::FlankGuard:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);   // Front
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 180.0f); // Back
        break;

    // --- Tier 3 ---
    case TankType::TripleShot:
        player.addBarrel(r * 2.0f, r * 0.8f, 0.0f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 45.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, -45.0f);
        break;
    case TankType::QuadTank:
        for (int i = 0; i < 4; i++)
            player.addBarrel(r * 2.0f, r * 0.8f, 0.0f, i * 90.0f);
        break;
    case TankType::TwinFlank:
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 0.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 0.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 180.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 180.0f);
        break;
    case TankType::Assassin:
        player.addBarrel(r * 3.0f, r * 0.8f, 0.0f, 0.0f);
        break;
    case TankType::Overseer:
        // Spawners look like wide short barrels
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 90.0f);
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, -90.0f);
        // TODO: Implement Drone Logic
        break;
    case TankType::Hunter:
        player.addBarrel(r * 2.5f, r * 0.7f, 0.0f, 0.0f);
        player.addBarrel(r * 2.0f, r * 1.0f, 0.0f, 0.0f); // Wider base
        break;
    case TankType::Destroyer:
        player.addBarrel(r * 2.0f, r * 1.5f, 0.0f, 0.0f);
        break;
    case TankType::Gunner:
        player.addBarrel(r * 1.5f, r * 0.4f, r * 0.3f, 0.0f);
        player.addBarrel(r * 1.5f, r * 0.4f, -r * 0.3f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.4f, r * 0.6f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.4f, -r * 0.6f, 0.0f);
        break;
    case TankType::TriAngle:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 150.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 210.0f);
        break;

    case TankType::Auto3:
        player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 0.0f);
        player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 120.0f);
        player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 240.0f);
        break;
    case TankType::Smasher:
        // No barrels, just body damage
        // Maybe visual spikes?
        break;
    
    // --- Tier 4 ---
    case TankType::SpreadShot:
        player.addBarrel(r * 2.0f, r * 0.6f, 0.0f, 0.0f);
        player.addBarrel(r * 1.9f, r * 0.6f, 0.0f, 15.0f);
        player.addBarrel(r * 1.9f, r * 0.6f, 0.0f, -15.0f);
        player.addBarrel(r * 1.8f, r * 0.6f, 0.0f, 30.0f);
        player.addBarrel(r * 1.8f, r * 0.6f, 0.0f, -30.0f);
        player.addBarrel(r * 1.7f, r * 0.6f, 0.0f, 45.0f);
        player.addBarrel(r * 1.7f, r * 0.6f, 0.0f, -45.0f);
        player.addBarrel(r * 1.6f, r * 0.6f, 0.0f, 60.0f);
        player.addBarrel(r * 1.6f, r * 0.6f, 0.0f, -60.0f);
        player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 75.0f);
        player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, -75.0f);
        break;
    case TankType::OctoTank:
        for (int i = 0; i < 8; i++)
            player.addBarrel(r * 2.0f, r * 0.8f, 0.0f, i * 45.0f);
        break;
    case TankType::Triplet:
        player.addBarrel(r * 2.0f, r * 0.7f, 0.0f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.7f, r * 0.6f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.7f, -r * 0.6f, 0.0f);
        break;
    case TankType::PentaShot:
        player.addBarrel(r * 2.0f, r * 0.7f, 0.0f, 0.0f);
        player.addBarrel(r * 1.9f, r * 0.7f, 0.0f, 20.0f);
        player.addBarrel(r * 1.9f, r * 0.7f, 0.0f, -20.0f);
        player.addBarrel(r * 1.7f, r * 0.7f, 0.0f, 40.0f);
        player.addBarrel(r * 1.7f, r * 0.7f, 0.0f, -40.0f);
        break;
    case TankType::Booster:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        player.addBarrel(r * 1.6f, r * 0.7f, 0.0f, 145.0f);
        player.addBarrel(r * 1.6f, r * 0.7f, 0.0f, 215.0f);
        player.addBarrel(r * 1.8f, r * 0.7f, 0.0f, 160.0f); 
        player.addBarrel(r * 1.8f, r * 0.7f, 0.0f, 200.0f);
        break;
    case TankType::Fighter:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 150.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 210.0f);
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 90.0f); 
        player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, -90.0f);
        break;
    case TankType::Hybrid:
        player.addBarrel(r * 2.0f, r * 1.5f, 0.0f, 0.0f);
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 180.0f); 
        break;
    case TankType::Annihilator:
        player.addBarrel(r * 1.8f, r * 2.2f, 0.0f, 0.0f); // Huge barrel
        break;
    case TankType::Overlord:
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 90.0f);
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, -90.0f);
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 0.0f);
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 180.0f);
        break;
    case TankType::Necromancer:
        // Square body usually, but standard for now
        player.addBarrel(r * 1.5f, r * 1.5f, 0.0f, 0.0f); // Placeholder
        break;
    case TankType::Manager:
        player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 0.0f); // One drone spawner
        break;
    case TankType::TripleTwin:
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 0.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 0.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 120.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 120.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 240.0f);
        player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 240.0f);
        break;
    case TankType::Battleship:
        player.addBarrel(r * 1.5f, r * 0.4f, r * 0.5f, 90.0f);
        player.addBarrel(r * 1.5f, r * 0.4f, -r * 0.5f, 90.0f);
        player.addBarrel(r * 1.5f, r * 0.4f, r * 0.5f, -90.0f);
        player.addBarrel(r * 1.5f, r * 0.4f, -r * 0.5f, -90.0f);
        break;
    case TankType::Ranger:
        player.addBarrel(r * 3.5f, r * 0.9f, 0.0f, 0.0f); // Very long
        break;
    case TankType::Stalker:
        player.addBarrel(r * 3.0f, r * 0.8f, 0.0f, 0.0f);
        break;
    case TankType::Predator:
        player.addBarrel(r * 2.8f, r * 0.6f, 0.0f, 0.0f);
        player.addBarrel(r * 2.4f, r * 0.8f, 0.0f, 0.0f);
        player.addBarrel(r * 2.0f, r * 1.0f, 0.0f, 0.0f);
        break;
    case TankType::Streamliner:
        for(int i=0; i<5; ++i)
            player.addBarrel(r * (2.5f - i*0.2f), r * 0.6f, 0.0f, 0.0f);
        break;
    
    // Default fallback
    default:
        player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
        break;
    }
}
