#pragma once
#include <SFML/Graphics.hpp>
<<<<<<< HEAD
=======
#include <vector>
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
#include "Player.hpp"
#include "Upgrade.hpp"
#include "FakeWindow.hpp"
#include "UpgradeWindow.hpp"
#include "GameStats.hpp"
<<<<<<< HEAD
#include "TankClass.hpp"

class UIRenderer
{
public:
    static void drawWelcomeScreen(sf::RenderWindow &window, const sf::Font &font, const FakeWindow &fw);
    static void drawGameOverScreen(sf::RenderWindow &window, const sf::Font &font, const GameStats &stats, const FakeWindow &fw);
    
    // Updated for Diep.io UI
    static void drawHUD(sf::RenderWindow &window, const sf::Font &font, const Player &player, const FakeWindow &fw);
    static void drawUpgradeWindow(sf::RenderWindow &window, const sf::Font &font, Player &player, sf::Vector2i mousePos, UpgradeWindow &uw);
    
    // Helper to draw a single stat bar
    static void drawStatBar(sf::RenderWindow &window, const sf::Font &font, sf::Vector2f pos, std::string label, int level, sf::Color color, bool canUpgrade, sf::Vector2i mousePos, int statIndex, Player &player);

private:
=======

// Handles drawing UI screens
class UIRenderer
{
public:
    // Draw the welcome screen
    static void drawWelcomeScreen(sf::RenderWindow &window, const sf::Font &font, const FakeWindow &fakeWin);

    // Draw the shop screen with upgrade cards in the main game window
    static void drawShopScreen(sf::RenderWindow &window, const sf::Font &font, Player &player,
                               std::vector<Upgrade> &upgrades, sf::Vector2i mousePos,
                               const FakeWindow &fakeWin, bool isOverlay = false);

    // Draw the shop screen in a separate upgrade window
    static void drawUpgradeWindow(sf::RenderWindow &window, const sf::Font &font, Player &player,
                                  std::vector<Upgrade> &upgrades, sf::Vector2i mousePos,
                                  const UpgradeWindow &upgradeWin);

    // Draw the game over screen with stats
    static void drawGameOverScreen(sf::RenderWindow &window, const sf::Font &font,
                                   const GameStats &stats, const FakeWindow &fakeWin);

private:
    // Center text at given coordinates
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
    static void centerText(sf::Text &text, float x, float y);
};
