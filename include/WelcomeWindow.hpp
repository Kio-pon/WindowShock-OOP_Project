#pragma once
#include "FakeWindow.hpp"

class WelcomeWindow : public FakeWindow
{
public:
    WelcomeWindow(int sw, int sh, float initialSize);

    // Can override update/draw if needed for welcome screen specific effects
};
