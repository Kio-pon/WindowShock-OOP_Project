#pragma once
#include "FakeWindow.hpp"

class PlayingWindow : public FakeWindow
{
private:
    // Minimum window size to prevent crushing the player
    const float minWidth = 250.0f;
    const float minHeight = 250.0f;

    // How much the window expands when hit
    const float expandAmount = 20.0f;
    // How fast the window shrinks
    const float collapseSpeed = 0.2f;
    // How fast the window expands back
    const float expandSpeed = 15.0f;

public:
    PlayingWindow(int sw, int sh, float initialSize);

    void update(float dt) override;
    void hitWall(int wallIndex);
};
