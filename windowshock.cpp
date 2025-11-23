#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

// ============================================================================
// GAME STATES
// ============================================================================
enum class GameState
{
    WELCOME,
    SHOP,
    PLAYING,
    GAMEOVER
};

// ============================================================================
// UPGRADE SYSTEM
// ============================================================================
struct Upgrade
{
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
struct GameStats
{
    int coinsCollected = 0;
    int starsCollected = 0;
    int timeSurvived = 0;
    int bulletsFired = 0;
    int enemiesKilled = 0;
    int bossesKilled = 0;
};

// ============================================================================
// FAKE WINDOW CLASS (ABSOLUTE COORDINATES)
// ============================================================================
class FakeWindow
{
private:
    // Absolute screen coordinates for the game arena boundaries
    float currentLeft, currentRight, currentTop, currentBottom;
    float targetLeft, targetRight, targetTop, targetBottom;

    // INCREASED: Minimum size to prevent unfair crushing
    const float minWidth = 250.0f;
    const float minHeight = 250.0f;

    float screenW, screenH;

    const float expandAmount = 20.0f;
    // REDUCED: Slower shrink rate for better pacing
    const float collapseSpeed = 0.1f;
    const float expandSpeed = 15.0f;

    const float titleBarHeight = 30.0f;
    const float borderThickness = 3.0f;

    bool isAnimating;
    float animationProgress;
    float animationDuration;

    sf::FloatRect animStartRect;
    sf::FloatRect animTargetRect;

public:
    FakeWindow(int sw, int sh, float initialSize)
        : screenW(static_cast<float>(sw)), screenH(static_cast<float>(sh)),
          isAnimating(false)
    {
        float centerX = screenW / 2.0f;
        float centerY = screenH / 2.0f;
        float halfSize = initialSize / 2.0f;

        currentLeft = targetLeft = centerX - halfSize;
        currentRight = targetRight = centerX + halfSize;
        currentTop = targetTop = centerY - halfSize;
        currentBottom = targetBottom = centerY + halfSize;
    }

    void resize(float newSize)
    {
        float centerX = screenW / 2.0f;
        float centerY = screenH / 2.0f;
        float halfSize = newSize / 2.0f;

        currentLeft = targetLeft = centerX - halfSize;
        currentRight = targetRight = centerX + halfSize;
        currentTop = targetTop = centerY - halfSize;
        currentBottom = targetBottom = centerY + halfSize;
    }

    void startCollapseAnimation()
    {
        isAnimating = true;
        animationProgress = 0.0f;
        animationDuration = 2.0f;

        animStartRect = sf::FloatRect({currentLeft, currentTop}, {currentRight - currentLeft, currentBottom - currentTop});

        float centerX = (currentLeft + currentRight) / 2.0f;
        float centerY = (currentTop + currentBottom) / 2.0f;
        float smallSize = 300.0f;

        animTargetRect = sf::FloatRect({centerX - smallSize / 2, centerY - smallSize / 2}, {smallSize, smallSize});

        targetLeft = animTargetRect.position.x;
        targetTop = animTargetRect.position.y;
        targetRight = animTargetRect.position.x + animTargetRect.size.x;
        targetBottom = animTargetRect.position.y + animTargetRect.size.y;
    }

    void update(float dt)
    {
        if (isAnimating)
        {
            animationProgress += dt / animationDuration;
            if (animationProgress >= 1.0f)
            {
                isAnimating = false;
                animationProgress = 1.0f;
                currentLeft = targetLeft;
                currentRight = targetRight;
                currentTop = targetTop;
                currentBottom = targetBottom;
            }
            else
            {
                float t = 1.0f - std::pow(1.0f - animationProgress, 3.0f);

                currentLeft = animStartRect.position.x + (animTargetRect.position.x - animStartRect.position.x) * t;

                float startRight = animStartRect.position.x + animStartRect.size.x;
                float targetRightVal = animTargetRect.position.x + animTargetRect.size.x;
                currentRight = startRight + (targetRightVal - startRight) * t;

                currentTop = animStartRect.position.y + (animTargetRect.position.y - animStartRect.position.y) * t;

                float startBottom = animStartRect.position.y + animStartRect.size.y;
                float targetBottomVal = animTargetRect.position.y + animTargetRect.size.y;
                currentBottom = startBottom + (targetBottomVal - startBottom) * t;
            }
            return;
        }

        // Shrink Targets
        if (targetRight - targetLeft > minWidth)
        {
            targetLeft += collapseSpeed;
            targetRight -= collapseSpeed;
        }
        if (targetBottom - targetTop > minHeight)
        {
            targetTop += collapseSpeed;
            targetBottom -= collapseSpeed;
        }

        // Move Current Walls
        auto moveTowards = [&](float &current, float target)
        {
            if (std::abs(current - target) < expandSpeed)
            {
                current = target;
            }
            else if (current < target)
            {
                current += expandSpeed;
            }
            else
            {
                current -= expandSpeed;
            }
        };

        moveTowards(currentLeft, targetLeft);
        moveTowards(currentRight, targetRight);
        moveTowards(currentTop, targetTop);
        moveTowards(currentBottom, targetBottom);
    }

