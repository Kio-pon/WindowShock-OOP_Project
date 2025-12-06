#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

// Represents a bullet fired by the player
class Bullet : public Entity
{
public:
    Bullet(sf::Vector2f position, sf::Vector2f vel, int dmg = 10);
    int getDamage() const { return damage; }

private:
    int damage;
};
