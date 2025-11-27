#pragma once
#include <SFML/Graphics.hpp>
<<<<<<< HEAD
#include "Entity.hpp"

// Represents a bullet fired by the player
class Bullet : public Entity
{
public:
    Bullet(sf::Vector2f position, sf::Vector2f vel);

    // update, draw, getPosition, getRadius handled by Entity
=======

// Represents a bullet fired by the player
class Bullet
{
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;

public:
    Bullet(sf::Vector2f position, sf::Vector2f vel);

    void update();

    void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition() const;
    float getRadius() const;
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
};
