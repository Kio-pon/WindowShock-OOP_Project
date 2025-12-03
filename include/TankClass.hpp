#pragma once
#include <vector>
#include <memory>
#include <string>

class Player;

// Abstract Base Class
class Tank {
public:
    virtual ~Tank() = default;
    virtual void configure(Player& p) = 0;
    virtual std::vector<std::shared_ptr<Tank>> getUpgrades() = 0;
    virtual std::string getName() = 0;
    virtual int getTier() = 0; // 1, 2, or 3
};

// --- Tier 1 ---
class BasicTank : public Tank {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Basic Tank"; }
    int getTier() override { return 1; }
};

// --- Tier 2 ---
class Twin : public Tank {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Twin"; }
    int getTier() override { return 2; }
};

class Sniper : public Tank {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Sniper"; }
    int getTier() override { return 2; }
};

class MachineGun : public Tank {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Machine Gun"; }
    int getTier() override { return 2; }
};

class FlankGuard : public Tank {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Flank Guard"; }
    int getTier() override { return 2; }
};

// --- Tier 3 (Inheriting from Tier 2) ---

// From Twin
class TripleShot : public Twin {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Triple Shot"; }
    int getTier() override { return 3; }
};

class QuadTank : public Twin { // Shared with FlankGuard, picking Twin as primary
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Quad Tank"; }
    int getTier() override { return 3; }
};

class TwinFlank : public Twin { // Shared with FlankGuard
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Twin Flank"; }
    int getTier() override { return 3; }
};

// From Sniper
class Assassin : public Sniper {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Assassin"; }
    int getTier() override { return 3; }
};

class Overseer : public Sniper {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Overseer"; }
    int getTier() override { return 3; }
};

class Hunter : public Sniper {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Hunter"; }
    int getTier() override { return 3; }
};

class Trapper : public Sniper {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Trapper"; }
    int getTier() override { return 3; }
};

// From MachineGun
class Destroyer : public MachineGun {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Destroyer"; }
    int getTier() override { return 3; }
};

class Gunner : public MachineGun {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Gunner"; }
    int getTier() override { return 3; }
};

// From FlankGuard
class TriAngle : public FlankGuard {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Tri-Angle"; }
    int getTier() override { return 3; }
};

class Auto3 : public FlankGuard {
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Auto 3"; }
    int getTier() override { return 3; }
};

// Special
class Smasher : public Tank { // Unique branch
public:
    void configure(Player& p) override;
    std::vector<std::shared_ptr<Tank>> getUpgrades() override;
    std::string getName() override { return "Smasher"; }
    int getTier() override { return 3; }
};
