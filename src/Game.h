// Game.h —— 页面状态机、菜单交互与存档
#pragma once

#include <memory>
#include <string>
#include <vector>

#include <graphics.h>

#include "Board.h"
#include "Leaderboard.h"
#include "Player.h"

class Game {
public:
    Game();
    ~Game() = default;

    void run();

private:
    // 页面绘制（每页一张背景图，吉祥物左右各一版）
    void showMainMenu();
    void showCreatGameMenu();
    void showModeMenu();
    void showGame();
    void showPauseMenu();
    void showGameOver();
    void showSaveMenu();
    void showLeaderboardPage();
    void showSettingMenu();
    void showRules();
    void showMusicMenu();
    void showSizeMenu();
    void showKeyMenu();
    void showExit();

    // 交互
    char handleMouseClick();
    void handleKeyMainMenu();
    void handleKeyCreatGameMenu();
    void handleKeyModeMenu();
    void handleKeyGame();
    void handleKeyPauseMenu();
    void handleKeyGameOver();
    void handleKeySaveMenu();
    void handleKeyLeaderboardPage();
    void handleKeySettingMenu();
    void handleKeyRules();
    void handleKeyMusicMenu();
    void handleKeySizeMenu();
    void handleKeyKeyMenu();

    // 杂项
    void initGame();
    void updateMascot();          // 基于时间的翻转（旧版按循环次数取模，菜单页会疯转）
    void drawPage(int imageIndex);
    void drawPagePair(int rightIndex, int leftIndex);

    // 存档（文件格式与旧版一致）
    void saveGame();
    void loadSave(const std::string& filename);
    void deleteSave(int index);
    std::vector<std::string> getSaveFiles();
    void startNewGame(const std::string& playerName, int gameMode);
    void teardownGame();

    std::unique_ptr<Board> board_;
    std::unique_ptr<Player> player_;
    Leaderboard leaderboard_;
    int currentPage_ = PAGE_MAIN_MENU;
    int key_ = 0;
    std::vector<IMAGE> images_;
    bool playBGM_ = true;
    int mascot_ = MASCOT_RIGHT;
    unsigned long long mascotTick_ = 0;
    unsigned long long mascotLastFlipMs_ = 0;
    bool directKeys_ = true;             // true=方向键，false=WASD
    bool pauseWithP_ = true;             // true=p 键暂停，false=空格暂停
};
