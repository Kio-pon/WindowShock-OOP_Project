#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Upgrade.hpp"
#include "FakeWindow.hpp"
#include "UpgradeWindow.hpp"
#include "GameStats.hpp"

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
    static void centerText(sf::Text &text, float x, float y);
};
