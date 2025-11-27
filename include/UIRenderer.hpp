#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Upgrade.hpp"
#include "FakeWindow.hpp"
#include "UpgradeWindow.hpp"
#include "GameStats.hpp"
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
    static void centerText(sf::Text &text, float x, float y);
};
