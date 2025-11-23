#include "../include/Player.hpp"
#include <cmath>
#include <algorithm>

Player::Player(float radius, float speed, float startX, float startY)
    : moveSpeed(speed)
{
    circle.setRadius(radius);
    circle.setOrigin(sf::Vector2f(radius, radius));
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(sf::Vector2f(startX, startY));
}

void Player::handleInput()
{
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += moveSpeed;

    circle.move(movement);
}

void Player::constrainToWindow(const FakeWindow &fw)
{
    sf::Vector2f pos = circle.getPosition();
    float r = circle.getRadius();

    if (pos.x - r < fw.getLeft())
        pos.x = fw.getLeft() + r;
    if (pos.x + r > fw.getRight())
        pos.x = fw.getRight() - r;
    if (pos.y - r < fw.getTop())
        pos.y = fw.getTop() + r;
    if (pos.y + r > fw.getBottom())
        pos.y = fw.getBottom() - r;

    circle.setPosition(pos);
}

sf::Vector2f Player::getPosition() const { return circle.getPosition(); }
float Player::getRadius() const { return circle.getRadius(); }

void Player::draw(sf::RenderWindow &window)
{
    window.draw(circle);
}

Bullet Player::createBullet(sf::Vector2f targetPos, float bulletSpeed)
{
    sf::Vector2f center = getPosition();
    sf::Vector2f direction = targetPos - center;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0)
    {
        direction /= length;
        direction *= bulletSpeed;
    }

    return Bullet(center, direction);
}

void Player::earnCurrency(int amount) { currency += amount; }
void Player::takeDamage(int damage) { health = std::max(0, health - damage); }
bool Player::isDead() const { return health <= 0; }

void Player::applyUpgrade(int upgradeIndex)
{
    switch (upgradeIndex)
    {
    case 0:
        splashDamageLevel++;
        break;
    case 1:
        speedLevel++;
        moveSpeed += 0.5f;
        break;
    case 2:
        multiShotLevel++;
        break;
    }
}
