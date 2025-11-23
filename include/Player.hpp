#pragma once
#include <SFML/Graphics.hpp>
#include "FakeWindow.hpp"
#include "Bullet.hpp"

// The player character controlled by the user
class Player
{
private:
    sf::CircleShape circle;
    float moveSpeed;

public:
    int currency = 83;
    int health = 100;
    int maxHealth = 100;

    // Upgrade levels
    int splashDamageLevel = 0;
    int speedLevel = 0;
    int multiShotLevel = 0;

    Player(float radius = 12.5f, float speed = 5.0f, float startX = 0.0f, float startY = 0.0f);

    // Handle keyboard input for movement
    void handleInput();

    // Keep player inside the window boundaries
    void constrainToWindow(const FakeWindow &fw);

    sf::Vector2f getPosition() const;
    float getRadius() const;

    void draw(sf::RenderWindow &window);

    // Create a bullet aimed at target position
    Bullet createBullet(sf::Vector2f targetPos, float bulletSpeed);

    void earnCurrency(int amount);
    void takeDamage(int damage);
    bool isDead() const;

    // Apply purchased upgrade effects
    void applyUpgrade(int upgradeIndex);
};
