#include "../include/UIRenderer.hpp"
#include <string>

void UIRenderer::drawWelcomeScreen(sf::RenderWindow &window, const sf::Font &font, const FakeWindow &fakeWin)
{
    sf::Vector2f pos = fakeWin.getPosition();
    sf::Vector2f size = fakeWin.getSize();

    float centerX = pos.x + size.x / 2.0f;
    float centerY = pos.y + size.y / 2.0f;

    sf::Text title(font, "WINDOWSHOCK", 80);
    title.setFillColor(sf::Color(138, 43, 226));
    title.setStyle(sf::Text::Bold);
    centerText(title, centerX, centerY - 100);
    window.draw(title);

    sf::Text instructions(font, "Press SPACE to Start", 30);
    instructions.setFillColor(sf::Color::White);
    centerText(instructions, centerX, centerY);
    window.draw(instructions);

    sf::Text info(font, "WASD - Move | Mouse - Aim | Click - Shoot | TAB - Shop", 20);
    info.setFillColor(sf::Color(150, 150, 150));
    centerText(info, centerX, centerY + 60);
    window.draw(info);
}

void UIRenderer::drawShopScreen(sf::RenderWindow &window, const sf::Font &font, Player &player,
                                std::vector<Upgrade> &upgrades, sf::Vector2i mousePos,
                                const FakeWindow &fakeWin, bool isOverlay)
{
    sf::Vector2f pos = fakeWin.getPosition();
    sf::Vector2f size = fakeWin.getSize();
    float centerX = pos.x + size.x / 2.0f;

    // Draw semi-transparent background
    sf::RectangleShape bg(size);
    bg.setPosition(pos);
    bg.setFillColor(sf::Color(0, 0, 0, 230));
    window.draw(bg);

    // Display player currency
    sf::Text currency(font, std::to_string(player.currency), 50);
    currency.setFillColor(sf::Color(138, 43, 226));
    currency.setStyle(sf::Text::Bold);
    currency.setPosition(sf::Vector2f(centerX - 40, pos.y + 40));
    window.draw(currency);

    sf::Text shopTitle(font, isOverlay ? "SHOP (TAB to close)" : "SHOP (SPACE to play)", 30);
    shopTitle.setFillColor(sf::Color(150, 150, 150));
    centerText(shopTitle, centerX, pos.y + 120);
    window.draw(shopTitle);

    // Draw upgrade cards
    float cardWidth = 200;
    float cardHeight = 220;
    float totalCardsWidth = (cardWidth * 3) + (20 * 2);
    float startX = centerX - totalCardsWidth / 2.0f;
    float startY = pos.y + 180;

    for (size_t i = 0; i < upgrades.size() && i < 3; i++)
    {
        float x = startX + i * (cardWidth + 20);
        float y = startY;

        bool isHovered = mousePos.x >= x && mousePos.x <= x + cardWidth &&
                         mousePos.y >= y && mousePos.y <= y + cardHeight;

        // Draw card background
        sf::RectangleShape card(sf::Vector2f(cardWidth, cardHeight));
        card.setPosition(sf::Vector2f(x, y));
        card.setFillColor(sf::Color::Transparent);
        card.setOutlineColor(isHovered ? sf::Color::White : sf::Color(100, 100, 100));
        card.setOutlineThickness(2);
        window.draw(card);

        // Draw upgrade level
        sf::Text levelText(font, "lv. " + std::to_string(upgrades[i].level), 20);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(sf::Vector2f(x + 10, y + 10));
        window.draw(levelText);

        // Draw upgrade name
        sf::Text nameText(font, upgrades[i].name, 22);
        nameText.setFillColor(sf::Color::White);
        nameText.setStyle(sf::Text::Bold);
        centerText(nameText, x + cardWidth / 2, y + 150);
        window.draw(nameText);

        // Draw upgrade cost
        sf::Text costText(font, std::to_string(upgrades[i].cost), 30);
        costText.setFillColor(sf::Color(138, 43, 226));
        costText.setStyle(sf::Text::Bold);
        centerText(costText, x + cardWidth / 2, y + 190);
        window.draw(costText);
    }
}

