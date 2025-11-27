#pragma once
#include <SFML/Graphics.hpp>
#include "FakeWindow.hpp"

enum class UpgradeWindowState
{
    Stats,
    ClassSelection
};

// Represents a separate upgrade shop window that appears above the game
class UpgradeWindow : public FakeWindow
{
private:
    float winLeft, winRight, winTop, winBottom;
    float screenW, screenH;
    const float titleBarHeight = 30.0f;
    const float borderThickness = 3.0f;
    const float windowWidth = 700.0f;
    const float windowHeight = 500.0f;
    bool visible;
    UpgradeWindowState state;

public:
    UpgradeWindow(int sw, int sh);

    void toggle();
    void show();
    void hide();
    bool getVisible() const;
    
    void setState(UpgradeWindowState s);
    UpgradeWindowState getState() const;

    float getLeft() const { return winLeft; }
    float getRight() const { return winRight; }
    float getTop() const { return winTop; }
    float getBottom() const { return winBottom; }
    float getWidth() const { return windowWidth; }
    float getHeight() const { return windowHeight; }
    sf::Vector2f getPosition() const { return sf::Vector2f(winLeft, winTop); }
    sf::Vector2f getSize() const { return sf::Vector2f(windowWidth, windowHeight); }

    // Draw the upgrade window frame and background
    void draw(sf::RenderWindow &window, const sf::Font &font) override;
};
