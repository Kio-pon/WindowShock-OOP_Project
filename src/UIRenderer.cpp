#include "../include/UIRenderer.hpp"
#include <iostream>

void UIRenderer::centerText(sf::Text &text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f));
    text.setPosition(sf::Vector2f(x, y));
}

void UIRenderer::drawWelcomeScreen(sf::RenderWindow &window, const sf::Font &font, const FakeWindow &fw)
{
    sf::Vector2f pos = fw.getPosition();
    sf::Vector2f size = fw.getSize();
    float centerX = pos.x + size.x / 2.0f;
    float centerY = pos.y + size.y / 2.0f;

    sf::Text title(font, "WindowShock", 50);
    title.setFillColor(sf::Color::White);
    centerText(title, centerX, centerY - 50);
    window.draw(title);

    sf::Text sub(font, "Press SPACE to Start", 20);
    sub.setFillColor(sf::Color(200, 200, 200));
    centerText(sub, centerX, centerY + 20);
    window.draw(sub);
}

void UIRenderer::drawGameOverScreen(sf::RenderWindow &window, const sf::Font &font, const GameStats &stats, const FakeWindow &fw)
{
    sf::Vector2f pos = fw.getPosition();
    sf::Vector2f size = fw.getSize();
    float centerX = pos.x + size.x / 2.0f;
    float centerY = pos.y + size.y / 2.0f;

    sf::Text title(font, "GAME OVER", 50);
    title.setFillColor(sf::Color::Red);
    centerText(title, centerX, centerY - 60);
    window.draw(title);

    sf::Text score(font, "Enemies Killed: " + std::to_string(stats.enemiesKilled), 20);
    score.setFillColor(sf::Color::White);
    centerText(score, centerX, centerY);
    window.draw(score);

    sf::Text time(font, "Time Survived: " + std::to_string(stats.timeSurvived) + "s", 20);
    time.setFillColor(sf::Color::White);
    centerText(time, centerX, centerY + 30);
    window.draw(time);
    
    sf::Text restart(font, "Press SPACE to Restart", 18);
    restart.setFillColor(sf::Color(150, 150, 150));
    centerText(restart, centerX, centerY + 80);
    window.draw(restart);
}

void UIRenderer::drawHUD(sf::RenderWindow &window, const sf::Font &font, const Player &player, const FakeWindow &fw)
{
    sf::Vector2f winPos = fw.getPosition();
    sf::Vector2f winSize = fw.getSize();
    
    // Draw XP Bar
    float barWidth = winSize.x * 0.6f;
    float barHeight = 10.0f;
    float barX = winPos.x + (winSize.x - barWidth) / 2.0f;
    float barY = winPos.y + winSize.y - 30.0f;
    
    sf::RectangleShape xpBg(sf::Vector2f(barWidth, barHeight));
    xpBg.setPosition(sf::Vector2f(barX, barY));
    xpBg.setFillColor(sf::Color(50, 50, 50));
    window.draw(xpBg);
    
    float xpRatio = 0.0f;
    int req = player.getXpForNextLevel();
    if (req > 0) xpRatio = (float)player.xp / req;
    if (xpRatio > 1.0f) xpRatio = 1.0f;
    
    sf::RectangleShape xpFill(sf::Vector2f(barWidth * xpRatio, barHeight));
    xpFill.setPosition(sf::Vector2f(barX, barY));
    xpFill.setFillColor(sf::Color(255, 215, 0));
    window.draw(xpFill);
    
    sf::Text lvlText(font, "Lvl " + std::to_string(player.level), 16);
    lvlText.setFillColor(sf::Color::White);
    centerText(lvlText, barX + barWidth / 2.0f, barY - 15.0f);
    window.draw(lvlText);
    
    // Display tank upgrade availability
    if (player.level >= 10 && player.currentTank->getUpgrades().size() > 0)
    {
        sf::Text upgText(font, "Tank Upgrade Available!", 18);
        upgText.setFillColor(sf::Color::Cyan);
        upgText.setPosition(sf::Vector2f(winPos.x + winSize.x - 220, winPos.y + 40));
        window.draw(upgText);
    }
}

void UIRenderer::drawStatBar(sf::RenderWindow &window, const sf::Font &font, sf::Vector2f pos, std::string label, int level, sf::Color color, bool canUpgrade, sf::Vector2i mousePos, int statIndex, Player &player)
{
    // Draw background
    sf::RectangleShape bg(sf::Vector2f(250.0f, 20.0f));
    bg.setPosition(pos);
    bg.setFillColor(sf::Color(30, 30, 30));
    bg.setOutlineColor(sf::Color(60, 60, 60));
    bg.setOutlineThickness(1.0f);
    window.draw(bg);
    
    // Draw fill level
    float fillWidth = (250.0f / 7.0f) * level;
    sf::RectangleShape fill(sf::Vector2f(fillWidth, 20.0f));
    fill.setPosition(pos);
    fill.setFillColor(color);
    window.draw(fill);
    
    // Draw label
    sf::Text lbl(font, label, 14);
    lbl.setFillColor(sf::Color::White);
    lbl.setPosition(sf::Vector2f(pos.x + 10, pos.y + 2));
    window.draw(lbl);
    
    // Draw upgrade button
    if (canUpgrade && level < 7)
    {
        sf::RectangleShape btn(sf::Vector2f(20.0f, 20.0f));
        btn.setPosition(sf::Vector2f(pos.x + 260.0f, pos.y));
        btn.setFillColor(color);
        
        // Handle hover effect
        sf::FloatRect btnRect(sf::Vector2f(pos.x + 260.0f, pos.y), sf::Vector2f(20.0f, 20.0f));
        if (btnRect.contains(sf::Vector2f((float)mousePos.x, (float)mousePos.y)))
        {
            btn.setOutlineColor(sf::Color::White);
            btn.setOutlineThickness(2.0f);
            
            // Interaction logic is handled in valid event loop, visual only here
        }
        
        window.draw(btn);
        
        sf::Text plus(font, "+", 16);
        plus.setFillColor(sf::Color::Black);
        centerText(plus, pos.x + 270.0f, pos.y + 10.0f);
        window.draw(plus);
    }
}

