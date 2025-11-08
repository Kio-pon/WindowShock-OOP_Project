#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

// ============================================================================
// GAME STATES
// ============================================================================
enum class GameState {
    WELCOME,
    SHOP,
    PLAYING,
    GAMEOVER
};

// ============================================================================
// UPGRADE SYSTEM
// ============================================================================
struct Upgrade {
    std::string name;
    std::string description;
    int cost;
    int level;
    int maxLevel;
    bool locked;
    
    Upgrade(std::string n, std::string desc, int c, int max = 10) 
        : name(n), description(desc), cost(c), level(1), maxLevel(max), locked(false) {}
};

// ============================================================================
// PLAYER STATS
// ============================================================================
struct GameStats {
    int coinsCollected = 0;
    int starsCollected = 0;
    int timeSurvived = 0;
    int bulletsFired = 0;
    int enemiesKilled = 0;
    int bossesKilled = 0;
};

// ============================================================================
// ENEMY CLASS
// ============================================================================
class Enemy {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int health;
    int currencyDrop;

public:
    Enemy(sf::Vector2f position, sf::Vector2f vel, int hp = 3, int currency = 5)
        : velocity(vel), health(hp), currencyDrop(currency)
    {
        shape.setRadius(12.0f);  // Increased from 8 to 12
        shape.setFillColor(sf::Color(0, 255, 0));  // Bright green
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2);
        shape.setPosition(position);
    }

    void update() {
        sf::Vector2f pos = shape.getPosition();
        pos += velocity;
        shape.setPosition(pos);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }

    float getRadius() const {
        return shape.getRadius();
    }

    void takeDamage(int damage) {
        health -= damage;
    }

    bool isDead() const {
        return health <= 0;
    }

    int getCurrencyDrop() const {
        return currencyDrop;
    }
};

// ============================================================================
// GAME WINDOW CLASS
// ============================================================================
class GameWindow
{
private:
    sf::RenderWindow window;
    HWND hwnd;
    POINT screenCenter;
    POINT lastScreenCenter;

    float leftWall, rightWall, topWall, bottomWall;
    float targetLeft, targetRight, targetTop, targetBottom;

    const float minSize = 150.0f;
    const float maxSize = 600.0f;
    const float expandAmount = 10.0f;
    const float collapseSpeed = 0.2f;
    const float expandSpeed = 10.0f;

    sf::Vector2f size;
    
    // Animation state
    bool isAnimating;
    float animationProgress;
    float animationDuration;
    float startSize;
    float targetSize;
    bool isCollapsing;

public:
    GameWindow(int initialSize, int screenCenterX, int screenCenterY)
        : isAnimating(false), animationProgress(0.0f), animationDuration(0.0f),
          startSize(0.0f), targetSize(0.0f), isCollapsing(false)
    {
        sf::VideoMode mode({static_cast<unsigned int>(initialSize), static_cast<unsigned int>(initialSize)});
        window.create(mode, "WindowShock Game", sf::State::Windowed);
        window.setFramerateLimit(60);

        hwnd = window.getNativeHandle();
        screenCenter.x = screenCenterX;
        screenCenter.y = screenCenterY;
        lastScreenCenter = screenCenter;

        float halfSize = initialSize / 2.0f;
        leftWall = rightWall = topWall = bottomWall = halfSize;
        targetLeft = targetRight = targetTop = targetBottom = halfSize;

        size = sf::Vector2f(static_cast<float>(initialSize), static_cast<float>(initialSize));

        int posX = screenCenterX - (initialSize / 2);
        int posY = screenCenterY - (initialSize / 2);
        SetWindowPos(hwnd, nullptr, posX, posY, initialSize, initialSize, SWP_NOZORDER);
    }

    void resizeWindow(int newSize) {
        float halfSize = newSize / 2.0f;
        leftWall = rightWall = topWall = bottomWall = halfSize;
        targetLeft = targetRight = targetTop = targetBottom = halfSize;
        
        size = sf::Vector2f(static_cast<float>(newSize), static_cast<float>(newSize));
        
        int posX = screenCenter.x - (newSize / 2);
        int posY = screenCenter.y - (newSize / 2);
        SetWindowPos(hwnd, nullptr, posX, posY, newSize, newSize, SWP_NOZORDER);
    }

    void startCollapseAnimation() {
        isAnimating = true;
        animationProgress = 0.0f;
        animationDuration = 3.0f;
        startSize = (leftWall + rightWall + topWall + bottomWall) / 4.0f;
        targetSize = 150.0f;
        isCollapsing = true;
    }

