#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

// Represents an enemy that chases the player
class Enemy : public Entity
{
public:
    Enemy(sf::Vector2f position, float speedVal, int hp = 2, int currency = 5);

    void update(sf::Vector2f playerPos, float dt);

    // draw, getPosition, getRadius handled by Entity

    void takeDamage(int damage);
    bool isDead() const;
    int getCurrencyDrop() const;

private:
    float speed;
    int health;
    int currencyDrop;
};
