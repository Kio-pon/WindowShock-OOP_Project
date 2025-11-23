#pragma once
#include <string>

// Represents a purchasable upgrade in the shop
struct Upgrade
{
    std::string name;
    std::string description;
    int cost;
    int level;
    int maxLevel;
    bool locked;

    Upgrade(std::string n, std::string desc, int c, int max = 10)
        : name(n), description(desc), cost(c), level(1), maxLevel(max), locked(false) {}
};
