#include "../include/Bullet.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f vel, int dmg)
    : Entity(position, 8.0f, sf::Color::Yellow), damage(dmg)
{
    velocity = vel;
}
