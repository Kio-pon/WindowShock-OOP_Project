#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <memory>

#include "../include/GameState.hpp"
#include "../include/Upgrade.hpp"
#include "../include/GameStats.hpp"
#include "../include/FakeWindow.hpp"
#include "../include/PlayingWindow.hpp"
#include "../include/ShoppingWindow.hpp"
#include "../include/WelcomeWindow.hpp"
#include "../include/UpgradeWindow.hpp"
#include "../include/Bullet.hpp"
#include "../include/Enemy.hpp"
#include "../include/Player.hpp"
#include "../include/UIRenderer.hpp"
#include "../include/TankClass.hpp"

int main()
{
    // Retrieve screen resolution
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Initialize full-screen transparent window
    sf::VideoMode desktop({static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)});
    sf::RenderWindow window(desktop, "WindowShock", sf::Style::None);
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    // Configure window attributes for transparency and layering
    HWND hwnd = window.getNativeHandle();
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLong(hwnd, GWL_STYLE, style | WS_VISIBLE);
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED | WS_EX_TOPMOST);
    SetLayeredWindowAttributes(hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // Font loading fallback
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf"))
            return -1;

    // Game object initialization
    std::unique_ptr<FakeWindow> currentWindow = std::make_unique<WelcomeWindow>(screenWidth, screenHeight, 675.0f);
    
    UpgradeWindow upgradeWindow(screenWidth, screenHeight);
    Player player(15.0f, 5.0f, screenWidth / 2.0f, screenHeight / 2.0f);

    GameState currentState = GameState::WELCOME;
    GameStats stats;
    bool isTransitioningToPlay = false;

    std::vector<Bullet> bullets;
    std::vector<Bullet> enemyBullets;
    std::vector<std::shared_ptr<Enemy>> enemies;

    sf::Clock shootClock;
    sf::Clock gameTimeClock;
    sf::Clock enemySpawnClock;
    sf::Clock deltaTimeClock;
    
    sf::View defaultView = window.getDefaultView();

    while (window.isOpen())
    {
        float deltaTime = deltaTimeClock.restart().asSeconds();

        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = static_cast<sf::Vector2f>(mousePixelPos);
        sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePixelPos);

        // Animation handling
        if (isTransitioningToPlay)
        {
            currentWindow->update(deltaTime);
            if (currentWindow->isAnimationComplete())
                isTransitioningToPlay = false;
        }

        // Event processing
        while (auto eventOpt = window.pollEvent())
        {
            const sf::Event &event = *eventOpt;
            if (event.is<sf::Event::Closed>())
                window.close();

            if (event.is<sf::Event::KeyPressed>())
            {
                const auto &keyEvent = event.getIf<sf::Event::KeyPressed>();
                
                // Game start
                if (keyEvent->code == sf::Keyboard::Key::Space)
                {
                    if (currentState == GameState::WELCOME || currentState == GameState::GAMEOVER)
                    {
                        isTransitioningToPlay = true;
                        
                        float currentSize = currentWindow->getWidth();
                        currentWindow = std::make_unique<PlayingWindow>(screenWidth, screenHeight, currentSize);
                        
                        currentWindow->startCollapseAnimation();
                        currentState = GameState::PLAYING;
                        upgradeWindow.hide();
                        gameTimeClock.restart();
                        enemySpawnClock.restart();

                        // Reset state
                        player = Player(15.0f, 5.0f, screenWidth / 2.0f, screenHeight / 2.0f);
                        bullets.clear();
                        enemyBullets.clear();
                        enemies.clear();
                        stats = GameStats();
                    }
                }
                
                // Toggle upgrade window
                if (keyEvent->code == sf::Keyboard::Key::Tab)
                {
                    if (currentState == GameState::PLAYING && !isTransitioningToPlay)
                        upgradeWindow.toggle();
                }
                
                // Exit application
                if (keyEvent->code == sf::Keyboard::Key::Escape)
                {
                    if (upgradeWindow.getVisible())
                        upgradeWindow.hide();
                    else
                        window.close();
                }
            }

            if (event.is<sf::Event::MouseButtonPressed>())
            {
                const auto &mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();

                // Handle upgrade window interactions
                if (currentState == GameState::PLAYING && upgradeWindow.getVisible() && mouseEvent->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f winPos = upgradeWindow.getPosition();
                    sf::Vector2f winSize = upgradeWindow.getSize();
                    
                    if (upgradeWindow.getState() == UpgradeWindowState::Stats)
                    {
                        // Handle stat upgrades
                        float startY = winPos.y + 100;
                        float gap = 35.0f;
                        for (int i = 0; i < 8; i++)
                        {
                            sf::FloatRect btnRect(sf::Vector2f(winPos.x + 50 + 260.0f, startY + gap * i), sf::Vector2f(20.0f, 20.0f));
                            if (btnRect.contains(mousePosF))
                            {
                                player.upgradeStat(i);
                            }
                        }
                        
                        // Handle tank upgrades
                        bool canUpgrade = false;
                        auto upgrades = player.currentTank->getUpgrades();
                        if (!upgrades.empty())
                        {
                            int currentTier = player.currentTank->getTier();
                            if ((currentTier == 1 && player.level >= 10) || (currentTier == 2 && player.level >= 20))
                                canUpgrade = true;
                        }

                        if (canUpgrade)
                        {
                            sf::FloatRect upgBtn(sf::Vector2f(winPos.x + winSize.x - 250, winPos.y + 50), sf::Vector2f(200.0f, 40.0f));
                            if (upgBtn.contains(mousePosF))
                            {
                                upgradeWindow.setState(UpgradeWindowState::ClassSelection);
                            }
                        }
                    }
                    else if (upgradeWindow.getState() == UpgradeWindowState::ClassSelection)
                    {
                        // Handle back button
                        sf::FloatRect backBtn(sf::Vector2f(winPos.x + 20, winPos.y + 20), sf::Vector2f(100.0f, 30.0f));
                        if (backBtn.contains(mousePosF))
                        {
                            upgradeWindow.setState(UpgradeWindowState::Stats);
                        }
                        
                        // Handle class selection
                        auto upgrades = player.currentTank->getUpgrades();
                        float startX = winPos.x + 100;
                        float startY = winPos.y + 150;
                        float boxSize = 120;
                        float gap = 30;
                        
                        for (size_t i = 0; i < upgrades.size(); i++)
                        {
                            float x = startX + (i % 3) * (boxSize + gap);
                            float y = startY + (i / 3) * (boxSize + gap);
                            sf::FloatRect box(sf::Vector2f(x, y), sf::Vector2f(boxSize, boxSize));
                            
                            if (box.contains(mousePosF))
                            {
                                player.setTank(upgrades[i]);
                                upgradeWindow.toggle();
                            }
                        }
                    }
                }
            }
        }

        // Core game logic update
        if (currentState == GameState::PLAYING && !isTransitioningToPlay && !upgradeWindow.getVisible())
        {
            stats.timeSurvived = static_cast<int>(gameTimeClock.getElapsedTime().asSeconds());
            currentWindow->update(deltaTime);
            
            // Player orientation
            sf::Vector2f playerPos = player.getPosition();
            sf::Vector2f dir = mouseWorldPos - playerPos;
            float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159f;
            player.setRotation(angle);
            
            player.handleInput();
            player.update(deltaTime);
            player.constrainToWindow(*currentWindow);

            // Player shooting
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && player.reloadTimer <= 0.0f)
            {
                std::vector<Bullet> newBullets = player.createBullets(mouseWorldPos);
                bullets.insert(bullets.end(), newBullets.begin(), newBullets.end());
                player.reloadTimer = player.currentReload;
            }

            // Update player bullets
            for (auto it = bullets.begin(); it != bullets.end();)
            {
                it->update(deltaTime);
                
                // Wall collisions
                bool hitWall = false;
                sf::Vector2f bPos = it->getPosition();
                
                PlayingWindow* playingWin = dynamic_cast<PlayingWindow*>(currentWindow.get());
                if (playingWin)
                {
                    if (bPos.x < playingWin->getLeft()) { playingWin->hitWall(0); hitWall = true; }
                    else if (bPos.x > playingWin->getRight()) { playingWin->hitWall(1); hitWall = true; }
                    else if (bPos.y < playingWin->getTop()) { playingWin->hitWall(2); hitWall = true; }
                    else if (bPos.y > playingWin->getBottom()) { playingWin->hitWall(3); hitWall = true; }
                }
                
                if (hitWall) it = bullets.erase(it);
                else ++it;
            }

            // Update enemy bullets
            for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
            {
                it->update(deltaTime);
                
                // Player collision
                sf::Vector2f bPos = it->getPosition();
                sf::Vector2f pPos = player.getPosition();
                float dist = std::sqrt(std::pow(bPos.x - pPos.x, 2) + std::pow(bPos.y - pPos.y, 2));
                
                if (dist < player.getRadius() + it->getRadius())
                {
                    player.takeDamage(it->getDamage());
                    it = enemyBullets.erase(it);
                    continue;
                }

                // Wall collision
                bool hitWall = false;
                PlayingWindow* playingWin = dynamic_cast<PlayingWindow*>(currentWindow.get());
                if (playingWin)
                {
                    sf::Vector2f bPos = it->getPosition();
                    if (bPos.x < playingWin->getLeft() || bPos.x > playingWin->getRight() ||
                        bPos.y < playingWin->getTop() || bPos.y > playingWin->getBottom())
                    {
                        hitWall = true;
                    }
                }

                if (hitWall) it = enemyBullets.erase(it);
                else ++it;
            }

            // Enemy Spawning
            if (enemySpawnClock.getElapsedTime().asSeconds() > 2.0f)
            {
                // Calculate spawn position outside window
                float buffer = 50.0f;
                float x, y;
                int side = rand() % 4;
                
                if (side == 0) // Top
                {
                    x = currentWindow->getLeft() + static_cast<float>(rand() % (int)currentWindow->getWidth());
                    y = currentWindow->getTop() - buffer;
                }
                else if (side == 1) // Bottom
                {
                    x = currentWindow->getLeft() + static_cast<float>(rand() % (int)currentWindow->getWidth());
                    y = currentWindow->getBottom() + buffer;
                }
                else if (side == 2) // Left
                {
                    x = currentWindow->getLeft() - buffer;
                    y = currentWindow->getTop() + static_cast<float>(rand() % (int)currentWindow->getHeight());
                }
                else // Right
                {
                    x = currentWindow->getRight() + buffer;
                    y = currentWindow->getTop() + static_cast<float>(rand() % (int)currentWindow->getHeight());
                }

                sf::Vector2f spawnPos(x, y);
                int roll = rand() % 100;
                int time = stats.timeSurvived; 
                
                // Check if boss active
                bool bossExists = false;
                for(const auto& e : enemies) if(dynamic_cast<Spiker*>(e.get())) bossExists = true;
                
                // Spawn logic
                if (time > 60 && !bossExists && (rand() % 20 == 0))
                {
                    enemies.emplace_back(std::make_shared<Spiker>(spawnPos));
                }
                else
                {
                    if (roll < 50) enemies.emplace_back(std::make_shared<Triangle>(spawnPos));
                    else if (roll < 75) enemies.emplace_back(std::make_shared<Circle>(spawnPos));
                    else enemies.emplace_back(std::make_shared<Square>(spawnPos));
                }
                
                enemySpawnClock.restart();
            }

            // Update enemies
            for (auto it = enemies.begin(); it != enemies.end();)
            {
                std::vector<Bullet> newEnemyBullets = (*it)->update(player.getPosition(), deltaTime);
                enemyBullets.insert(enemyBullets.end(), newEnemyBullets.begin(), newEnemyBullets.end());
                
                // Player collision
                sf::Vector2f ePos = (*it)->getPosition();
                sf::Vector2f pPos = player.getPosition();
                float dist = std::sqrt(std::pow(ePos.x - pPos.x, 2) + std::pow(ePos.y - pPos.y, 2));
                if (dist < player.getRadius() + (*it)->getRadius())
                {
                    player.takeDamage(20);
                    if (dynamic_cast<Spiker*>(it->get())) player.takeDamage(100);
                    
                    // Apply body damage to enemy
                    (*it)->takeDamage(static_cast<int>(player.currentBodyDamage)); 
                }

                // Bullet collision
                bool bulletHit = false;
                for (auto bit = bullets.begin(); bit != bullets.end();)
                {
                    sf::Vector2f bPos = bit->getPosition();
                    float bDist = std::sqrt(std::pow(ePos.x - bPos.x, 2) + std::pow(ePos.y - bPos.y, 2));
                    if (bDist < (*it)->getRadius() + bit->getRadius())
                    {
                        (*it)->takeDamage(bit->getDamage());
                        bit = bullets.erase(bit);
                        bulletHit = true;
                        break;
                    }
                    else
                    {
                        ++bit;
                    }
                }

                if ((*it)->isDead())
                {
                    player.earnXp((*it)->getCurrencyDrop() * 10);
                    stats.enemiesKilled++;
                    it = enemies.erase(it);
                }
                else if (!bulletHit)
                {
                    ++it;
                }
            }

            if (player.isDead())
            {
                currentWindow->resize(675.0f);
                currentState = GameState::GAMEOVER;
            }
        }

        // Render pass
        window.clear(sf::Color(255, 0, 255)); // Transparent key

        window.setView(defaultView);
        currentWindow->draw(window, font);

        if (currentState == GameState::PLAYING && !isTransitioningToPlay)
        {
            // Draw with clipping
            window.setView(currentWindow->getClippingView());

            for (auto &e : enemies) e->draw(window);
            for (auto &b : bullets) b.draw(window);
            for (auto &b : enemyBullets) b.draw(window);
            player.draw(window);

            // Draw HUD
            window.setView(defaultView);
            UIRenderer::drawHUD(window, font, player, *currentWindow);
        }
        else if (currentState == GameState::WELCOME)
        {
            UIRenderer::drawWelcomeScreen(window, font, *currentWindow);
        }
        else if (currentState == GameState::GAMEOVER)
        {
            UIRenderer::drawGameOverScreen(window, font, stats, *currentWindow);
        }

        if (upgradeWindow.getVisible())
        {
            upgradeWindow.draw(window, font);
            UIRenderer::drawUpgradeWindow(window, font, player, mousePixelPos, upgradeWindow);
        }

        window.display();
    }

    return 0;
}
