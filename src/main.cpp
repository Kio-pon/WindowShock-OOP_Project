#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "../include/GameState.hpp"
#include "../include/Upgrade.hpp"
#include "../include/GameStats.hpp"
#include "../include/FakeWindow.hpp"
#include "../include/UpgradeWindow.hpp"
#include "../include/Bullet.hpp"
#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include "../include/UIRenderer.hpp"

int main()
{
    // Get screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create fullscreen transparent window
    sf::VideoMode desktop({static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)});
    sf::RenderWindow window(desktop, "WindowShock", sf::Style::None);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    // Make window transparent and always on top
    HWND hwnd = window.getNativeHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style | WS_VISIBLE);
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // Load font
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf"))
            return -1;

    // Initialize game objects
    FakeWindow fakeWindow(screenWidth, screenHeight, 900.0f);
    UpgradeWindow upgradeWindow(screenWidth, screenHeight);
    Player player(12.5f, 5.0f, screenWidth / 2.0f, screenHeight / 2.0f);

    GameState currentState = GameState::WELCOME;
    GameStats stats;
    bool isTransitioningToPlay = false;

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;
    const float bulletSpeed = 12.0f;

    // Create available upgrades
    std::vector<Upgrade> upgrades;
    upgrades.push_back(Upgrade("splash damage", "Area damage", 42));
    upgrades.push_back(Upgrade("speed", "Move faster", 10));
    upgrades.push_back(Upgrade("multi shot", "More bullets", 50));

    // Setup timing clocks
    sf::Clock shootClock;
    sf::Clock gameTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock deltaTimeClock;
    const sf::Time shootCooldown = sf::milliseconds(250);
    bool canShoot = true;

    // Store default view for UI rendering
    sf::View defaultView = window.getDefaultView();

    // Main game loop
    while (window.isOpen())
    {
        float deltaTime = deltaTimeClock.restart().asSeconds();

        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = static_cast<sf::Vector2f>(mousePixelPos);

        // Handle window collapse animation
        if (isTransitioningToPlay)
        {
            fakeWindow.update(deltaTime);
            if (fakeWindow.isAnimationComplete())
                isTransitioningToPlay = false;
        }

        // Process events
        while (auto eventOpt = window.pollEvent())
        {
            const sf::Event &event = *eventOpt;
            if (event.is<sf::Event::Closed>())
                window.close();

            if (event.is<sf::Event::KeyPressed>())
            {
                const auto &keyEvent = event.getIf<sf::Event::KeyPressed>();
                
                // Start game
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (currentState == GameState::WELCOME || (currentState == GameState::SHOP && !isTransitioningToPlay))
                    {
                        isTransitioningToPlay = true;
                        fakeWindow.startCollapseAnimation();
                        currentState = GameState::PLAYING;
                        upgradeWindow.hide();
                        gameTimeClock.restart();
                        enemySpawnClock.restart();

                        // Reset game state
                        player = Player(12.5f, 5.0f + player.speedLevel * 0.5f, screenWidth / 2.0f, screenHeight / 2.0f);
                        bullets.clear();
                        enemies.clear();
                    }
                }
                
                // Toggle upgrade shop window
                if (keyEvent->code == sf::Keyboard::Key::Tab)
                {
                    if (currentState == GameState::PLAYING && !isTransitioningToPlay)
                        upgradeWindow.toggle();
                }
                
                // Exit to game over
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (upgradeWindow.getVisible())
                        upgradeWindow.hide();
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

                // Purchase upgrade from upgrade window
                if (mouseEvent->button == sf::Mouse::Button::Left && upgradeWindow.getVisible())
                {
                    sf::Vector2f pos = upgradeWindow.getPosition();
                    sf::Vector2f size = upgradeWindow.getSize();
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
                // Purchase upgrade from shop screen
                else if (mouseEvent->button == sf::Mouse::Button::Left && currentState == GameState::SHOP)
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
        // Auto-shoot bullets (only when upgrade window is closed)
        if (canShoot && currentState == GameState::PLAYING && !upgradeWindow.getVisible())
        {
            bullets.push_back(player.createBullet(mousePosF, bulletSpeed));
            stats.bulletsFired++;
            shootClock.restart();
            canShoot = false;
        }
        // Reset shoot cooldown
        if (!canShoot && shootClock.getElapsedTime() >= shootCooldown)
            canShoot = true;

        // Update game logic (pause when upgrade window is open)
        if (currentState == GameState::PLAYING && !isTransitioningToPlay && !upgradeWindow.getVisible())
        {
            stats.timeSurvived = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());
            fakeWindow.update(deltaTime);
            player.handleInput();
            player.constrainToWindow(fakeWindow);

            // Update bullets
            for (auto it = bullets.begin(); it != bullets.end();)
            {
                it->update();
                sf::Vector2f bPos = it->getPosition();
                bool hit = false;

                // Check if bullet hit window edges
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

                // Check if bullet hit enemy
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

            // Spawn new enemies
            if (enemySpawnClock.getElapsedTime().asSeconds() > 1.5f)
            {
                int side = rand() % 4;
                sf::Vector2f spawnPos;
                
                // Spawn from random screen edge
                if (side == 0)
                    spawnPos = {static_cast<float>(rand() % screenWidth), -20.0f};
                else if (side == 1)
                    spawnPos = {static_cast<float>(rand() % screenWidth), static_cast<float>(screenHeight) + 20.0f};
                else if (side == 2)
                    spawnPos = {-20.0f, static_cast<float>(rand() % screenHeight)};
                else
                    spawnPos = {static_cast<float>(screenWidth) + 20.0f, static_cast<float>(rand() % screenHeight)};

                enemies.push_back(Enemy(spawnPos, 1.0f));
                enemySpawnClock.restart();
            }

            // Update enemies
            for (auto it = enemies.begin(); it != enemies.end();)
            {
                it->update(player.getPosition());
                sf::Vector2f diff = player.getPosition() - it->getPosition();
                float distSq = diff.x * diff.x + diff.y * diff.y;
                float r = it->getRadius() + player.getRadius();

                // Check if enemy hit player
                if (distSq < r * r)
                {
                    player.takeDamage(10);
                    it = enemies.erase(it);
                    continue;
                }

                // Remove enemies that went off screen
                sf::Vector2f p = it->getPosition();
                if (p.x < -100 || p.x > screenWidth + 100 || p.y < -100 || p.y > screenHeight + 100)
                    it = enemies.erase(it);
                else
                    ++it;
            }

            // Check if player died
            if (player.isDead())
            {
                fakeWindow.resize(900.0f);
                currentState = GameState::GAMEOVER;
            }
        }

        // Render everything
        window.clear(sf::Color(255, 0, 255)); // Transparent color key

        // Draw window frame
        window.setView(defaultView);
        fakeWindow.draw(window, font);

        // Draw game entities with clipping
        if (currentState == GameState::PLAYING && !isTransitioningToPlay)
        {
            window.setView(fakeWindow.getClippingView());

            for (auto &e : enemies)
                e.draw(window);
            for (auto &b : bullets)
                b.draw(window);
            player.draw(window);

            // Switch back to default view for UI
            window.setView(defaultView);

            // Draw HUD
            sf::Vector2f winPos = fakeWindow.getPosition();
            sf::Text hud(font, "HP: " + std::to_string(player.health) + " $ " + std::to_string(player.currency), 20);
            hud.setFillColor(sf::Color::White);
            hud.setPosition(sf::Vector2f(winPos.x + 10, winPos.y + 40));
            window.draw(hud);
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

        // Draw upgrade window on top of everything if visible
        if (upgradeWindow.getVisible())
        {
            upgradeWindow.draw(window, font);
            UIRenderer::drawUpgradeWindow(window, font, player, upgrades, mousePixelPos, upgradeWindow);
        }

        window.display();
    }

    return 0;
}
