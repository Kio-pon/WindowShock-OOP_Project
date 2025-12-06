#include "../include/Entity.hpp"

Entity::Entity(sf::Vector2f pos, float r, sf::Color col)
    : position(pos), radius(r), bodyColor(col), rotation(0.0f)
{
    barrelColor = sf::Color(153, 153, 153);
}

void Entity::update(float dt)
{
    position += velocity * dt;
}

void Entity::draw(sf::RenderWindow &window)
{
    // Draw barrels first to layer them beneath the body
    for (const auto &b : barrels)
    {
        sf::RectangleShape barrelShape(sf::Vector2f(b.length, b.width));
        barrelShape.setOrigin(sf::Vector2f(0.0f, b.width / 2.0f));
        barrelShape.setFillColor(barrelColor);
        barrelShape.setOutlineThickness(2.0f);
        barrelShape.setOutlineColor(sf::Color(85, 85, 85));

        // Calculate barrel transform
        float totalAngle = rotation + b.angle;
        float radAngle = totalAngle * 3.14159f / 180.0f;
        
        // Apply recoil offset
        float recoilOffset = b.recoil;
        
        // Compute barrel position using local coordinate axes
        sf::Vector2f forward(std::cos(radAngle), std::sin(radAngle));
        sf::Vector2f right(-forward.y, forward.x);
        
        sf::Vector2f barrelPos = position + right * b.offset - forward * recoilOffset;
        
        barrelShape.setPosition(barrelPos);
        barrelShape.setRotation(sf::degrees(totalAngle));
        
        window.draw(barrelShape);
    }

    // Draw body
    sf::CircleShape body(radius);
    body.setOrigin(sf::Vector2f(radius, radius));
    body.setPosition(position);
    body.setFillColor(bodyColor);
    body.setOutlineThickness(3.0f);
    body.setOutlineColor(sf::Color(85, 85, 85));
    
    window.draw(body);
}

sf::Vector2f Entity::getPosition() const
{
    return position;
}

void Entity::setPosition(sf::Vector2f pos)
{
    position = pos;
}

float Entity::getRadius() const
{
    return radius;
}

void Entity::setRadius(float r)
{
    radius = r;
}

float Entity::getRotation() const
{
    return rotation;
}

void Entity::setRotation(float angle)
{
    rotation = angle;
}

void Entity::setColor(sf::Color col)
{
    bodyColor = col;
}

void Entity::addBarrel(float len, float wid, float off, float ang)
{
    Barrel b;
    b.length = len;
    b.width = wid;
    b.offset = off;
    b.angle = ang;
    b.recoil = 0.0f;
    b.maxRecoil = 0.0f;
    b.recoilRecovery = 0.0f;
    barrels.push_back(b);
}

void Entity::clearBarrels()
{
    barrels.clear();
}

void Entity::applyRecoil(size_t barrelIndex, float amount)
{
    if (barrelIndex < barrels.size())
    {
        barrels[barrelIndex].recoil = amount;
    }
}
