#include "../include/Player.hpp"
#include <cmath>
#include <iostream>

Player::Player(float radius, float speed, float startX, float startY)
    : Entity(sf::Vector2f(startX, startY), radius, sf::Color(0, 178, 225)),
      moveSpeed(speed)
{
    recalculateStats();
    currentHealth = currentMaxHealth;
    setTank(std::make_shared<BasicTank>());
}

void Player::setTank(std::shared_ptr<Tank> newTank)
{
    currentTank = newTank;
    currentTank->configure(*this);
}

void Player::recalculateStats()
{
    // Initialize stats with base values and level multipliers
    currentMaxHealth = 100.0f + (statLevels[1] * 20.0f);
    currentHealthRegen = 0.05f + (statLevels[0] * 0.05f);
    currentBodyDamage = 20.0f + (statLevels[2] * 5.0f);
    
    currentBulletSpeed = 800.0f + (statLevels[3] * 50.0f);
    currentBulletPenetration = 5.0f + (statLevels[4] * 5.0f);
    currentBulletDamage = 25.0f + (statLevels[5] * 5.0f);
    
    currentReload = 0.5f * std::pow(0.9f, statLevels[6]);
    currentMovementSpeed = 300.0f + (statLevels[7] * 20.0f);
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
    Entity::update(dt);
    
    // Regenerate health
    if (currentHealth < currentMaxHealth)
    {
        currentHealth += currentHealthRegen;
        if (currentHealth > currentMaxHealth) currentHealth = currentMaxHealth;
    }
    
    // Update reload timer
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
        
        applyRecoil(i, 5.0f);
        
        // Calculate total rotation angle for the barrel
        float totalAngle = baseAngle + b.angle;
        float radAngle = totalAngle * 3.14159f / 180.0f;
        
        sf::Vector2f dir(std::cos(radAngle), std::sin(radAngle));
        
        // Calculate bullet spawn position at barrel tip
        sf::Vector2f forward = dir;
        sf::Vector2f right(-forward.y, forward.x);
        
        sf::Vector2f spawnPos = pos + forward * (b.length) + right * b.offset;
        
        newBullets.emplace_back(spawnPos, dir * currentBulletSpeed, static_cast<int>(currentBulletDamage));
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
    }
}

int Player::getXpForNextLevel() const
{
    return 100 * level; 
}

void Player::earnCurrency(int amount)
{
    currency += amount;
    earnXp(amount * 10);
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
    // Map upgrades to specific stats
    if (upgradeIndex == 0) upgradeStat(5); // Damage
    else if (upgradeIndex == 1) upgradeStat(7); // Speed
    else if (upgradeIndex == 2) upgradeStat(6); // Reload
}