    void updateAnimation(float dt) {
        if (!isAnimating) return;

        animationProgress += dt / animationDuration;

        if (animationProgress >= 1.0f) {
            if (isCollapsing) {
                isCollapsing = false;
                animationProgress = 0.0f;
                startSize = targetSize;
                targetSize = 300.0f;
            } else {
                isAnimating = false;
                animationProgress = 1.0f;
            }
        }

        float t = animationProgress;
        float easedProgress;
        
        if (isCollapsing) {
            easedProgress = t * t;
        } else {
            easedProgress = 1.0f - (1.0f - t) * (1.0f - t);
        }

        float currentSize = startSize + (targetSize - startSize) * easedProgress;
        
        leftWall = rightWall = topWall = bottomWall = currentSize;
        targetLeft = targetRight = targetTop = targetBottom = currentSize;

        unsigned int windowSize = static_cast<unsigned int>(currentSize * 2);
        int posX = screenCenter.x - static_cast<int>(currentSize);
        int posY = screenCenter.y - static_cast<int>(currentSize);

        SetWindowPos(hwnd, nullptr, posX, posY, windowSize, windowSize, SWP_NOZORDER);
        size = sf::Vector2f(static_cast<float>(windowSize), static_cast<float>(windowSize));
    }

    bool isAnimationComplete() const {
        return !isAnimating;
    }

    void shrink(float dt)
    {
        bool changed = false;

        if (targetLeft > minSize)
            targetLeft = std::max(targetLeft - collapseSpeed, minSize);
        if (targetRight > minSize)
            targetRight = std::max(targetRight - collapseSpeed, minSize);
        if (targetTop > minSize)
            targetTop = std::max(targetTop - collapseSpeed, minSize);
        if (targetBottom > minSize)
            targetBottom = std::max(targetBottom - collapseSpeed, minSize);

        float leftDist = (leftWall > targetLeft) ? leftWall - targetLeft : 0.0f;
        float rightDist = (rightWall > targetRight) ? rightWall - targetRight : 0.0f;
        float topDist = (topWall > targetTop) ? topWall - targetTop : 0.0f;
        float bottomDist = (bottomWall > targetBottom) ? bottomWall - targetBottom : 0.0f;

        float maxCollapseDist = std::max({leftDist, rightDist, topDist, bottomDist});

        if (maxCollapseDist > 0.0f)
        {
            float framesNeeded = maxCollapseDist / collapseSpeed;

            if (leftDist > 0.0f)
            {
                float speed = leftDist / framesNeeded;
                leftWall = std::max(leftWall - speed, targetLeft);
                changed = true;
            }
            if (rightDist > 0.0f)
            {
                float speed = rightDist / framesNeeded;
                rightWall = std::max(rightWall - speed, targetRight);
                changed = true;
            }
            if (topDist > 0.0f)
            {
                float speed = topDist / framesNeeded;
                topWall = std::max(topWall - speed, targetTop);
                changed = true;
            }
            if (bottomDist > 0.0f)
            {
                float speed = bottomDist / framesNeeded;
                bottomWall = std::max(bottomWall - speed, targetBottom);
                changed = true;
            }
        }

        if (leftWall < targetLeft)
        {
            leftWall = std::min(leftWall + expandSpeed, targetLeft);
            changed = true;
        }
        if (rightWall < targetRight)
        {
            rightWall = std::min(rightWall + expandSpeed, targetRight);
            changed = true;
        }
        if (topWall < targetTop)
        {
            topWall = std::min(topWall + expandSpeed, targetTop);
            changed = true;
        }
        if (bottomWall < targetBottom)
        {
            bottomWall = std::min(bottomWall + expandSpeed, targetBottom);
            changed = true;
        }

        if (changed)
        {
            unsigned int windowWidth = static_cast<unsigned int>(leftWall + rightWall);
            unsigned int windowHeight = static_cast<unsigned int>(topWall + bottomWall);

            int posX = screenCenter.x - static_cast<int>(leftWall);
            int posY = screenCenter.y - static_cast<int>(topWall);

            SetWindowPos(hwnd, nullptr, posX, posY, windowWidth, windowHeight, SWP_NOZORDER);

            size.x = static_cast<float>(windowWidth);
            size.y = static_cast<float>(windowHeight);
        }
    }

    void handleEdgeHit(sf::Vector2f bulletPos, float bulletRadius)
    {
        float windowWidth = leftWall + rightWall;
        float windowHeight = topWall + bottomWall;

        if (bulletPos.x - bulletRadius <= 0)
            expandLeftWall();
        if (bulletPos.x + bulletRadius >= windowWidth)
            expandRightWall();
        if (bulletPos.y - bulletRadius <= 0)
            expandTopWall();
        if (bulletPos.y + bulletRadius >= windowHeight)
            expandBottomWall();
    }

