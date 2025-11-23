#pragma once
#include <SFML/Graphics.hpp>

// Represents a separate upgrade shop window that appears above the game
class UpgradeWindow
{
private:
    float left, right, top, bottom;
    float screenW, screenH;
    const float titleBarHeight = 30.0f;
    const float borderThickness = 3.0f;
    const float windowWidth = 700.0f;
    const float windowHeight = 500.0f;
    bool isVisible;

public:
    // Initialize upgrade window centered on screen
    UpgradeWindow(int sw, int sh);

    void show();
    void hide();
    bool getVisible() const;
    void toggle();

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    float getWidth() const;
    float getHeight() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    // Draw the upgrade window frame and background
    void draw(sf::RenderWindow &window, const sf::Font &font);
};
