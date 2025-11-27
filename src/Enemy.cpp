#include "../include/Enemy.hpp"
#include <cmath>

Enemy::Enemy(sf::Vector2f position, float speedVal, int hp, int currency)
    : Entity(position, 11.25f, sf::Color(241, 78, 84)), // Diep.io Red
      speed(speedVal), health(hp), currencyDrop(currency)
{
}

void Enemy::update(sf::Vector2f playerPos, float dt)
{
    sf::Vector2f direction = playerPos - getPosition();
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0)
    {
        direction /= length;
        velocity = direction * speed;
    }
    else
    {
        velocity = sf::Vector2f(0.0f, 0.0f);
    }

    // Call base update to apply velocity
    Entity::update(dt);
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
}

bool Enemy::isDead() const
{
    return health <= 0;
}

int Enemy::getCurrencyDrop() const
{
    return currencyDrop;
}