    sf::RenderWindow &getWindow() { return window; }
    sf::Vector2f getSize() const { return size; }

    sf::Vector2i getScreenCenterShift()
    {
        sf::Vector2i shift(
            screenCenter.x - lastScreenCenter.x,
            screenCenter.y - lastScreenCenter.y);
        lastScreenCenter = screenCenter;
        return shift;
    }

    bool isOutOfBounds(sf::Vector2f position) const
    {
        return position.x < -10 || position.x > size.x + 10 ||
               position.y < -10 || position.y > size.y + 10;
    }

    sf::Vector2f getWallChanges() const
    {
        return sf::Vector2f(0.0f, 0.0f);
    }

private:
    void expandLeftWall()
    {
        if (targetLeft < maxSize)
        {
            float shift = expandAmount / 1.0f;
            targetLeft = std::min(targetLeft + expandAmount, maxSize);
            targetRight = std::min(targetRight + shift, maxSize);
            screenCenter.x -= static_cast<int>(shift);
        }
    }

    void expandRightWall()
    {
        if (targetRight < maxSize)
        {
            float shift = expandAmount / 1.0f;
            targetRight = std::min(targetRight + expandAmount, maxSize);
            targetLeft = std::min(targetLeft + shift, maxSize);
            screenCenter.x += static_cast<int>(shift);
        }
    }

    void expandTopWall()
    {
        if (targetTop < maxSize)
        {
            float shift = expandAmount / 1.0f;
            targetTop = std::min(targetTop + expandAmount, maxSize);
            targetBottom = std::min(targetBottom + shift, maxSize);
            screenCenter.y -= static_cast<int>(shift);
        }
    }

    void expandBottomWall()
    {
        if (targetBottom < maxSize)
        {
            float shift = expandAmount / 1.0f;
            targetBottom = std::min(targetBottom + expandAmount, maxSize);
            targetTop = std::min(targetTop + shift, maxSize);
            screenCenter.y += static_cast<int>(shift);
        }
    }
};

// ============================================================================
// BULLET CLASS
// ============================================================================
class Bullet
{
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;

public:
    Bullet(sf::Vector2f position, sf::Vector2f vel)
    {
        shape.setRadius(3.0f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(position);
        velocity = vel;
    }

    void update()
    {
        sf::Vector2f pos = shape.getPosition();
        pos += velocity;
        shape.setPosition(pos);
    }

    void adjustPosition(sf::Vector2f offset)
    {
        sf::Vector2f pos = shape.getPosition();
        pos += offset;
        shape.setPosition(pos);
    }

    void draw(sf::RenderWindow &window) { window.draw(shape); }
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    float getRadius() const { return shape.getRadius(); }
};

// ============================================================================
// PLAYER CLASS
// ============================================================================
class Player
{
private:
    sf::CircleShape circle;
    float moveSpeed;
    sf::Vector2f offsetFromCenter;

public:
    int currency = 83;
    int health = 100;
    int maxHealth = 100;
    
    int splashDamageLevel = 0;
    int speedLevel = 0;
    int multiShotLevel = 0;
    
    Player(float radius = 25.0f, float speed = 5.0f, float startX = 0.0f, float startY = 0.0f)
        : moveSpeed(speed)
    {
        circle.setRadius(radius);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(sf::Vector2f(startX, startY));
        offsetFromCenter = sf::Vector2f(0.0f, 0.0f);
    }

    void handleInput()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            offsetFromCenter.y -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            offsetFromCenter.y += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            offsetFromCenter.x -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            offsetFromCenter.x += moveSpeed;
    }

    void updatePosition(const GameWindow &gameWindow)
    {
        float radius = circle.getRadius();
        sf::Vector2f windowSize = gameWindow.getSize();
        sf::Vector2f windowCenter(windowSize.x / 2.0f, windowSize.y / 2.0f);

        sf::Vector2f desiredPos = windowCenter + offsetFromCenter;

        float playerLeft = desiredPos.x;
        float playerRight = desiredPos.x + radius * 2;
        float playerTop = desiredPos.y;
        float playerBottom = desiredPos.y + radius * 2;

        float windowLeft = 0;
        float windowRight = windowSize.x;
        float windowTop = 0;
        float windowBottom = windowSize.y;

        if (playerLeft < windowLeft)
        {
            desiredPos.x = windowLeft;
            offsetFromCenter.x = desiredPos.x - windowCenter.x;
        }
        if (playerRight > windowRight)
        {
            desiredPos.x = windowRight - radius * 2;
            offsetFromCenter.x = desiredPos.x - windowCenter.x;
        }
        if (playerTop < windowTop)
        {
            desiredPos.y = windowTop;
            offsetFromCenter.y = desiredPos.y - windowCenter.y;
        }
        if (playerBottom > windowBottom)
        {
            desiredPos.y = windowBottom - radius * 2;
            offsetFromCenter.y = desiredPos.y - windowCenter.y;
        }

        circle.setPosition(desiredPos);
    }

