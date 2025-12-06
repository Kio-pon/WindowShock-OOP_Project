#include "../include/Enemy.hpp"
#include <cmath>

// --- Base Enemy ---

Enemy::Enemy(sf::Vector2f position, float speedVal, int hp, int currency)
    : Entity(position, 20.0f, sf::Color::Red), speed(speedVal), health(hp), maxHealth(hp), currencyDrop(currency)
{
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

// --- Triangle Enemy ---

Triangle::Triangle(sf::Vector2f position)
    : Enemy(position, 100.0f, 30, 10)
{
    setRadius(15.0f);
    setColor(sf::Color(255, 255, 0));
}

std::vector<Bullet> Triangle::update(sf::Vector2f playerPos, float dt)
{
    sf::Vector2f pos = getPosition();
    sf::Vector2f dir = playerPos - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir = (len != 0) ? dir / len : sf::Vector2f(0,0);

    float currentSpeed = speed;
    if (len < 150.0f) currentSpeed *= 0.3f; // Reduce speed at close range

    if (len != 0)
    {
        sf::Vector2f velocity = normDir * currentSpeed;
        setPosition(pos + velocity * dt);
        
        float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159f;
        setRotation(angle);
    }
    
    Entity::update(dt);
    return {};
}

// --- Circle Enemy ---

Circle::Circle(sf::Vector2f position)
    : Enemy(position, 250.0f, 10, 20)
{
    setRadius(10.0f);
    setColor(sf::Color(0, 0, 255));
    moveTimer = 1.0f;
}

std::vector<Bullet> Circle::update(sf::Vector2f playerPos, float dt)
{
    sf::Vector2f pos = getPosition();
    sf::Vector2f dir = playerPos - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir = (len != 0) ? dir / len : sf::Vector2f(0,0);

    if (isMoving)
    {
        moveTimer -= dt;
        if (moveTimer <= 0.0f)
        {
            isMoving = false;
            stopTimer = 0.5f;
        }
        
        if (len != 0)
        {
            sf::Vector2f velocity = normDir * speed;
            setPosition(pos + velocity * dt);
            float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159f;
            setRotation(angle);
        }
    }
    else
    {
        stopTimer -= dt;
        if (stopTimer <= 0.0f)
        {
            isMoving = true;
            moveTimer = 1.0f;
        }
    }

    Entity::update(dt);
    return {};
}

// --- Square Enemy ---

Square::Square(sf::Vector2f position)
    : Enemy(position, 200.0f, 50, 15)
{
    setRadius(18.0f);
    setColor(sf::Color(0, 255, 0));
    moveTimer = 2.0f;
}

std::vector<Bullet> Square::update(sf::Vector2f playerPos, float dt)
{
    sf::Vector2f pos = getPosition();
    sf::Vector2f dir = playerPos - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir = (len != 0) ? dir / len : sf::Vector2f(0,0);

    if (!isMoving) // Preparation phase
    {
        moveTimer -= dt;
        float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159f;
        setRotation(angle);
        
        if (moveTimer <= 0.0f)
        {
            isMoving = true;
            moveTimer = 0.3f; // Dash duration
            dashDirection = normDir;
        }
    }
    else // Dashing phase
    {
        moveTimer -= dt;
        sf::Vector2f velocity = dashDirection * (speed * 3.0f);
        setPosition(pos + velocity * dt);
        
        if (moveTimer <= 0.0f)
        {
            isMoving = false;
            moveTimer = 2.0f; // Cooldown duration
        }
    }

    Entity::update(dt);
    return {};
}

// --- Spiker (Boss) ---

Spiker::Spiker(sf::Vector2f position)
    : Enemy(position, 30.0f, 500, 500)
{
    setRadius(50.0f);
    setColor(sf::Color(50, 50, 50));
    
    // Configure multi-barrel setup
    for(int i=0; i<8; ++i)
    {
        addBarrel(50.0f, 15.0f, 0.0f, i * 45.0f);
    }
    reloadTime = 0.8f;
}

std::vector<Bullet> Spiker::update(sf::Vector2f playerPos, float dt)
{
    std::vector<Bullet> newBullets;
    sf::Vector2f pos = getPosition();
    sf::Vector2f dir = playerPos - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir = (len != 0) ? dir / len : sf::Vector2f(0,0);

    // Movement logic
    if (len != 0)
    {
        sf::Vector2f velocity = normDir * speed;
        setPosition(pos + velocity * dt);
        
        // Constant rotation
        float currentRot = getRotation();
        setRotation(currentRot + 30.0f * dt);
    }
    
    // Shooting logic
    if (reloadTimer > 0.0f)
    {
        reloadTimer -= dt;
    }
    else
    {
        float baseAngle = getRotation();
        
        for (size_t i = 0; i < barrels.size(); ++i)
        {
            const auto& b = barrels[i];
            applyRecoil(i, 5.0f);
            
            float totalAngle = baseAngle + b.angle;
            float radAngle = totalAngle * 3.14159f / 180.0f;
            sf::Vector2f bDir(std::cos(radAngle), std::sin(radAngle));
            
            sf::Vector2f forward = bDir;
            sf::Vector2f right(-forward.y, forward.x);
            sf::Vector2f spawnPos = getPosition() + forward * b.length + right * b.offset;
            
            newBullets.emplace_back(spawnPos, bDir * 200.0f, 15);
        }
        reloadTimer = reloadTime;
    }

    Entity::update(dt);
    return newBullets;
}
