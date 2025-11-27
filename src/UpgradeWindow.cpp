#include "../include/UpgradeWindow.hpp"

UpgradeWindow::UpgradeWindow(int sw, int sh)
<<<<<<< HEAD
    : FakeWindow(sw, sh, 600.0f), screenW(static_cast<float>(sw)), screenH(static_cast<float>(sh)), 
      visible(false), state(UpgradeWindowState::Stats)
=======
    : screenW(static_cast<float>(sw)), screenH(static_cast<float>(sh)),
      isVisible(false)
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
{
    float centerX = screenW / 2.0f;
    float centerY = screenH / 2.0f;
    
<<<<<<< HEAD
    winLeft = centerX - windowWidth / 2.0f;
    winRight = centerX + windowWidth / 2.0f;
    winTop = centerY - windowHeight / 2.0f;
    winBottom = centerY + windowHeight / 2.0f;
}

void UpgradeWindow::toggle()
{
    visible = !visible;
    if (visible)
    {
        state = UpgradeWindowState::Stats; // Reset to stats on open
    }
}

void UpgradeWindow::show()
{
    visible = true;
    state = UpgradeWindowState::Stats;
}

void UpgradeWindow::hide()
{
    visible = false;
}

bool UpgradeWindow::getVisible() const
{
    return visible;
}

void UpgradeWindow::setState(UpgradeWindowState s)
{
    state = s;
}

UpgradeWindowState UpgradeWindow::getState() const
{
    return state;
}

void UpgradeWindow::draw(sf::RenderWindow &window, const sf::Font &font)
{
    if (!visible)
        return;

    float x = winLeft;
    float y = winTop - titleBarHeight;
=======
    left = centerX - windowWidth / 2.0f;
    right = centerX + windowWidth / 2.0f;
    top = centerY - windowHeight / 2.0f;
    bottom = centerY + windowHeight / 2.0f;
}

void UpgradeWindow::show() { isVisible = true; }
void UpgradeWindow::hide() { isVisible = false; }
bool UpgradeWindow::getVisible() const { return isVisible; }
void UpgradeWindow::toggle() { isVisible = !isVisible; }

float UpgradeWindow::getLeft() const { return left; }
float UpgradeWindow::getRight() const { return right; }
float UpgradeWindow::getTop() const { return top; }
float UpgradeWindow::getBottom() const { return bottom; }
float UpgradeWindow::getWidth() const { return windowWidth; }
float UpgradeWindow::getHeight() const { return windowHeight; }
sf::Vector2f UpgradeWindow::getPosition() const { return sf::Vector2f(left, top); }
sf::Vector2f UpgradeWindow::getSize() const { return sf::Vector2f(windowWidth, windowHeight); }

void UpgradeWindow::draw(sf::RenderWindow &window, const sf::Font &font)
{
    if (!isVisible)
        return;

    float x = left;
    float y = top - titleBarHeight;
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd

    // Draw title bar
    sf::RectangleShape titleBar(sf::Vector2f(windowWidth, titleBarHeight));
    titleBar.setPosition(sf::Vector2f(x, y));
    titleBar.setFillColor(sf::Color(45, 45, 48));
    window.draw(titleBar);

    // Draw title text
    sf::Text titleText(font, "Upgrade Shop", 14);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(sf::Vector2f(x + 10, y + 7));
    window.draw(titleText);

    // Draw window control buttons
    float buttonSize = 12.0f;
    float buttonY = y + titleBarHeight / 2.0f - buttonSize / 2.0f;

    auto drawBtn = [&](float offsetX, sf::Color col)
    {
        sf::CircleShape btn(buttonSize / 2.0f);
        btn.setPosition(sf::Vector2f(x + windowWidth - offsetX - buttonSize, buttonY));
        btn.setFillColor(col);
        window.draw(btn);
    };

    drawBtn(15, sf::Color(255, 95, 86));   // Close button
    drawBtn(35, sf::Color(40, 201, 64));   // Maximize button
    drawBtn(55, sf::Color(255, 189, 46));  // Minimize button

    // Draw window background
    sf::RectangleShape background(sf::Vector2f(windowWidth, windowHeight));
<<<<<<< HEAD
    background.setPosition(sf::Vector2f(winLeft, winTop));
=======
    background.setPosition(sf::Vector2f(left, top));
>>>>>>> 10429a210025b856e15b58ac6bad7062ef9f51fd
    background.setFillColor(sf::Color(20, 20, 25));
    window.draw(background);

    // Draw window border
    sf::RectangleShape border(sf::Vector2f(windowWidth, windowHeight + titleBarHeight));
    border.setPosition(sf::Vector2f(x, y));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color(100, 100, 100));
    border.setOutlineThickness(borderThickness);
    window.draw(border);
}