    void compensateForWindowShift(sf::Vector2i shiftAmount)
    {
        offsetFromCenter.x -= static_cast<float>(shiftAmount.x);
        offsetFromCenter.y -= static_cast<float>(shiftAmount.y);
    }

    sf::Vector2f getPosition() const { return circle.getPosition(); }
    float getRadius() const { return circle.getRadius(); }
    void draw(sf::RenderWindow &window) { window.draw(circle); }

    sf::Vector2f getCenter() const
    {
        sf::Vector2f pos = circle.getPosition();
        float radius = circle.getRadius();
        return sf::Vector2f(pos.x + radius, pos.y + radius);
    }

    Bullet createBullet(sf::Vector2f targetPos, float bulletSpeed)
    {
        sf::Vector2f center = getCenter();
        sf::Vector2f direction = targetPos - center;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (length > 0)
        {
            direction.x = (direction.x / length) * bulletSpeed;
            direction.y = (direction.y / length) * bulletSpeed;
        }

        return Bullet(center, direction);
    }
    
    void earnCurrency(int amount) {
        currency += amount;
    }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
    
    bool isDead() const {
        return health <= 0;
    }
    
    void applyUpgrade(int upgradeIndex) {
        switch(upgradeIndex) {
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
};

// ============================================================================
// UI RENDERING
// ============================================================================
class UIRenderer {
public:
    static void drawWelcomeScreen(sf::RenderWindow& window, const sf::Font& font) {
        sf::Vector2u size = window.getSize();
        
        sf::RectangleShape background(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
        background.setFillColor(sf::Color(0, 0, 0, 230));
        window.draw(background);
        
        sf::Text title(font, "WINDOWSHOCK", 80);
        title.setFillColor(sf::Color(138, 43, 226));
        title.setStyle(sf::Text::Bold);
        centerText(title, size.x / 2.0f, size.y / 3.0f);
        window.draw(title);
        
        sf::Text instructions(font, "Press SPACE to Start", 30);
        instructions.setFillColor(sf::Color::White);
        centerText(instructions, size.x / 2.0f, size.y / 2.0f);
        window.draw(instructions);
        
        sf::Text info(font, "WASD - Move | Mouse - Aim | Click - Shoot | TAB - Shop", 20);
        info.setFillColor(sf::Color(150, 150, 150));
        centerText(info, size.x / 2.0f, size.y / 2.0f + 60);
        window.draw(info);
    }
    
    static void drawShopScreen(sf::RenderWindow& window, const sf::Font& font, Player& player, 
                               std::vector<Upgrade>& upgrades, sf::Vector2i mousePos, bool isOverlay = false) {
        sf::Vector2u size = window.getSize();
        
        sf::RectangleShape background(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
        background.setFillColor(isOverlay ? sf::Color(0, 0, 0, 200) : sf::Color(0, 0, 0, 230));
        window.draw(background);
        
        sf::CircleShape dot(8);
        dot.setFillColor(sf::Color(138, 43, 226));
        dot.setPosition(sf::Vector2f(size.x / 2.0f - 80, 50));
        window.draw(dot);
        
        sf::Text currency(font, std::to_string(player.currency), 50);
        currency.setFillColor(sf::Color(138, 43, 226));
        currency.setStyle(sf::Text::Bold);
        currency.setPosition(sf::Vector2f(size.x / 2.0f - 40, 40));
        window.draw(currency);
        
        sf::Text perksText(font, "perks >", 35);
        perksText.setFillColor(sf::Color::White);
        perksText.setPosition(sf::Vector2f(size.x - 180, 50));
        window.draw(perksText);
        
        sf::Text shopTitle(font, isOverlay ? "SHOP (TAB to close)" : "SHOP (SPACE to play)", 30);
        shopTitle.setFillColor(sf::Color(150, 150, 150));
        centerText(shopTitle, size.x / 2.0f, 120);
        window.draw(shopTitle);
        
        float cardWidth = 200;
        float cardHeight = 220;
        float startX = (size.x - (cardWidth * 3 + 40)) / 2.0f;
        float startY = 180;
        
        sf::Color purple(138, 43, 226);
        sf::Color green(100, 200, 100);
        
        for (size_t i = 0; i < upgrades.size() && i < 3; i++) {
            float x = startX + i * (cardWidth + 20);
            float y = startY;
            
            bool isHovered = mousePos.x >= x && mousePos.x <= x + cardWidth &&
                           mousePos.y >= y && mousePos.y <= y + cardHeight;
            
            sf::RectangleShape card(sf::Vector2f(cardWidth, cardHeight));
            card.setPosition(sf::Vector2f(x, y));
            card.setFillColor(sf::Color::Transparent);
            card.setOutlineColor(isHovered ? sf::Color::White : sf::Color(100, 100, 100));
            card.setOutlineThickness(2);
            window.draw(card);
            
            sf::Text levelText(font, "lv. " + std::to_string(upgrades[i].level), 20);
            levelText.setFillColor(sf::Color::White);
            levelText.setPosition(sf::Vector2f(x + 10, y + 10));
            window.draw(levelText);
            
            // Draw icon based on upgrade type
            if (i == 0) { // Splash damage - diamond
                sf::RectangleShape diamond(sf::Vector2f(40, 40));
                diamond.setPosition(sf::Vector2f(x + 80, y + 60));
                diamond.setRotation(sf::degrees(45));
                diamond.setFillColor(sf::Color::Transparent);
                diamond.setOutlineColor(sf::Color(255, 100, 100));
                diamond.setOutlineThickness(3);
                window.draw(diamond);
            }
            else if (i == 1) { // Speed - arrow
                sf::RectangleShape arrow(sf::Vector2f(60, 8));
                arrow.setPosition(sf::Vector2f(x + 70, y + 75));
                arrow.setFillColor(sf::Color::White);
                window.draw(arrow);
                
                sf::CircleShape arrowHead(12, 3);
                arrowHead.setPosition(sf::Vector2f(x + 120, y + 75));
                arrowHead.setRotation(sf::degrees(-90));
                arrowHead.setFillColor(sf::Color::White);
                window.draw(arrowHead);
            }
            else if (i == 2) { // Multi shot - dots
                for (int row = 0; row < 3; row++) {
                    for (int col = 0; col < 3; col++) {
                        sf::CircleShape dotIcon(5);
                        dotIcon.setPosition(sf::Vector2f(x + 70 + col * 20, y + 70 + row * 20));
                        dotIcon.setFillColor(green);
                        window.draw(dotIcon);
                    }
                }
            }
            
            sf::Text nameText(font, upgrades[i].name, 22);
            nameText.setFillColor(sf::Color::White);
            nameText.setStyle(sf::Text::Bold);
            centerText(nameText, x + cardWidth / 2, y + 150);
            window.draw(nameText);
            
            sf::CircleShape costDot(6);
            costDot.setFillColor(purple);
            costDot.setPosition(sf::Vector2f(x + cardWidth / 2 - 30, y + 190));
            window.draw(costDot);
            
            sf::Text costText(font, std::to_string(upgrades[i].cost), 30);
            costText.setFillColor(purple);
            costText.setStyle(sf::Text::Bold);
            costText.setPosition(sf::Vector2f(x + cardWidth / 2 - 10, y + 180));
            window.draw(costText);
            
            if (upgrades[i].locked) {
                sf::Text lockText(font, "[LOCKED]", 18);
                lockText.setFillColor(sf::Color::Red);
                centerText(lockText, x + cardWidth / 2, y + cardHeight - 20);
                window.draw(lockText);
            }
        }
    }
    
    static void drawGameOverScreen(sf::RenderWindow& window, const sf::Font& font, const GameStats& stats) {
        sf::Vector2u size = window.getSize();
        
        sf::RectangleShape background(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
        background.setFillColor(sf::Color(0, 0, 0, 230));
        window.draw(background);
        
        sf::Text title(font, "game over", 80);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        centerText(title, size.x / 2.0f, 120);
        window.draw(title);
        
        float startY = 250;
        float spacing = 50;
        
        drawStat(window, font, "coins collected: ", std::to_string(stats.coinsCollected), 
                size.x / 2.0f, startY);
        drawStat(window, font, "stars collected: ", std::to_string(stats.starsCollected),
                size.x / 2.0f, startY + spacing);
        drawStat(window, font, "time survived: ", std::to_string(stats.timeSurvived),
                size.x / 2.0f, startY + spacing * 2);
        drawStat(window, font, "bullets fired: ", std::to_string(stats.bulletsFired),
                size.x / 2.0f, startY + spacing * 3);
        drawStat(window, font, "enemies killed: ", std::to_string(stats.enemiesKilled),
                size.x / 2.0f, startY + spacing * 4);
        drawStat(window, font, "bosses killed: ", std::to_string(stats.bossesKilled),
                size.x / 2.0f, startY + spacing * 5);
        
        sf::Text instructions(font, "close this window to try again", 25);
        instructions.setFillColor(sf::Color(150, 150, 150));
        centerText(instructions, size.x / 2.0f, size.y - 60);
        window.draw(instructions);
    }
    
private:
    static void centerText(sf::Text& text, float x, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
        text.setPosition(sf::Vector2f(x, y));
    }
    
    static void drawStat(sf::RenderWindow& window, const sf::Font& font, 
                        const std::string& label, const std::string& value, 
                        float x, float y) {
        sf::Text text(font, label + value, 28);
        text.setFillColor(sf::Color::White);
        centerText(text, x, y);
        window.draw(text);
    }
};

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int screenCenterX = screenWidth / 2;
    int screenCenterY = screenHeight / 2;
    int welcomeSize = 900;

    GameWindow gameWindow(welcomeSize, screenCenterX, screenCenterY);
    sf::RenderWindow &window = gameWindow.getWindow();

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf")) {
            return -1;
        }
    }

    GameState currentState = GameState::WELCOME;
    GameStats stats;
    bool shopOverlayOpen = false;
    bool isTransitioningToPlay = false;
    
    float playerStartX = 150 - 12.5f;
    float playerStartY = 150 - 12.5f;
    Player player(12.5f, 3.0f, playerStartX, playerStartY);
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    const float bulletSpeed = 6.0f;

    std::vector<Upgrade> upgrades;
    upgrades.push_back(Upgrade("splash damage", "Area damage on hit", 42));
    upgrades.push_back(Upgrade("speed", "Move faster", 10));
    upgrades.push_back(Upgrade("+1 multi shot", "Shoot multiple bullets", 50));
    
    sf::Clock shootClock;
    sf::Clock gameTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock deltaTimeClock;
    const sf::Time shootCooldown = sf::milliseconds(100);
    bool canShoot = true;

    sf::View gameView;

    while (window.isOpen())
    {
        float deltaTime = deltaTimeClock.restart().asSeconds();
        sf::Vector2f currentWindowSize = gameWindow.getSize();
        gameView.setSize(currentWindowSize);
        gameView.setCenter(sf::Vector2f(currentWindowSize.x / 2.0f, currentWindowSize.y / 2.0f));
        window.setView(gameView);
        
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);

        if (isTransitioningToPlay) {
            gameWindow.updateAnimation(deltaTime);
            if (gameWindow.isAnimationComplete()) {
                isTransitioningToPlay = false;
            }
        }

        while (auto eventOpt = window.pollEvent())
        {
            const sf::Event &event = *eventOpt;

            if (event.is<sf::Event::Closed>()) window.close();

            if (event.is<sf::Event::KeyPressed>())
            {
                const auto& keyEvent = event.getIf<sf::Event::KeyPressed>();
                
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (currentState == GameState::WELCOME) {
                        // Go directly to playing with collapse animation
                        isTransitioningToPlay = true;
                        gameWindow.startCollapseAnimation();
                        currentState = GameState::PLAYING;
                        shopOverlayOpen = false;
                        gameTimeClock.restart();
                        enemySpawnClock.restart();
                        
                        playerStartX = 150 - 12.5f;
                        playerStartY = 150 - 12.5f;
                        player = Player(12.5f, 3.0f + player.speedLevel * 0.5f, playerStartX, playerStartY);
                        bullets.clear();
                        enemies.clear();
                    }
                    else if (currentState == GameState::SHOP && !isTransitioningToPlay) {
                        isTransitioningToPlay = true;
                        gameWindow.startCollapseAnimation();
                        currentState = GameState::PLAYING;
                        shopOverlayOpen = false;
                        gameTimeClock.restart();
                        enemySpawnClock.restart();
                        
                        playerStartX = 150 - 12.5f;
                        playerStartY = 150 - 12.5f;
                        player = Player(12.5f, 3.0f + player.speedLevel * 0.5f, playerStartX, playerStartY);
                        player.currency = player.currency; // Keep currency
                        bullets.clear();
                        enemies.clear();
                    }
                }
                
                if (keyEvent->code == sf::Keyboard::Key::Tab)
                {
                    if (currentState == GameState::PLAYING && !isTransitioningToPlay) {
                        shopOverlayOpen = !shopOverlayOpen;
                    }
                }
                
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (currentState == GameState::PLAYING && !shopOverlayOpen) {
                        gameWindow.resizeWindow(900);
                        currentState = GameState::GAMEOVER;
                        stats.timeSurvived = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());
                    }
                    else if (shopOverlayOpen) {
                        shopOverlayOpen = false;
                    }
                }
            }

