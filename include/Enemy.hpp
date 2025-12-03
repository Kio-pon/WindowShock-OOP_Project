#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Bullet.hpp"
#include <vector>
#include <memory>

// Abstract Base Class
class Enemy : public Entity
{
public:
    Enemy(sf::Vector2f position, float speedVal, int hp, int currency);
    virtual ~Enemy() = default;

    // Pure virtual update to enforce specific behavior
    virtual std::vector<Bullet> update(sf::Vector2f playerPos, float dt) = 0;

    void takeDamage(int damage);
    bool isDead() const;
    int getCurrencyDrop() const;

protected:
    float speed;
    int health;
    int maxHealth;
    int currencyDrop;
    
    // Shooting logic helper
    float reloadTimer = 0.0f;
    float reloadTime = 2.0f;
};

// --- Derived Classes ---

class Triangle : public Enemy
{
public:
    Triangle(sf::Vector2f position);
    std::vector<Bullet> update(sf::Vector2f playerPos, float dt) override;
};

class Circle : public Enemy
{
public:
    Circle(sf::Vector2f position);
    std::vector<Bullet> update(sf::Vector2f playerPos, float dt) override;
private:
    float moveTimer = 0.0f;
    float stopTimer = 0.0f;
    bool isMoving = true;
};

class Square : public Enemy
{
public:
    Square(sf::Vector2f position);
    std::vector<Bullet> update(sf::Vector2f playerPos, float dt) override;
private:
    float moveTimer = 0.0f;
    bool isMoving = false;
    sf::Vector2f dashDirection;
};

class Spiker : public Enemy
{
public:
    Spiker(sf::Vector2f position);
    std::vector<Bullet> update(sf::Vector2f playerPos, float dt) override;
};
