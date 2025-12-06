#include "../include/TankClass.hpp"
#include "../include/Player.hpp"
#include <cmath>

// --- Basic Tank ---
void BasicTank::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> BasicTank::getUpgrades() {
    return {
        std::make_shared<Twin>(),
        std::make_shared<Sniper>(),
        std::make_shared<MachineGun>(),
        std::make_shared<FlankGuard>()
    };
}

// --- Tier 2 ---

// Twin
void Twin::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 0.0f);
    player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Twin::getUpgrades() {
    return {
        std::make_shared<TripleShot>(),
        std::make_shared<QuadTank>(),
        std::make_shared<TwinFlank>()
    };
}

// Sniper
void Sniper::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.5f, r * 0.8f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Sniper::getUpgrades() {
    return {
        std::make_shared<Assassin>(),
        std::make_shared<Overseer>(),
        std::make_shared<Hunter>(),
        std::make_shared<Trapper>()
    };
}

// Machine Gun
void MachineGun::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 1.8f, r * 1.2f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> MachineGun::getUpgrades() {
    return {
        std::make_shared<Destroyer>(),
        std::make_shared<Gunner>()
    };
}

// Flank Guard
void FlankGuard::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
    player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 180.0f);
}

std::vector<std::shared_ptr<Tank>> FlankGuard::getUpgrades() {
    return {
        std::make_shared<TriAngle>(),
        std::make_shared<QuadTank>(),
        std::make_shared<TwinFlank>(),
        std::make_shared<Auto3>()
    };
}

// --- Tier 3 ---

// Triple Shot
void TripleShot::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.0f, r * 0.8f, 0.0f, 0.0f);
    player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 45.0f);
    player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, -45.0f);
}

std::vector<std::shared_ptr<Tank>> TripleShot::getUpgrades() { return {}; }

// Quad Tank
void QuadTank::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    for (int i = 0; i < 4; i++)
        player.addBarrel(r * 2.0f, r * 0.8f, 0.0f, i * 90.0f);
}

std::vector<std::shared_ptr<Tank>> QuadTank::getUpgrades() { return {}; }

// Twin Flank
void TwinFlank::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 0.0f);
    player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 0.0f);
    player.addBarrel(r * 2.0f, r * 0.8f, r * 0.5f, 180.0f);
    player.addBarrel(r * 2.0f, r * 0.8f, -r * 0.5f, 180.0f);
}

std::vector<std::shared_ptr<Tank>> TwinFlank::getUpgrades() { return {}; }

// Assassin
void Assassin::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 3.0f, r * 0.8f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Assassin::getUpgrades() { return {}; }

// Overseer
void Overseer::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 90.0f);
    player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, -90.0f);
}

std::vector<std::shared_ptr<Tank>> Overseer::getUpgrades() { return {}; }

// Hunter
void Hunter::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.5f, r * 0.7f, 0.0f, 0.0f);
    player.addBarrel(r * 2.0f, r * 1.0f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Hunter::getUpgrades() { return {}; }

// Trapper
void Trapper::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 1.5f, r * 1.2f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Trapper::getUpgrades() { return {}; }

// Destroyer
void Destroyer::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.0f, r * 1.5f, 0.0f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Destroyer::getUpgrades() { return {}; }

// Gunner
void Gunner::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 1.5f, r * 0.4f, r * 0.3f, 0.0f);
    player.addBarrel(r * 1.5f, r * 0.4f, -r * 0.3f, 0.0f);
    player.addBarrel(r * 1.8f, r * 0.4f, r * 0.6f, 0.0f);
    player.addBarrel(r * 1.8f, r * 0.4f, -r * 0.6f, 0.0f);
}

std::vector<std::shared_ptr<Tank>> Gunner::getUpgrades() { return {}; }

// Tri-Angle
void TriAngle::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 2.2f, r * 0.8f, 0.0f, 0.0f);
    player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 150.0f);
    player.addBarrel(r * 1.8f, r * 0.8f, 0.0f, 210.0f);
}

std::vector<std::shared_ptr<Tank>> TriAngle::getUpgrades() { return {}; }

// Auto 3
void Auto3::configure(Player& player) {
    player.clearBarrels();
    float r = player.getRadius();
    player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 0.0f);
    player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 120.0f);
    player.addBarrel(r * 1.5f, r * 0.6f, 0.0f, 240.0f);
}

std::vector<std::shared_ptr<Tank>> Auto3::getUpgrades() { return {}; }

// Smasher
void Smasher::configure(Player& player) {
    player.clearBarrels();
    // No barrels for Smasher
}

std::vector<std::shared_ptr<Tank>> Smasher::getUpgrades() { return {}; }
