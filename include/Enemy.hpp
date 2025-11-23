#pragma once
#include <SFML/Graphics.hpp>

// Represents an enemy that chases the player
class Enemy
{
public:
    Enemy(sf::Vector2f position, float speedVal, int hp = 2, int currency = 5);

    void update(sf::Vector2f playerPos);

    void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition() const;
    float getRadius() const;
    void takeDamage(int damage);
    bool isDead() const;
    int getCurrencyDrop() const;

private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;
    int health;
    int currencyDrop;
};
