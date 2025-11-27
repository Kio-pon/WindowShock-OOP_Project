#pragma once
#include "Player.hpp"

enum class TankType
{
    Basic,
    Twin,
    Sniper,
    MachineGun,
    FlankGuard
};

// Helper to configure player barrels and stats based on tank type
void configureTank(Player& player, TankType type);
