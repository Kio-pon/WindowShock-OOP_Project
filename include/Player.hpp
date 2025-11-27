#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "FakeWindow.hpp"
#include "Bullet.hpp"

// The player character controlled by the user
class Player : public Entity
{
private:
    float moveSpeed;

public:
    int currency = 0; // will use this after
    
    // Leveling System
    int xp = 0;
    int level = 1;
    int skillPoints = 0;
    int maxLevel = 45;
    
    // Stats (0-7 levels)
    int statLevels[8] = {0};
    // 0: Health Regen
    // 1: Max Health
    // 2: Body Damage
    // 3: Bullet Speed
    // 4: Bullet Penetration
    // 5: Bullet Damage
    // 6: Reload
    // 7: Movement Speed

    // Derived Stats (Calculated from levels)
    float currentHealth;
    float currentMaxHealth;
    float currentHealthRegen;
    float currentBodyDamage;
    float currentBulletSpeed;
    float currentBulletPenetration;
    float currentBulletDamage;
    float currentReload;
    float currentMovementSpeed;

    // Reload management
    float reloadTimer = 0.0f;

    Player(float radius = 20.0f, float speed = 5.0f, float startX = 0.0f, float startY = 0.0f);

    // Handle keyboard input for movement
    void handleInput();

    // Keep player inside the window boundaries
    void constrainToWindow(const FakeWindow &fw);

    // Override update to handle stats regen, reload, etc.
    void update(float dt) override;

    // Create bullets aimed at target position (supports multiple barrels)
    std::vector<Bullet> createBullets(sf::Vector2f targetPos);

    void earnXp(int amount);
    void earnCurrency(int amount); // Legacy wrapper
    void takeDamage(int damage);
    bool isDead() const;

    // Upgrade a specific stat
    void upgradeStat(int statIndex);
    
    // Recalculate derived stats based on levels
    void recalculateStats();
    
    // Get required XP for next level
    int getXpForNextLevel() const;
    
    // Apply purchased upgrade effects (Legacy - might remove or adapt)
    void applyUpgrade(int upgradeIndex);
};
