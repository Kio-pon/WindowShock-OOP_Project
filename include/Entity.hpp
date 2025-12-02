#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Barrel
{
    float length;
    float width;
    float offset; // Distance from center
    float angle;  // Angle offset in degrees
    float recoil; // Current recoil offset
    float maxRecoil;
    float recoilRecovery;
};

class Entity
{
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius;
    float rotation; // In degrees
    sf::Color bodyColor;
    sf::Color barrelColor;
    
    std::vector<Barrel> barrels;

public:
    Entity(sf::Vector2f pos, float r, sf::Color col);
    virtual ~Entity() = default;

    virtual void update(float dt);
    virtual void draw(sf::RenderWindow &window);

    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f pos);
    
    float getRadius() const;
    void setRadius(float r);

    float getRotation() const;

    void setRotation(float angle);

    void setColor(sf::Color col);

    void addBarrel(float len, float wid, float off = 0.0f, float ang = 0.0f);
    void clearBarrels();
    
    // Apply recoil to a specific barrel (e.g. when shooting)
    void applyRecoil(size_t barrelIndex, float amount);
};