            if (event.is<sf::Event::MouseButtonPressed>())
            {
                const auto &mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
                
                if (mouseEvent->button == sf::Mouse::Button::Left && 
                    canShoot && currentState == GameState::PLAYING && !shopOverlayOpen)
                {
                    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePixelPos, gameView);
                    bullets.push_back(player.createBullet(mousePosF, bulletSpeed));
                    stats.bulletsFired++;
                    shootClock.restart();
                    canShoot = false;
                }
                
                if (mouseEvent->button == sf::Mouse::Button::Left && 
                    (currentState == GameState::SHOP || shopOverlayOpen))
                {
                    sf::Vector2u size = window.getSize();
                    float cardWidth = 200;
                    float cardHeight = 220;
                    float startX = (size.x - (cardWidth * 3 + 40)) / 2.0f;
                    float startY = 180;
                    
                    for (size_t i = 0; i < upgrades.size() && i < 3; i++) {
                        float x = startX + i * (cardWidth + 20);
                        float y = startY;
                        
                        if (mousePixelPos.x >= x && mousePixelPos.x <= x + cardWidth &&
                            mousePixelPos.y >= y && mousePixelPos.y <= y + cardHeight)
                        {
                            if (player.currency >= upgrades[i].cost && !upgrades[i].locked) {
                                player.currency -= upgrades[i].cost;
                                player.applyUpgrade(i);
                                upgrades[i].level++;
                                upgrades[i].cost += 5;
                            }
                        }
                    }
                }
                