    void hitWall(int wallIndex)
    {
        if (isAnimating)
            return;

        float limitBuffer = 50.0f;

        switch (wallIndex)
        {
        case 0: // Left
            if (targetLeft > limitBuffer)
                targetLeft -= expandAmount;
            break;
        case 1: // Right
            if (targetRight < screenW - limitBuffer)
                targetRight += expandAmount;
            break;
        case 2: // Top
            if (targetTop > limitBuffer)
                targetTop -= expandAmount;
            break;
        case 3: // Bottom
            if (targetBottom < screenH - limitBuffer)
                targetBottom += expandAmount;
            break;
        }
    }

    // NEW: Creates a View that clips rendering to exactly the window bounds
    sf::View getClippingView() const
    {
        sf::View view;
        float w = getWidth();
        float h = getHeight();

        // The view looks at the world coordinates matching the window
        view.setCenter({currentLeft + w / 2.0f, currentTop + h / 2.0f});
        view.setSize({w, h});

        // The viewport (0.0-1.0) defines where on the screen this view draws
        float vpX = currentLeft / screenW;
        float vpY = currentTop / screenH;
        float vpW = w / screenW;
        float vpH = h / screenH;

        view.setViewport(sf::FloatRect({vpX, vpY}, {vpW, vpH}));
        return view;
    }

    float getLeft() const { return currentLeft; }
    float getRight() const { return currentRight; }
    float getTop() const { return currentTop; }
    float getBottom() const { return currentBottom; }

    float getWidth() const { return currentRight - currentLeft; }
    float getHeight() const { return currentBottom - currentTop; }

    sf::Vector2f getPosition() const { return sf::Vector2f(currentLeft, currentTop); }
    sf::Vector2f getSize() const { return sf::Vector2f(getWidth(), getHeight()); }

    bool isAnimationComplete() const { return !isAnimating; }

    void draw(sf::RenderWindow &window, const sf::Font &font)
    {
        float w = getWidth();
        float h = getHeight();
        float x = currentLeft;
        float y = currentTop - titleBarHeight;

        // Title bar
        sf::RectangleShape titleBar(sf::Vector2f(w, titleBarHeight));
        titleBar.setPosition(sf::Vector2f(x, y));
        titleBar.setFillColor(sf::Color(45, 45, 48));
        window.draw(titleBar);

        // Title text
        sf::Text titleText(font, "WindowShock Game", 14);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(sf::Vector2f(x + 10, y + 7));
        window.draw(titleText);

        // Buttons
        float buttonSize = 12.0f;
        float buttonY = y + titleBarHeight / 2.0f - buttonSize / 2.0f;

        auto drawBtn = [&](float offsetX, sf::Color col)
        {
            sf::CircleShape btn(buttonSize / 2.0f);
            btn.setPosition(sf::Vector2f(x + w - offsetX - buttonSize, buttonY));
            btn.setFillColor(col);
            window.draw(btn);
        };

        drawBtn(15, sf::Color(255, 95, 86));
        drawBtn(35, sf::Color(40, 201, 64));
        drawBtn(55, sf::Color(255, 189, 46));

        // Background (The Void)
        sf::RectangleShape gameArea(sf::Vector2f(w, h));
        gameArea.setPosition(sf::Vector2f(currentLeft, currentTop));
        gameArea.setFillColor(sf::Color::Black);
        window.draw(gameArea);

        // Border
        sf::RectangleShape border(sf::Vector2f(w, h + titleBarHeight));
        border.setPosition(sf::Vector2f(x, y));
        border.setFillColor(sf::Color::Transparent);
        border.setOutlineColor(sf::Color(100, 100, 100));
        border.setOutlineThickness(borderThickness);
        window.draw(border);
    }
};

// ============================================================================
// ENEMY CLASS
// ============================================================================
class Enemy
{
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    int health;
    int currencyDrop;

public:
    Enemy(sf::Vector2f position, sf::Vector2f vel, int hp = 3, int currency = 5)
        : velocity(vel), health(hp), currencyDrop(currency)
    {
        shape.setRadius(12.0f);
        shape.setFillColor(sf::Color(0, 255, 0));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2);
        shape.setOrigin(sf::Vector2f(12.0f, 12.0f));
        shape.setPosition(position);
    }

