#include "../include/FakeWindow.hpp"

FakeWindow::FakeWindow(int sw, int sh, float initialSize)
    : screenW(static_cast<float>(sw)), screenH(static_cast<float>(sh)),
      isAnimating(false)
{
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;
    float halfSize = initialSize / 2.0f;

    currentLeft = targetLeft = centerX - halfSize;
    currentRight = targetRight = centerX + halfSize;
    currentTop = targetTop = centerY - halfSize;
    currentBottom = targetBottom = centerY + halfSize;
}

void FakeWindow::resize(float newSize)
{
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;
    float halfSize = newSize / 2.0f;

    currentLeft = targetLeft = centerX - halfSize;
    currentRight = targetRight = centerX + halfSize;
    currentTop = targetTop = centerY - halfSize;
    currentBottom = targetBottom = centerY + halfSize;
}

void FakeWindow::startCollapseAnimation()
{
    isAnimating = true;
    animationProgress = 0.0f;
    animationDuration = 2.0f;

    animStartRect = sf::FloatRect({currentLeft, currentTop}, {currentRight - currentLeft, currentBottom - currentTop});

    float centerX = (currentLeft + currentRight) / 2.0f;
    float centerY = (currentTop + currentBottom) / 2.0f;
    float smallSize = 300.0f;

    animTargetRect = sf::FloatRect({centerX - smallSize / 2, centerY - smallSize / 2}, {smallSize, smallSize});

    targetLeft = animTargetRect.position.x;
    targetTop = animTargetRect.position.y;
    targetRight = animTargetRect.position.x + animTargetRect.size.x;
    targetBottom = animTargetRect.position.y + animTargetRect.size.y;
}

void FakeWindow::update(float dt)
{
    // Handle smooth animation
    if (isAnimating)
    {
        animationProgress += dt / animationDuration;
        if (animationProgress >= 1.0f)
        {
            isAnimating = false;
            animationProgress = 1.0f;
            currentLeft = targetLeft;
            currentRight = targetRight;
            currentTop = targetTop;
            currentBottom = targetBottom;
        }
        else
        {
            // Smooth easing function
            float t = 1.0f - std::pow(1.0f - animationProgress, 3.0f);

            currentLeft = animStartRect.position.x + (animTargetRect.position.x - animStartRect.position.x) * t;

            float startRight = animStartRect.position.x + animStartRect.size.x;
            float targetRightVal = animTargetRect.position.x + animTargetRect.size.x;
            currentRight = startRight + (targetRightVal - startRight) * t;

            currentTop = animStartRect.position.y + (animTargetRect.position.y - animStartRect.position.y) * t;

            float startBottom = animStartRect.position.y + animStartRect.size.y;
            float targetBottomVal = animTargetRect.position.y + animTargetRect.size.y;
            currentBottom = startBottom + (targetBottomVal - startBottom) * t;
        }
        return;
    }
<<<<<<< HEAD
=======

    // Gradually shrink the window
    if (targetRight - targetLeft > minWidth)
    {
        targetLeft += collapseSpeed;
        targetRight -= collapseSpeed;
    }
    if (targetBottom - targetTop > minHeight)
    {
        targetTop += collapseSpeed;
        targetBottom -= collapseSpeed;
    }

    // Smoothly move current walls toward target positions
    auto moveTowards = [&](float &current, float target)
    {
        if (std::abs(current - target) < expandSpeed)
        {
            current = target;
        }
        else if (current < target)
        {
            current += expandSpeed;
        }
        else
        {
            current -= expandSpeed;
        }
    };

    moveTowards(currentLeft, targetLeft);
    moveTowards(currentRight, targetRight);
    moveTowards(currentTop, targetTop);
    moveTowards(currentBottom, targetBottom);
}

void FakeWindow::hitWall(int wallIndex)
{
    if (isAnimating)
        return;

    float limitBuffer = 50.0f;

    switch (wallIndex)
    {
    case 0: // Left wall
        if (targetLeft > limitBuffer)
            targetLeft -= expandAmount;
        break;
    case 1: // Right wall
        if (targetRight < screenW - limitBuffer)
            targetRight += expandAmount;
        break;
    case 2: // Top wall
        if (targetTop > limitBuffer)
            targetTop -= expandAmount;
        break;
    case 3: // Bottom wall
        if (targetBottom < screenH - limitBuffer)
            targetBottom += expandAmount;
        break;
    }
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
}

sf::View FakeWindow::getClippingView() const
{
    sf::View view;
    float w = getWidth();
    float h = getHeight();

    view.setCenter({currentLeft + w / 2.0f, currentTop + h / 2.0f});
    view.setSize({w, h});

    // Set viewport to match window position on screen
    float vpX = currentLeft / screenW;
    float vpY = currentTop / screenH;
    float vpW = w / screenW;
    float vpH = h / screenH;

    view.setViewport(sf::FloatRect({vpX, vpY}, {vpW, vpH}));
    return view;
}

float FakeWindow::getLeft() const { return currentLeft; }
float FakeWindow::getRight() const { return currentRight; }
float FakeWindow::getTop() const { return currentTop; }
float FakeWindow::getBottom() const { return currentBottom; }

float FakeWindow::getWidth() const { return currentRight - currentLeft; }
float FakeWindow::getHeight() const { return currentBottom - currentTop; }

sf::Vector2f FakeWindow::getPosition() const { return sf::Vector2f(currentLeft, currentTop); }
sf::Vector2f FakeWindow::getSize() const { return sf::Vector2f(getWidth(), getHeight()); }

bool FakeWindow::isAnimationComplete() const { return !isAnimating; }

void FakeWindow::draw(sf::RenderWindow &window, const sf::Font &font)
{
    float w = getWidth();
    float h = getHeight();
    float x = currentLeft;
    float y = currentTop - titleBarHeight;

    // Draw title bar
    sf::RectangleShape titleBar(sf::Vector2f(w, titleBarHeight));
    titleBar.setPosition(sf::Vector2f(x, y));
    titleBar.setFillColor(sf::Color(45, 45, 48));
    window.draw(titleBar);

    // Draw title text
    sf::Text titleText(font, "WindowShock Game", 14);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(x + 10, y + 7));
    window.draw(titleText);

    // Draw window control buttons
    float buttonSize = 12.0f;
    float buttonY = y + titleBarHeight / 2.0f - buttonSize / 2.0f;

    auto drawBtn = [&](float offsetX, sf::Color col)
    {
        sf::CircleShape btn(buttonSize / 2.0f);
        btn.setPosition(sf::Vector2f(x + w - offsetX - buttonSize, buttonY));
        btn.setFillColor(col);
        window.draw(btn);
    };

    drawBtn(15, sf::Color(255, 95, 86));   // Close button
    drawBtn(35, sf::Color(40, 201, 64));   // Maximize button
    drawBtn(55, sf::Color(255, 189, 46));  // Minimize button

    // Draw black game area
    sf::RectangleShape gameArea(sf::Vector2f(w, h));
    gameArea.setPosition(sf::Vector2f(currentLeft, currentTop));
    gameArea.setFillColor(sf::Color::Black);
    window.draw(gameArea);

    // Draw window border
    sf::RectangleShape border(sf::Vector2f(w, h + titleBarHeight));
    border.setPosition(sf::Vector2f(x, y));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(100, 100, 100));
    border.setOutlineThickness(borderThickness);
    window.draw(border);
}
