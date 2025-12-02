#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Bullet.hpp"

enum class EnemyType
{
    Triangle, // Basic, slows when close
    Circle,   // Fast, burst movement
    Square,   // Dashes
    Spiker    // Boss
};

// Represents an enemy that chases the player
class Enemy : public Entity
{
public:
    Enemy(sf::Vector2f position, float speedVal, int hp, int currency, EnemyType type = EnemyType::Triangle);

    // Update returns bullets if the enemy shoots
    std::vector<Bullet> update(sf::Vector2f playerPos, float dt);

    // draw, getPosition, getRadius handled by Entity

    void takeDamage(int damage);
    bool isDead() const;
    int getCurrencyDrop() const;
    EnemyType getType() const { return type; }

private:
    float speed;
    int health;
    int maxHealth;
    int currencyDrop;
    EnemyType type;
    
    // Shooting logic
    float reloadTimer = 0.0f;
    float reloadTime = 2.0f;
    
    // Behavior state
    float moveTimer = 0.0f;
    float stopTimer = 0.0f;
    bool isMoving = true;
    sf::Vector2f dashDirection;
};
