#pragma once
#include <SFML/Graphics.hpp>

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
};
