#include "../include/PlayingWindow.hpp"

PlayingWindow::PlayingWindow(int sw, int sh, float initialSize)
    : FakeWindow(sw, sh, initialSize)
{
}

void PlayingWindow::update(float dt)
{
    // Call base update for animation handling
    FakeWindow::update(dt);

    if (isAnimating)
        return;

    // Gradually shrink the window
    float shrinkAmount = 10.0f * dt; // 10 pixels per second per side
    if (targetRight - targetLeft > minWidth)
    {
        targetLeft += shrinkAmount;
        targetRight -= shrinkAmount;
    }
    if (targetBottom - targetTop > minHeight)
    {
        targetTop += shrinkAmount;
        targetBottom -= shrinkAmount;
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

void PlayingWindow::hitWall(int wallIndex)
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
}
