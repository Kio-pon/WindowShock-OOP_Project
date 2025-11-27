#pragma once
#include <SFML/Graphics.hpp>
<<<<<<< HEAD
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
=======

// Represents a separate upgrade shop window that appears above the game
class UpgradeWindow
{
private:
    float left, right, top, bottom;
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
    float screenW, screenH;
    const float titleBarHeight = 30.0f;
    const float borderThickness = 3.0f;
    const float windowWidth = 700.0f;
    const float windowHeight = 500.0f;
<<<<<<< HEAD
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
=======
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
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
};
