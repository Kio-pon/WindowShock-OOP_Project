#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

// Base class for different window types
class FakeWindow
{
protected:
    // Current and target positions of window edges
    float currentLeft, currentRight, currentTop, currentBottom;
    float targetLeft, targetRight, targetTop, targetBottom;

    float screenW, screenH;

    const float titleBarHeight = 30.0f;
    const float borderThickness = 3.0f;

    // Animation state for smooth transitions
    bool isAnimating;
    float animationProgress;
    float animationDuration;

    sf::FloatRect animStartRect;
    sf::FloatRect animTargetRect;

public:
    // Initialize window centered on screen
    FakeWindow(int sw, int sh, float initialSize);
    virtual ~FakeWindow() = default;

    // Resize window to a new size
    void resize(float newSize);

    // Start animation to collapse window to smaller size
    // Made virtual so derived classes can implement specific animations if needed
    virtual void startCollapseAnimation();

    // Update window position and size each frame
    virtual void update(float dt);

    // Create a view that only shows content inside the window
    sf::View getClippingView() const;

    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;

    float getWidth() const;
    float getHeight() const;

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    bool isAnimationComplete() const;

    // Draw the window frame and background
    virtual void draw(sf::RenderWindow &window, const sf::Font &font);
};
