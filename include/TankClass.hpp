#pragma once
#pragma once
#include <vector>

class Player;

enum class TankType
{
    // Tier 1 (Level 1)
    Basic,
    
    // Tier 2 (Level 5)
    Twin,
    Sniper,
    MachineGun,
    FlankGuard,
    
    // Tier 3 (Level 10)
    TripleShot,
    QuadTank,
    TwinFlank,
    Assassin,
    Overseer,
    Hunter,
    Trapper,
    Destroyer,
    Gunner,
    TriAngle,
    Auto3,
    Smasher,
    
    // Tier 4 (Level 15)
    Triplet,
    PentaShot,
    SpreadShot,
    OctoTank,
    Auto5,
    TripleTwin,
    Battleship,
    Ranger,
    Stalker,
    Overlord,
    Necromancer,
    Manager,
    Overtrapper,
    Factory,
    Predator,
    Streamliner,
    TriTrapper,
    GunnerTrapper,
    MegaTrapper,
    AutoTrapper,
    Hybrid,
    Annihilator,
    Skimmer,
    Rocketeer,
    Glider,
    AutoGunner,
    Sprayer,
    Booster,
    Fighter,
    Landmine,
    AutoSmasher,
    Spike,
    AutoTank
};

// Helper to get available upgrades for a specific tank type
std::vector<TankType> getAvailableUpgrades(TankType current);

// Helper to configure player barrels and stats based on tank type
void configureTank(Player& player, TankType type);