    void update()
    {
        shape.move(velocity);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

    sf::Vector2f getPosition() const { return shape.getPosition(); }
    float getRadius() const { return shape.getRadius(); }
    void takeDamage(int damage) { health -= damage; }
    bool isDead() const { return health <= 0; }
    int getCurrencyDrop() const { return currencyDrop; }
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
        shape.setOrigin(sf::Vector2f(3.0f, 3.0f));
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(position);
        velocity = vel;
    }

    void update()
    {
        shape.move(velocity);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(shape);
    }

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

public:
    int currency = 83;
    int health = 100;
    int maxHealth = 100;

    int splashDamageLevel = 0;
    int speedLevel = 0;
    int multiShotLevel = 0;

    Player(float radius = 12.5f, float speed = 5.0f, float startX = 0.0f, float startY = 0.0f)
        : moveSpeed(speed)
    {
        circle.setRadius(radius);
        circle.setOrigin(sf::Vector2f(radius, radius));
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(sf::Vector2f(startX, startY));
    }

    void handleInput()
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

    void constrainToWindow(const FakeWindow &fw)
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

    sf::Vector2f getPosition() const { return circle.getPosition(); }
    float getRadius() const { return circle.getRadius(); }

    void draw(sf::RenderWindow &window)
    {
        window.draw(circle);
    }

    Bullet createBullet(sf::Vector2f targetPos, float bulletSpeed)
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

    void earnCurrency(int amount) { currency += amount; }
    void takeDamage(int damage) { health = std::max(0, health - damage); }
    bool isDead() const { return health <= 0; }

    void applyUpgrade(int upgradeIndex)
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
};

// ============================================================================
// UI RENDERING
// ============================================================================
class UIRenderer
{
public:
    static void drawWelcomeScreen(sf::RenderWindow &window, const sf::Font &font, const FakeWindow &fakeWin)
    {
        sf::Vector2f pos = fakeWin.getPosition();
        sf::Vector2f size = fakeWin.getSize();

        float centerX = pos.x + size.x / 2.0f;
        float centerY = pos.y + size.y / 2.0f;

        sf::Text title(font, "WINDOWSHOCK", 80);
        title.setFillColor(sf::Color(138, 43, 226));
        title.setStyle(sf::Text::Bold);
        centerText(title, centerX, centerY - 100);
        window.draw(title);

        sf::Text instructions(font, "Press SPACE to Start", 30);
        instructions.setFillColor(sf::Color::White);
        centerText(instructions, centerX, centerY);
        window.draw(instructions);

        sf::Text info(font, "WASD - Move | Mouse - Aim | Click - Shoot | TAB - Shop", 20);
        info.setFillColor(sf::Color(150, 150, 150));
        centerText(info, centerX, centerY + 60);
        window.draw(info);
    }