void UIRenderer::drawUpgradeWindow(sf::RenderWindow &window, const sf::Font &font, Player &player,
                                   std::vector<Upgrade> &upgrades, sf::Vector2i mousePos,
                                   const UpgradeWindow &upgradeWin)
{
    if (!upgradeWin.getVisible())
        return;

    sf::Vector2f pos = upgradeWin.getPosition();
    sf::Vector2f size = upgradeWin.getSize();
    float centerX = pos.x + size.x / 2.0f;

    // Display player currency
    sf::Text currency(font, std::to_string(player.currency), 50);
    currency.setFillColor(sf::Color(138, 43, 226));
    currency.setStyle(sf::Text::Bold);
    currency.setPosition(sf::Vector2f(centerX - 40, pos.y + 40));
    window.draw(currency);

    sf::Text shopTitle(font, "UPGRADE SHOP (TAB to close)", 30);
    shopTitle.setFillColor(sf::Color(200, 200, 200));
    centerText(shopTitle, centerX, pos.y + 120);
    window.draw(shopTitle);

    // Draw upgrade cards
    float cardWidth = 200;
    float cardHeight = 220;
    float totalCardsWidth = (cardWidth * 3) + (20 * 2);
    float startX = centerX - totalCardsWidth / 2.0f;
    float startY = pos.y + 180;

    for (size_t i = 0; i < upgrades.size() && i < 3; i++)
    {
        float x = startX + i * (cardWidth + 20);
        float y = startY;

        bool isHovered = mousePos.x >= x && mousePos.x <= x + cardWidth &&
                         mousePos.y >= y && mousePos.y <= y + cardHeight;

        // Draw card background
        sf::RectangleShape card(sf::Vector2f(cardWidth, cardHeight));
        card.setPosition(sf::Vector2f(x, y));
        card.setFillColor(sf::Color(30, 30, 35));
        card.setOutlineColor(isHovered ? sf::Color::White : sf::Color(100, 100, 100));
        card.setOutlineThickness(2);
        window.draw(card);

        // Draw upgrade level
        sf::Text levelText(font, "lv. " + std::to_string(upgrades[i].level), 20);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(sf::Vector2f(x + 10, y + 10));
        window.draw(levelText);

        // Draw upgrade name
        sf::Text nameText(font, upgrades[i].name, 22);
        nameText.setFillColor(sf::Color::White);
        nameText.setStyle(sf::Text::Bold);
        centerText(nameText, x + cardWidth / 2, y + 150);
        window.draw(nameText);

        // Draw upgrade cost
        sf::Text costText(font, std::to_string(upgrades[i].cost), 30);
        costText.setFillColor(sf::Color(138, 43, 226));
        costText.setStyle(sf::Text::Bold);
        centerText(costText, x + cardWidth / 2, y + 190);
        window.draw(costText);
    }
}

void UIRenderer::drawGameOverScreen(sf::RenderWindow &window, const sf::Font &font,
                                    const GameStats &stats, const FakeWindow &fakeWin)
{
    sf::Vector2f pos = fakeWin.getPosition();
    sf::Vector2f size = fakeWin.getSize();
    float centerX = pos.x + size.x / 2.0f;

    sf::RectangleShape background(size);
    background.setPosition(pos);
    background.setFillColor(sf::Color(0, 0, 0, 230));
    window.draw(background);

    sf::Text title(font, "GAME OVER", 80);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    centerText(title, centerX, pos.y + 120);
    window.draw(title);

    float startY = pos.y + 250;
    float spacing = 40;

    // Helper function to draw stat lines
    auto draw = [&](std::string label, int val, int idx)
    {
        sf::Text t(font, label + std::to_string(val), 24);
        t.setFillColor(sf::Color::White);
        centerText(t, centerX, startY + idx * spacing);
        window.draw(t);
    };

    draw("Time Survived: ", stats.timeSurvived, 0);
    draw("Enemies Killed: ", stats.enemiesKilled, 1);
    draw("Coins Collected: ", stats.coinsCollected, 2);

    sf::Text instructions(font, "Close Window to Exit", 25);
    instructions.setFillColor(sf::Color(150, 150, 150));
    centerText(instructions, centerX, pos.y + size.y - 60);
    window.draw(instructions);
}

void UIRenderer::centerText(sf::Text &text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    text.setPosition(sf::Vector2f(x, y));
}
