#include "../include/Bullet.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f vel)
    : Entity(position, 6.0f, sf::Color(0, 178, 225)) // Diep.io Blue
{
    velocity = vel;
}
