#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

<<<<<<< HEAD
// Base class for different window types
class FakeWindow
{
protected:
=======
// Manages the shrinking game window boundaries
class FakeWindow
{
private:
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
    // Current and target positions of window edges
    float currentLeft, currentRight, currentTop, currentBottom;
    float targetLeft, targetRight, targetTop, targetBottom;

<<<<<<< HEAD
    float screenW, screenH;

=======
    // Minimum window size to prevent crushing the player
    const float minWidth = 250.0f;
    const float minHeight = 250.0f;

    float screenW, screenH;

    // How much the window expands when hit
    const float expandAmount = 20.0f;
    // How fast the window shrinks
    const float collapseSpeed = 0.2f;
    // How fast the window expands back
    const float expandSpeed = 15.0f;

>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
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
<<<<<<< HEAD
    virtual ~FakeWindow() = default;
=======
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd

    // Resize window to a new size
    void resize(float newSize);

    // Start animation to collapse window to smaller size
<<<<<<< HEAD
    // Made virtual so derived classes can implement specific animations if needed
    virtual void startCollapseAnimation();

    // Update window position and size each frame
    virtual void update(float dt);
=======
    void startCollapseAnimation();

    // Update window position and size each frame
    void update(float dt);

    // Expand window when a wall is hit
    void hitWall(int wallIndex);
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd

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
<<<<<<< HEAD
    virtual void draw(sf::RenderWindow &window, const sf::Font &font);
=======
    void draw(sf::RenderWindow &window, const sf::Font &font);
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
};
