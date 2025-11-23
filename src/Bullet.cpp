#include "../include/Bullet.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f vel)
{
    shape.setRadius(3.0f);
    shape.setOrigin(sf::Vector2f(3.0f, 3.0f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    velocity = vel;
}

void Bullet::update()
{
    shape.move(velocity);
}

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(shape);
}

sf::Vector2f Bullet::getPosition() const { return shape.getPosition(); }
float Bullet::getRadius() const { return shape.getRadius(); }
