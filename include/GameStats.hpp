#pragma once

// Tracks player statistics during gameplay
struct GameStats
{
    int coinsCollected = 0;
    int starsCollected = 0;
    int timeSurvived = 0;
    int bulletsFired = 0;
    int enemiesKilled = 0;
    int bossesKilled = 0;
};
