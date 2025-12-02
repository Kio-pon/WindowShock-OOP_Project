#include "../include/Enemy.hpp"
#include <cmath>

Enemy::Enemy(sf::Vector2f position, float speedVal, int hp, int currency, EnemyType type)
    : Entity(position, 20.0f, sf::Color::Red), speed(speedVal), health(hp), maxHealth(hp), currencyDrop(currency), type(type)
{
    // Customize based on type
    switch (type)
    {
    case EnemyType::Triangle:
        setRadius(15.0f); // Smaller
        setColor(sf::Color(255, 255, 0)); // Yellow
        // 3 sides for triangle? Visuals are circles for now, maybe change later
        break;
    case EnemyType::Circle:
        setRadius(10.0f); // Smaller
        setColor(sf::Color(0, 0, 255)); // Blue
        moveTimer = 1.0f; // Move for 1s
        break;
    case EnemyType::Square:
        setRadius(18.0f); // Smaller
        setColor(sf::Color(0, 255, 0)); // Green
        moveTimer = 2.0f; // Wait 2s before dash
        break;
    case EnemyType::Spiker: // Boss
        setRadius(50.0f); // Slightly smaller boss
        setColor(sf::Color(50, 50, 50)); // Dark Grey
        // 8 barrels
        for(int i=0; i<8; ++i)
        {
            addBarrel(50.0f, 15.0f, 0.0f, i * 45.0f);
        }
        reloadTime = 0.8f;
        break;
    }
}

std::vector<Bullet> Enemy::update(sf::Vector2f playerPos, float dt)
{
    std::vector<Bullet> newBullets;
    
    sf::Vector2f pos = getPosition();
    sf::Vector2f dir = playerPos - pos;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    sf::Vector2f normDir = (len != 0) ? dir / len : sf::Vector2f(0,0);
    
    // Movement Logic
    if (type == EnemyType::Triangle)
    {
        // Moves towards player, slows down when close
        float currentSpeed = speed;
        if (len < 150.0f) currentSpeed *= 0.3f; // Slow down
        
        if (len != 0)
        {
            sf::Vector2f velocity = normDir * currentSpeed;
            setPosition(pos + velocity * dt);
            
            float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159f;
            setRotation(angle);
        }
    }
    else if (type == EnemyType::Circle)
    {
        // Burst movement: Move fast, then stop
        if (isMoving)
        {
            moveTimer -= dt;
            if (moveTimer <= 0.0f)
            {
                isMoving = false;
                stopTimer = 0.5f; // Stop for 0.5s
            }
            
            if (len != 0)
            {
                sf::Vector2f velocity = normDir * speed; // Speed is high
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
    }
    else if (type == EnemyType::Square)
    {
        // Dash behavior
        // Wait, then dash
        if (!isMoving) // Waiting state
        {
            moveTimer -= dt; // Wait timer
            // Face player while waiting
            float angle = std::atan2(normDir.y, normDir.x) * 180.0f / 3.14159f;
            setRotation(angle);
            
            if (moveTimer <= 0.0f)
            {
                isMoving = true; // Start dash
                moveTimer = 0.3f; // Dash duration
                dashDirection = normDir; // Lock direction
            }
        }
        else // Dashing state
        {
            moveTimer -= dt;
            sf::Vector2f velocity = dashDirection * (speed * 3.0f); // Fast dash
            setPosition(pos + velocity * dt);
            
            if (moveTimer <= 0.0f)
            {
                isMoving = false;
                moveTimer = 2.0f; // Cooldown
            }
        }
    }
    else if (type == EnemyType::Spiker)
    {
        // Boss: Very slow movement towards player
        if (len != 0)
        {
            sf::Vector2f velocity = normDir * (speed * 0.2f);
            setPosition(pos + velocity * dt);
            
            // Rotate slowly
            float currentRot = getRotation();
            setRotation(currentRot + 30.0f * dt);
        }
        
        // Shooting
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
    }

    Entity::update(dt);
    return newBullets;
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