                if (mouseEvent->button == sf::Mouse::Button::Right && 
                    (currentState == GameState::SHOP || shopOverlayOpen))
                {
                    sf::Vector2u size = window.getSize();
                    float cardWidth = 200;
                    float cardHeight = 220;
                    float startX = (size.x - (cardWidth * 3 + 40)) / 2.0f;
                    float startY = 180;
                    
                    for (size_t i = 0; i < upgrades.size() && i < 3; i++) {
                        float x = startX + i * (cardWidth + 20);
                        float y = startY;
                        
                        if (mousePixelPos.x >= x && mousePixelPos.x <= x + cardWidth &&
                            mousePixelPos.y >= y && mousePixelPos.y <= y + cardHeight)
                        {
                            upgrades[i].locked = !upgrades[i].locked;
                        }
                    }
                }
            }
        }

        if (!canShoot && shootClock.getElapsedTime() >= shootCooldown) {
            canShoot = true;
        }

        if (currentState == GameState::PLAYING && !isTransitioningToPlay)
        {
            stats.timeSurvived = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());
            
            // Update enemies (always, even when shop is open)
            for (auto it = enemies.begin(); it != enemies.end();) {
                if (!shopOverlayOpen) {
                    it->update();
                }
                
                sf::Vector2f enemyPos = it->getPosition();
                sf::Vector2f playerPos = player.getCenter();
                float dist = std::sqrt(std::pow(enemyPos.x - playerPos.x, 2) + 
                                     std::pow(enemyPos.y - playerPos.y, 2));
                
                if (dist < player.getRadius() + it->getRadius()) {
                    player.takeDamage(10);
                    it = enemies.erase(it);
                    continue;
                }
                
                if (gameWindow.isOutOfBounds(enemyPos)) {
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            }
            
            // Spawn enemies only when shop is closed
            if (!shopOverlayOpen && enemySpawnClock.getElapsedTime().asSeconds() > 2.0f) {
                sf::Vector2f spawnPos;
                int edge = rand() % 4;
                
                // Spawn inside the window for testing
                if (edge == 0) {
                    spawnPos = sf::Vector2f(currentWindowSize.x / 4, currentWindowSize.y / 4);
                } else if (edge == 1) {
                    spawnPos = sf::Vector2f(currentWindowSize.x * 3 / 4, currentWindowSize.y / 4);
                } else if (edge == 2) {
                    spawnPos = sf::Vector2f(currentWindowSize.x / 4, currentWindowSize.y * 3 / 4);
                } else {
                    spawnPos = sf::Vector2f(currentWindowSize.x * 3 / 4, currentWindowSize.y * 3 / 4);
                }
                
                sf::Vector2f playerPos = player.getCenter();
                sf::Vector2f direction = playerPos - spawnPos;
                float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (length > 0) {
                    direction.x = (direction.x / length) * 2.0f;
                    direction.y = (direction.y / length) * 2.0f;
                }
                
                enemies.push_back(Enemy(spawnPos, direction, 3, 5));
                enemySpawnClock.restart();
            }
            
            // Window shrinking
            if (!shopOverlayOpen) {
                gameWindow.shrink(deltaTime);
            }
            
            // Player movement
            sf::Vector2i centerShift = gameWindow.getScreenCenterShift();
            player.compensateForWindowShift(centerShift);
            
            if (!shopOverlayOpen) {
                player.handleInput();
            }
            player.updatePosition(gameWindow);
            
            sf::Vector2f wallChanges = gameWindow.getWallChanges();
            for (auto &bullet : bullets) {
                bullet.adjustPosition(wallChanges);
            }
            
            for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
                bulletIt->update();
                
                sf::Vector2f bulletPos = bulletIt->getPosition();
                float bulletRadius = bulletIt->getRadius();
                
                bool hitEnemy = false;
                for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
                    sf::Vector2f enemyPos = enemyIt->getPosition();
                    float dist = std::sqrt(std::pow(bulletPos.x - enemyPos.x, 2) + 
                                         std::pow(bulletPos.y - enemyPos.y, 2));
                    
                    if (dist < bulletRadius + enemyIt->getRadius()) {
                        enemyIt->takeDamage(1);
                        if (enemyIt->isDead()) {
                            player.earnCurrency(enemyIt->getCurrencyDrop());
                            stats.coinsCollected += enemyIt->getCurrencyDrop();
                            stats.enemiesKilled++;
                            enemyIt = enemies.erase(enemyIt);
                        } else {
                            ++enemyIt;
                        }
                        hitEnemy = true;
                        break;
                    } else {
                        ++enemyIt;
                    }
                }
                
                if (hitEnemy) {
                    bulletIt = bullets.erase(bulletIt);
                    continue;
                }
                
                bool hitEdge = false;
                if (bulletPos.x - bulletRadius <= 0 ||
                    bulletPos.x + bulletRadius >= currentWindowSize.x ||
                    bulletPos.y - bulletRadius <= 0 ||
                    bulletPos.y + bulletRadius >= currentWindowSize.y)
                {
                    gameWindow.handleEdgeHit(bulletPos, bulletRadius);
                    hitEdge = true;
                }
                
                if (gameWindow.isOutOfBounds(bulletPos) || hitEdge) {
                    bulletIt = bullets.erase(bulletIt);
                } else {
                    ++bulletIt;
                }
            }
            
            if (player.isDead()) {
                gameWindow.resizeWindow(900);
                currentState = GameState::GAMEOVER;
            }
        }

        window.clear(sf::Color::Black);

        if (currentState == GameState::WELCOME) {
            UIRenderer::drawWelcomeScreen(window, font);
        }
        else if (currentState == GameState::SHOP && !isTransitioningToPlay) {
            UIRenderer::drawShopScreen(window, font, player, upgrades, mousePixelPos, false);
        }
        else if (currentState == GameState::PLAYING) {
            if (!isTransitioningToPlay) {
                player.draw(window);
                for (auto &bullet : bullets) {
                    bullet.draw(window);
                }
                for (auto &enemy : enemies) {
                    enemy.draw(window);
                }
                
                sf::Text hudText(font, "Health: " + std::to_string(player.health) + 
                               " | Currency: " + std::to_string(player.currency) +
                               " | Time: " + std::to_string(stats.timeSurvived) + "s" +
                               " | Enemies: " + std::to_string(enemies.size()), 16);
                hudText.setFillColor(sf::Color::White);
                hudText.setPosition(sf::Vector2f(10, 10));
                window.draw(hudText);
                
                sf::Text tabHint(font, "[TAB] Shop", 14);
                tabHint.setFillColor(sf::Color(150, 150, 150));
                tabHint.setPosition(sf::Vector2f(10, currentWindowSize.y - 30));
                window.draw(tabHint);
            }
            
            if (shopOverlayOpen) {
                UIRenderer::drawShopScreen(window, font, player, upgrades, mousePixelPos, true);
            }
        }
        else if (currentState == GameState::GAMEOVER) {
            UIRenderer::drawGameOverScreen(window, font, stats);
        }

        window.display();
    }

    return 0;
}
