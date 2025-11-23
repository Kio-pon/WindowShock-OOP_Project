#include "../include/Enemy.hpp"

#include <cmath>

Enemy::Enemy(sf::Vector2f position, float speedVal, int hp, int currency)
    : speed(speedVal), health(hp), currencyDrop(currency)
{
    shape.setRadius(12.0f);
    shape.setFillColor(sf::Color(0, 255, 0));
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2);
    shape.setOrigin(sf::Vector2f(12.0f, 12.0f));
    shape.setPosition(position);
}

void Enemy::update(sf::Vector2f playerPos)
{
    sf::Vector2f dir = playerPos - shape.getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
    {
        velocity = (dir / len) * speed;
        shape.move(velocity);
    }
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

sf::Vector2f Enemy::getPosition() const { return shape.getPosition(); }
float Enemy::getRadius() const { return shape.getRadius(); }
void Enemy::takeDamage(int damage) { health -= damage; }
bool Enemy::isDead() const { return health <= 0; }
int Enemy::getCurrencyDrop() const { return currencyDrop; }
