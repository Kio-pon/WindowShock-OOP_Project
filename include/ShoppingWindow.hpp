#pragma once
#include "FakeWindow.hpp"

class ShoppingWindow : public FakeWindow
{
public:
    ShoppingWindow(int sw, int sh, float initialSize);
    
    // Can override update/draw if needed for shop specific effects
};
