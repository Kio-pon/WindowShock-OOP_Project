#include "../include/Player.hpp"
#include <cmath>
#include <iostream>

Player::Player(float radius, float speed, float startX, float startY)
    : Entity(sf::Vector2f(startX, startY), radius, sf::Color(0, 178, 225)), // Diep.io Blue
      moveSpeed(speed)
{
    // Initialize stats
    recalculateStats();
    currentHealth = currentMaxHealth;
    
    // Add default barrel (Basic Tank)
    // Length, Width, Offset, Angle
    addBarrel(radius * 2.2f, radius * 0.8f, 0.0f, 0.0f);
}

void Player::recalculateStats()
{
    // Base stats + Multipliers based on levels
    currentMaxHealth = 100.0f + (statLevels[1] * 20.0f);
    currentHealthRegen = 0.05f + (statLevels[0] * 0.05f); // HP per frame/tick
    currentBodyDamage = 20.0f + (statLevels[2] * 5.0f);
    
    currentBulletSpeed = 300.0f + (statLevels[3] * 30.0f);
    currentBulletPenetration = 5.0f + (statLevels[4] * 5.0f); // Health of bullet
    currentBulletDamage = 10.0f + (statLevels[5] * 3.0f);
    
    currentReload = 0.5f * std::pow(0.9f, statLevels[6]); // Seconds between shots
    currentMovementSpeed = 150.0f + (statLevels[7] * 15.0f);
}

void Player::handleInput()
{
    velocity = sf::Vector2f(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        velocity.y -= currentMovementSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        velocity.y += currentMovementSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        velocity.x -= currentMovementSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        velocity.x += currentMovementSpeed;
}

void Player::update(float dt)
{
    // Base entity update (position + recoil recovery)
    Entity::update(dt);
    
    // Regen health
    if (currentHealth < currentMaxHealth)
    {
        currentHealth += currentHealthRegen; // Should scale with DT, but keeping simple for now
        if (currentHealth > currentMaxHealth) currentHealth = currentMaxHealth;
    }
    
    // Reload timer
    if (reloadTimer > 0.0f)
    {
        reloadTimer -= dt;
    }
}

void Player::constrainToWindow(const FakeWindow &fw)
{
    sf::Vector2f pos = getPosition();
    float r = getRadius();

    if (pos.x - r < fw.getLeft()) pos.x = fw.getLeft() + r;
    if (pos.x + r > fw.getRight()) pos.x = fw.getRight() - r;
    if (pos.y - r < fw.getTop()) pos.y = fw.getTop() + r;
    if (pos.y + r > fw.getBottom()) pos.y = fw.getBottom() - r;

    setPosition(pos);
}

std::vector<Bullet> Player::createBullets(sf::Vector2f targetPos)
{
    std::vector<Bullet> newBullets;
    
    // Only shoot if reload is ready (checked in main, but good to know)
    
    sf::Vector2f pos = getPosition();
    sf::Vector2f dirToMouse = targetPos - pos;
    float len = std::sqrt(dirToMouse.x * dirToMouse.x + dirToMouse.y * dirToMouse.y);
    float baseAngle = 0.0f;
    if (len != 0)
    {
        dirToMouse /= len;
        baseAngle = std::atan2(dirToMouse.y, dirToMouse.x) * 180.0f / 3.14159f;
    }

    for (size_t i = 0; i < barrels.size(); ++i)
    {
        const auto& b = barrels[i];
        
        // Apply recoil
        applyRecoil(i, 5.0f);
        
        // Calculate bullet spawn position and direction
        // Barrel angle is relative to player rotation (which is baseAngle)
        float totalAngle = baseAngle + b.angle;
        float radAngle = totalAngle * 3.14159f / 180.0f;
        
        sf::Vector2f dir(std::cos(radAngle), std::sin(radAngle));
        
        // Spawn position:
        // Start at center
        // Move forward by (Radius * 0.8 + BarrelLength) ? No, usually barrel length is visual.
        // Let's say tip is at: Center + Rotate(Offset_Lateral) + Rotate(Length_Forward)
        
        // Actually, using the transform logic from draw:
        // 1. Rotate by totalAngle
        // 2. Translate (0, b.offset) -> This is lateral in local space
        // 3. Translate (b.length, 0) -> This is forward to tip
        
        // We need to manually calculate this vector addition
        
        // Forward vector (along barrel angle)
        sf::Vector2f forward = dir;
        
        // Right vector (perpendicular)
        sf::Vector2f right(-forward.y, forward.x); // (-sin, cos) is +90 deg rotation?
        // If forward is (1,0) [0 deg], right should be (0,1) [90 deg].
        // (-0, 1) -> Correct.
        
        // Position = Center + Forward * (Radius * 0.8 + Length) + Right * Offset
        // Adjust "Radius * 0.8" to match where barrel visually starts.
        // In draw, we just draw it.
        // Let's approximate: Tip is at Center + Forward * (b.length) + Right * b.offset
        // But b.length is the length of the rect.
        // And we usually want it to spawn at the tip.
        
        sf::Vector2f spawnPos = pos + forward * (b.length) + right * b.offset;
        
        newBullets.emplace_back(spawnPos, dir * currentBulletSpeed);
    }
    
    return newBullets;
}

void Player::earnXp(int amount)
{
    if (level >= maxLevel) return;
    
    xp += amount;
    int req = getXpForNextLevel();
    while (xp >= req && level < maxLevel)
    {
        xp -= req;
        level++;
        skillPoints++;
        req = getXpForNextLevel();
        // Notify level up? handled by checking level in main
    }
}

int Player::getXpForNextLevel() const
{
    // Simple curve
    return 100 * level; 
}

void Player::earnCurrency(int amount)
{
    currency += amount;
    earnXp(amount * 10); // Convert currency/score to XP
}

void Player::takeDamage(int damage)
{
    currentHealth -= damage;
}

bool Player::isDead() const
{
    return currentHealth <= 0;
}

void Player::upgradeStat(int statIndex)
{
    if (skillPoints > 0 && statIndex >= 0 && statIndex < 8)
    {
        if (statLevels[statIndex] < 7)
        {
            statLevels[statIndex]++;
            skillPoints--;
            recalculateStats();
        }
    }
}

void Player::applyUpgrade(int upgradeIndex)
{
    // Legacy support or map to new stats
    // 0: Splash -> maybe Bullet Damage?
    // 1: Speed -> Movement Speed
    // 2: Multi Shot -> maybe Reload?
    
    if (upgradeIndex == 0) upgradeStat(5); // Dmg
    else if (upgradeIndex == 1) upgradeStat(7); // Speed
    else if (upgradeIndex == 2) upgradeStat(6); // Reload
}