void UIRenderer::drawUpgradeWindow(sf::RenderWindow &window, const sf::Font &font, Player &player, sf::Vector2i mousePos, UpgradeWindow &uw)
{
    sf::Vector2f pos = uw.getPosition();
    sf::Vector2f size = uw.getSize();
    
    if (uw.getState() == UpgradeWindowState::Stats)
    {
        sf::Text title(font, "Stats Upgrade (Points: " + std::to_string(player.skillPoints) + ")", 24);
        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(pos.x + 50, pos.y + 50));
        window.draw(title);
        
        float startY = pos.y + 100;
        float gap = 35.0f;
        
        // Render 8 stat bars with distinct colors
        drawStatBar(window, font, {pos.x + 50, startY + gap*0}, "Health Regen", player.statLevels[0], sf::Color(255, 150, 100), player.skillPoints > 0, mousePos, 0, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*1}, "Max Health", player.statLevels[1], sf::Color(255, 100, 255), player.skillPoints > 0, mousePos, 1, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*2}, "Body Damage", player.statLevels[2], sf::Color(150, 100, 255), player.skillPoints > 0, mousePos, 2, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*3}, "Bullet Speed", player.statLevels[3], sf::Color(100, 150, 255), player.skillPoints > 0, mousePos, 3, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*4}, "Bullet Pen.", player.statLevels[4], sf::Color(255, 255, 100), player.skillPoints > 0, mousePos, 4, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*5}, "Bullet Damage", player.statLevels[5], sf::Color(255, 100, 100), player.skillPoints > 0, mousePos, 5, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*6}, "Reload", player.statLevels[6], sf::Color(100, 255, 100), player.skillPoints > 0, mousePos, 6, player);
        drawStatBar(window, font, {pos.x + 50, startY + gap*7}, "Movement Spd", player.statLevels[7], sf::Color(100, 255, 255), player.skillPoints > 0, mousePos, 7, player);
        
        // Tank Upgrade Button
        if (player.level >= 10 && player.currentTank->getTier() < 3)
        {
            sf::RectangleShape btn(sf::Vector2f(200.0f, 40.0f));
            btn.setPosition(sf::Vector2f(pos.x + size.x - 250, pos.y + 50));
            btn.setFillColor(sf::Color(0, 100, 200));
            window.draw(btn);
            
            sf::Text txt(font, "Tank Upgrade ->", 18);
            txt.setFillColor(sf::Color::White);
            centerText(txt, pos.x + size.x - 150, pos.y + 70);
            window.draw(txt);
        }
    }
    else if (uw.getState() == UpgradeWindowState::ClassSelection)
    {
        sf::Text title(font, "Select Class", 30);
        title.setFillColor(sf::Color::White);
        centerText(title, pos.x + size.x/2, pos.y + 50);
        window.draw(title);
        
        // Back Button
        sf::RectangleShape backBtn(sf::Vector2f(100.0f, 30.0f));
        backBtn.setPosition(sf::Vector2f(pos.x + 20, pos.y + 20));
        backBtn.setFillColor(sf::Color(100, 100, 100));
        window.draw(backBtn);
        
        sf::Text backTxt(font, "<- Back", 16);
        backTxt.setFillColor(sf::Color::White);
        centerText(backTxt, pos.x + 70, pos.y + 35);
        window.draw(backTxt);
        
        auto upgrades = player.currentTank->getUpgrades();
        
        // Draw Class Options in grid
        float startX = pos.x + 100;
        float startY = pos.y + 150;
        float boxSize = 120;
        float gap = 30;
        
        for (size_t i = 0; i < upgrades.size(); i++)
        {
            float x = startX + (i % 3) * (boxSize + gap);
            float y = startY + (i / 3) * (boxSize + gap);
            
            sf::RectangleShape box(sf::Vector2f(boxSize, boxSize));
            box.setPosition(sf::Vector2f(x, y));
            box.setFillColor(sf::Color(0, 178, 225));
            box.setOutlineColor(sf::Color::White);
            box.setOutlineThickness(2);
            window.draw(box);
            
            // Preview tank configuration
            Player tempPlayer(15.0f, 0.0f, x + boxSize/2, y + boxSize/2);
            upgrades[i]->configure(tempPlayer);
            tempPlayer.draw(window);
            
            sf::Text nameTxt(font, upgrades[i]->getName(), 14);
            nameTxt.setFillColor(sf::Color::White);
            centerText(nameTxt, x + boxSize/2, y + boxSize + 15);
            window.draw(nameTxt);
        }
    }
}