    static void drawShopScreen(sf::RenderWindow &window, const sf::Font &font, Player &player,
                               std::vector<Upgrade> &upgrades, sf::Vector2i mousePos,
                               const FakeWindow &fakeWin, bool isOverlay = false)
    {
        sf::Vector2f pos = fakeWin.getPosition();
        sf::Vector2f size = fakeWin.getSize();
        float centerX = pos.x + size.x / 2.0f;

        // Dim background only inside the fake window
        sf::RectangleShape bg(size);
        bg.setPosition(pos);
        bg.setFillColor(sf::Color(0, 0, 0, 230));
        window.draw(bg);

        sf::Text currency(font, std::to_string(player.currency), 50);
        currency.setFillColor(sf::Color(138, 43, 226));
        currency.setStyle(sf::Text::Bold);
        currency.setPosition(sf::Vector2f(centerX - 40, pos.y + 40));
        window.draw(currency);

        sf::Text shopTitle(font, isOverlay ? "SHOP (TAB to close)" : "SHOP (SPACE to play)", 30);
        shopTitle.setFillColor(sf::Color(150, 150, 150));
        centerText(shopTitle, centerX, pos.y + 120);
        window.draw(shopTitle);

        float cardWidth = 200;
        float cardHeight = 220;
        float totalCardsWidth = (cardWidth * 3) + (20 * 2);
        float startX = centerX - totalCardsWidth / 2.0f;
        float startY = pos.y + 180;

        for (size_t i = 0; i < upgrades.size() && i < 3; i++)
        {
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

            sf::Text nameText(font, upgrades[i].name, 22);
            nameText.setFillColor(sf::Color::White);
            nameText.setStyle(sf::Text::Bold);
            centerText(nameText, x + cardWidth / 2, y + 150);
            window.draw(nameText);

            sf::Text costText(font, std::to_string(upgrades[i].cost), 30);
            costText.setFillColor(sf::Color(138, 43, 226));
            costText.setStyle(sf::Text::Bold);
            centerText(costText, x + cardWidth / 2, y + 190);
            window.draw(costText);
        }
    }

    static void drawGameOverScreen(sf::RenderWindow &window, const sf::Font &font,
                                   const GameStats &stats, const FakeWindow &fakeWin)
    {
        sf::Vector2f pos = fakeWin.getPosition();
        sf::Vector2f size = fakeWin.getSize();
        float centerX = pos.x + size.x / 2.0f;

        sf::RectangleShape background(size);
        background.setPosition(pos);
        background.setFillColor(sf::Color(0, 0, 0, 230));
        window.draw(background);

        sf::Text title(font, "GAME OVER", 80);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        centerText(title, centerX, pos.y + 120);
        window.draw(title);

        float startY = pos.y + 250;
        float spacing = 40;

        auto draw = [&](std::string label, int val, int idx)
        {
            sf::Text t(font, label + std::to_string(val), 24);
            t.setFillColor(sf::Color::White);
            centerText(t, centerX, startY + idx * spacing);
            window.draw(t);
        };

        draw("Time Survived: ", stats.timeSurvived, 0);
        draw("Enemies Killed: ", stats.enemiesKilled, 1);
        draw("Coins Collected: ", stats.coinsCollected, 2);

        sf::Text instructions(font, "Close Window to Exit", 25);
        instructions.setFillColor(sf::Color(150, 150, 150));
        centerText(instructions, centerX, pos.y + size.y - 60);
        window.draw(instructions);
    }

private:
    static void centerText(sf::Text &text, float x, float y)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
        text.setPosition(sf::Vector2f(x, y));
    }
};

