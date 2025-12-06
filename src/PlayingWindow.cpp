#include "../include/PlayingWindow.hpp"

PlayingWindow::PlayingWindow(int sw, int sh, float initialSize)
    : FakeWindow(sw, sh, initialSize)
{
}

void PlayingWindow::update(float dt)
{
    // Base update handles initial animation
    FakeWindow::update(dt);

    if (isAnimating)
        return;

    // Apply continuous shrinking
    float shrinkAmount = 10.0f * dt;
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

    // Interpolate wall positions
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
    case 0: // Left
        if (targetLeft > limitBuffer)
            targetLeft -= expandAmount;
        break;
    case 1: // Right
        if (targetRight < screenW - limitBuffer)
            targetRight += expandAmount;
        break;
    case 2: // Top
        if (targetTop > limitBuffer)
            targetTop -= expandAmount;
        break;
    case 3: // Bottom
        if (targetBottom < screenH - limitBuffer)
            targetBottom += expandAmount;
        break;
    }
}
