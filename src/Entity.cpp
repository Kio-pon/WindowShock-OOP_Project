#include "../include/Entity.hpp"

Entity::Entity(sf::Vector2f pos, float r, sf::Color col)
    : position(pos), radius(r), rotation(0.0f), bodyColor(col)
{
    barrelColor = sf::Color(153, 153, 153); // Standard grey for barrels
}

void Entity::update(float dt)
{
    position += velocity * dt;

    // Recover recoil
    for (auto &b : barrels)
    {
        if (b.recoil > 0)
        {
            b.recoil -= b.recoilRecovery * dt;
            if (b.recoil < 0) b.recoil = 0;
        }
    }
}

void Entity::draw(sf::RenderWindow &window)
{
    // Draw barrels first (so they are under the body)
    for (const auto &b : barrels)
    {
        sf::RectangleShape barrelShape(sf::Vector2f(b.length, b.width));
        barrelShape.setOrigin(sf::Vector2f(0, b.width / 2.0f)); // Pivot at the base center
        barrelShape.setFillColor(barrelColor);
        barrelShape.setOutlineColor(sf::Color(85, 85, 85)); // Darker grey outline
        barrelShape.setOutlineThickness(2.0f);

        // Calculate barrel position based on entity rotation and barrel offset
        float radAngle = (rotation + b.angle) * 3.14159f / 180.0f;
        
        // Apply recoil: move barrel backwards along its angle
        // Actually, we want to draw the barrel at the entity position, rotated, 
        // but shifted slightly inwards by recoil.
        // The barrel is attached to the body.
        
        // Let's simplify: The barrel is a rectangle. 
        // Position: Entity center.
        // Rotation: Entity rotation + Barrel angle.
        // Offset: Move it 'offset' distance sideways if needed (for twin), 
        // and 'radius - recoil' distance forwards? No, usually starts inside.
        
        // Standard Diep barrel starts at center (or slightly offset) and extends out.
        // Let's assume 'offset' is lateral offset (like Twin).
        // And we need a forward offset which is usually radius * 0.8 or something.
        
        // Correct approach:
        // 1. Start at Entity Center.
        // 2. Rotate by (Rotation + BarrelAngle).
        // 3. Move forward by (Radius - Recoil).
        // 4. Move laterally by (Offset).
        
        // Wait, 'offset' in my struct usually means lateral offset for Twin tanks.
        // Let's use SFML transforms for easier handling.
        
        sf::Transform transform;
        transform.translate(position);
        transform.rotate(sf::degrees(rotation + b.angle));
        
        // Lateral offset (y-axis in local coords)
        transform.translate(sf::Vector2f(0, b.offset));
        
        // Recoil (x-axis in local coords, negative)
        transform.translate(sf::Vector2f(-b.recoil, 0));
        
        // Draw
        window.draw(barrelShape, transform);
    }

    // Draw body
    sf::CircleShape body(radius);
    body.setOrigin(sf::Vector2f(radius, radius));
    body.setPosition(position);
    body.setFillColor(bodyColor);
    
    // Darker outline
    sf::Color outlineCol = sf::Color(
        std::max(0, bodyColor.r - 40),
        std::max(0, bodyColor.g - 40),
        std::max(0, bodyColor.b - 40)
    );
    body.setOutlineColor(outlineCol);
    body.setOutlineThickness(3.0f);
    
    window.draw(body);
}

sf::Vector2f Entity::getPosition() const { return position; }
void Entity::setPosition(sf::Vector2f pos) { position = pos; }

float Entity::getRadius() const { return radius; }
void Entity::setRadius(float r) { radius = r; }

float Entity::getRotation() const { return rotation; }
void Entity::setRotation(float angle) { rotation = angle; }

void Entity::addBarrel(float len, float wid, float off, float ang)
{
    Barrel b;
    b.length = len;
    b.width = wid;
    b.offset = off;
    b.angle = ang;
    b.recoil = 0.0f;
    b.maxRecoil = 5.0f; // Default
    b.recoilRecovery = 20.0f; // Default
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