// ============================================================================
// MAIN
// ============================================================================
int main()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    sf::VideoMode desktop({static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)});
    sf::RenderWindow window(desktop, "WindowShock", sf::Style::None);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    HWND hwnd = window.getNativeHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style | WS_VISIBLE);
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf"))
            return -1;

    FakeWindow fakeWindow(screenWidth, screenHeight, 900.0f);
    Player player(12.5f, 5.0f, screenWidth / 2.0f, screenHeight / 2.0f);

    GameState currentState = GameState::WELCOME;
    GameStats stats;
    bool shopOverlayOpen = false;
    bool isTransitioningToPlay = false;

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    const float bulletSpeed = 12.0f;

    std::vector<Upgrade> upgrades;
    upgrades.push_back(Upgrade("splash damage", "Area damage", 42));
    upgrades.push_back(Upgrade("speed", "Move faster", 10));
    upgrades.push_back(Upgrade("multi shot", "More bullets", 50));

    sf::Clock shootClock;
    sf::Clock gameTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock deltaTimeClock;
    const sf::Time shootCooldown = sf::milliseconds(100);
    bool canShoot = true;

    // Store default view for UI/Background
    sf::View defaultView = window.getDefaultView();

    while (window.isOpen())
    {
        float deltaTime = deltaTimeClock.restart().asSeconds();

        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = static_cast<sf::Vector2f>(mousePixelPos);

        if (isTransitioningToPlay)
        {
            fakeWindow.update(deltaTime);
            if (fakeWindow.isAnimationComplete())
                isTransitioningToPlay = false;
        }

        while (auto eventOpt = window.pollEvent())
        {
            const sf::Event &event = *eventOpt;
            if (event.is<sf::Event::Closed>())
                window.close();

            if (event.is<sf::Event::KeyPressed>())
            {
                const auto &keyEvent = event.getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (currentState == GameState::WELCOME || (currentState == GameState::SHOP && !isTransitioningToPlay))
                    {
                        isTransitioningToPlay = true;
                        fakeWindow.startCollapseAnimation();
                        currentState = GameState::PLAYING;
                        shopOverlayOpen = false;
                        gameTimeClock.restart();
                        enemySpawnClock.restart();

                        player = Player(12.5f, 5.0f + player.speedLevel * 0.5f, screenWidth / 2.0f, screenHeight / 2.0f);
                        bullets.clear();
                        enemies.clear();
                    }
                }
                if (keyEvent->code == sf::Keyboard::Key::Tab)
                {
                    if (currentState == GameState::PLAYING && !isTransitioningToPlay)
                        shopOverlayOpen = !shopOverlayOpen;
                }
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (shopOverlayOpen)
                        shopOverlayOpen = false;
                    else if (currentState == GameState::PLAYING)
                    {
                        fakeWindow.resize(900.0f);
                        currentState = GameState::GAMEOVER;
                    }
                }
            }

            if (event.is<sf::Event::MouseButtonPressed>())
            {
                const auto &mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

                if (mouseEvent->button == sf::Mouse::Button::Left && canShoot && currentState == GameState::PLAYING && !shopOverlayOpen)
                {
                    bullets.push_back(player.createBullet(mousePosF, bulletSpeed));
                    stats.bulletsFired++;
                    shootClock.restart();
                    canShoot = false;
                }

                if (mouseEvent->button == sf::Mouse::Button::Left && (currentState == GameState::SHOP || shopOverlayOpen))
                {
                    sf::Vector2f pos = fakeWindow.getPosition();
                    sf::Vector2f size = fakeWindow.getSize();
                    float centerX = pos.x + size.x / 2.0f;
                    float cardWidth = 200;
                    float totalCardsWidth = (cardWidth * 3) + 40;
                    float startX = centerX - totalCardsWidth / 2.0f;
                    float startY = pos.y + 180;

                    for (size_t i = 0; i < upgrades.size() && i < 3; i++)
                    {
                        float x = startX + i * (cardWidth + 20);
                        if (mousePosF.x >= x && mousePosF.x <= x + cardWidth && mousePosF.y >= startY && mousePosF.y <= startY + 220)
                        {
                            if (player.currency >= upgrades[i].cost)
                            {
                                player.currency -= upgrades[i].cost;
                                player.applyUpgrade(i);
                                upgrades[i].level++;
                                upgrades[i].cost += 5;
                            }
                        }
                    }
                }
            }
        }

        if (!canShoot && shootClock.getElapsedTime() >= shootCooldown)
            canShoot = true;

        if (currentState == GameState::PLAYING && !isTransitioningToPlay && !shopOverlayOpen)
        {
            stats.timeSurvived = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());
            fakeWindow.update(deltaTime);
            player.handleInput();
            player.constrainToWindow(fakeWindow);

            for (auto it = bullets.begin(); it != bullets.end();)
            {
                it->update();
                sf::Vector2f bPos = it->getPosition();
                bool hit = false;

                if (bPos.x < fakeWindow.getLeft())
                {
                    fakeWindow.hitWall(0);
                    hit = true;
                }
                else if (bPos.x > fakeWindow.getRight())
                {
                    fakeWindow.hitWall(1);
                    hit = true;
                }
                else if (bPos.y < fakeWindow.getTop())
                {
                    fakeWindow.hitWall(2);
                    hit = true;
                }
                else if (bPos.y > fakeWindow.getBottom())
                {
                    fakeWindow.hitWall(3);
                    hit = true;
                }

                if (hit)
                {
                    it = bullets.erase(it);
                    continue;
                }

                bool enemyHit = false;
                for (auto eIt = enemies.begin(); eIt != enemies.end();)
                {
                    sf::Vector2f diff = bPos - eIt->getPosition();
                    float distSq = diff.x * diff.x + diff.y * diff.y;
                    float r = eIt->getRadius() + it->getRadius();
                    if (distSq < r * r)
                    {
                        eIt->takeDamage(1);
                        if (eIt->isDead())
                        {
                            player.earnCurrency(eIt->getCurrencyDrop());
                            stats.enemiesKilled++;
                            eIt = enemies.erase(eIt);
                        }
                        else
                        {
                            ++eIt;
                        }
                        enemyHit = true;
                        break;
                    }
                    else
                        ++eIt;
                }

                if (enemyHit)
                    it = bullets.erase(it);
                else
                    ++it;
            }

            if (enemySpawnClock.getElapsedTime().asSeconds() > 1.0f)
            {
                int side = rand() % 4;
                sf::Vector2f spawnPos;
                if (side == 0)
                    spawnPos = {static_cast<float>(rand() % screenWidth), -20.0f};
                else if (side == 1)
                    spawnPos = {static_cast<float>(rand() % screenWidth), static_cast<float>(screenHeight) + 20.0f};
                else if (side == 2)
                    spawnPos = {-20.0f, static_cast<float>(rand() % screenHeight)};
                else
                    spawnPos = {static_cast<float>(screenWidth) + 20.0f, static_cast<float>(rand() % screenHeight)};

                sf::Vector2f dir = player.getPosition() - spawnPos;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len > 0)
                    dir = (dir / len) * 2.5f;

                enemies.push_back(Enemy(spawnPos, dir));
                enemySpawnClock.restart();
            }

            for (auto it = enemies.begin(); it != enemies.end();)
            {
                it->update();
                sf::Vector2f diff = player.getPosition() - it->getPosition();
                float distSq = diff.x * diff.x + diff.y * diff.y;
                float r = it->getRadius() + player.getRadius();

                if (distSq < r * r)
                {
                    player.takeDamage(10);
                    it = enemies.erase(it);
                    continue;
                }

                sf::Vector2f p = it->getPosition();
                if (p.x < -100 || p.x > screenWidth + 100 || p.y < -100 || p.y > screenHeight + 100)
                    it = enemies.erase(it);
                else
                    ++it;
            }

            if (player.isDead())
            {
                fakeWindow.resize(900.0f);
                currentState = GameState::GAMEOVER;
            }
        }

        window.clear(sf::Color(255, 0, 255)); // Transparent Key Color

        // 1. Draw Window Frame (Using Default View)
        window.setView(defaultView);
        fakeWindow.draw(window, font);

        // 2. Draw Gameplay Entities (Using Clipping View)
        // This hides anything (enemies/bullets) outside the fake window
        if (currentState == GameState::PLAYING && !isTransitioningToPlay)
        {
            window.setView(fakeWindow.getClippingView());

            for (auto &e : enemies)
                e.draw(window);
            for (auto &b : bullets)
                b.draw(window);
            player.draw(window);

            // Restore default view for UI
            window.setView(defaultView);

            // Draw HUD (Unclipped)
            sf::Vector2f winPos = fakeWindow.getPosition();
            sf::Text hud(font, "HP: " + std::to_string(player.health) + " $ " + std::to_string(player.currency), 20);
            hud.setFillColor(sf::Color::White);
            hud.setPosition(sf::Vector2f(winPos.x + 10, winPos.y + 40));
            window.draw(hud);

            if (shopOverlayOpen)
                UIRenderer::drawShopScreen(window, font, player, upgrades, mousePixelPos, fakeWindow, true);
        }
        else if (currentState == GameState::WELCOME)
        {
            UIRenderer::drawWelcomeScreen(window, font, fakeWindow);
        }
        else if (currentState == GameState::SHOP)
        {
            UIRenderer::drawShopScreen(window, font, player, upgrades, mousePixelPos, fakeWindow, false);
        }
        else if (currentState == GameState::GAMEOVER)
        {
            UIRenderer::drawGameOverScreen(window, font, stats, fakeWindow);
        }

        window.display();
    }

    return 0;
}