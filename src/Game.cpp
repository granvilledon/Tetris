// Game.cpp —— 页面状态机、菜单交互与存档
#include "Game.h"

#include <algorithm>
#include <conio.h>
#include <filesystem>
#include <fstream>
#include <graphics.h>
#include <mmsystem.h>
#include <sstream>
#include <tuple>
#include <windows.h>

#include "common.h"

Game::Game() : board_(std::make_unique<Board>(0, std::string())), leaderboard_() {}

// ---------------------------------------------------------- 初始化
void Game::initGame() {
    const int width = 16 * gridSize;
    const int height = 20 * gridSize;

    static const char* kPages[] = {
        "PAGE_EXIT",             // 0
        "PAGE_MAIN_MENU1",       // 1
        "PAGE_MAIN_MENU2",       // 2
        "PAGE_CREATGAME_MENU1",  // 3
        "PAGE_CREATGAME_MENU2",  // 4
        "PAGE_MODE_MENU1",       // 5
        "PAGE_MODE_MENU2",       // 6
        "PAGE_GAME1",            // 7
        "PAGE_GAME2",            // 8
        "PAGE_PAUSE_MENU1",      // 9
        "PAGE_PAUSE_MENU2",      // 10
        "PAGE_GAMEOVER1",        // 11
        "PAGE_GAMEOVER2",        // 12
        "PAGE_SAVE_MENU1",       // 13
        "PAGE_SAVE_MENU2",       // 14
        "PAGE_LEADERBOARD1",     // 15
        "PAGE_LEADERBOARD2",     // 16
        "PAGE_SETTINGS_MENU1",   // 17
        "PAGE_SETTINGS_MENU2",   // 18
        "PAGE_RULES1",           // 19
        "PAGE_RULES2",           // 20
        "PAGE_MUSIC_MENU1",      // 21
        "PAGE_MUSIC_MENU2",      // 22
        "PAGE_SIZE_MENU1",       // 23
        "PAGE_SIZE_MENU2",       // 24
        "PAGE_KEY_MENU1",        // 25
        "PAGE_KEY_MENU2",        // 26
    };
    images_.clear();
    for (const char* page : kPages) {
        IMAGE img;
        loadimage(&img, (std::string("./picture/") + page + ".jpg").c_str(), width, height);
        images_.push_back(std::move(img));
    }

    leaderboard_.load();
    leaderboard_.sortByScore();

    initgraph(width, height);
    setbkcolor(WHITE);
    cleardevice();
    settextstyle(gridSize, 0, "微软雅黑");
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);

    srand(static_cast<unsigned>(time(nullptr)));
}

// ---------------------------------------------------------- 通用绘制
void Game::drawPagePair(int rightIndex, int leftIndex) {
    BeginBatchDraw();
    cleardevice();
    putimage(0, 0, &images_[mascot_ == MASCOT_RIGHT ? rightIndex : leftIndex]);
    EndBatchDraw();
}

void Game::showMainMenu() { drawPagePair(1, 2); }
void Game::showCreatGameMenu() { drawPagePair(3, 4); }
void Game::showModeMenu() { drawPagePair(5, 6); }
void Game::showPauseMenu() { drawPagePair(9, 10); }
void Game::showGameOver() { drawPagePair(11, 12); }
void Game::showSettingMenu() { drawPagePair(17, 18); }
void Game::showRules() { drawPagePair(19, 20); }
void Game::showMusicMenu() { drawPagePair(21, 22); }
void Game::showSizeMenu() { drawPagePair(23, 24); }
void Game::showKeyMenu() { drawPagePair(25, 26); }

void Game::showGame() {
    if (!board_->current()) board_->spawnNext();

    while (currentPage_ == PAGE_GAME) {
        updateMascot();
        handleKeyGame();

        BeginBatchDraw();
        cleardevice();
        putimage(0, 0, &images_[mascot_ == MASCOT_RIGHT ? 7 : 8]);
        board_->draw();
        EndBatchDraw();

        board_->stepDown();

        // 有输入时帧率拉高让操作跟手，否则按难度节奏下落
        Sleep(_kbhit() ? 60 : player_->getGameMode());

        if (board_->gameOver()) currentPage_ = PAGE_GAMEOVER;
    }
}

void Game::showSaveMenu() {
    drawPagePair(13, 14);

    std::vector<std::string> saves = getSaveFiles();
    int y = 7 * gridSize;
    int index = 1;
    for (const std::string& f : saves) {
        std::ifstream file("save/" + f);
        if (!file.is_open()) continue;
        int year, month, day, hour, minute, second, score, mode;
        std::string name;
        file >> year >> month >> day >> hour >> minute >> second >> name >> score >> mode;
        file.close();

        const char* modeText = mode == MODE_EASY  ? "简单"
                               : mode == MODE_NORMAL ? "一般"
                                                     : "困难";
        std::ostringstream ss;
        ss << index++ << ". " << name << ' ' << score << ' ' << modeText << ' '
           << year << '-' << month << '-' << day << ' '
           << hour << ':' << minute << ':' << second;
        outtextxy(2 * gridSize, y, ss.str().c_str());
        y += gridSize;
    }
}

void Game::showLeaderboardPage() {
    drawPagePair(15, 16);
    outtextxy(0, 4 * gridSize, "简单模式");
    leaderboard_.show(MODE_EASY, 0, 5 * gridSize);
    outtextxy(12 * gridSize, 4 * gridSize, "一般模式");
    leaderboard_.show(MODE_NORMAL, 11 * gridSize, 5 * gridSize);
    outtextxy(6 * gridSize, 4 * gridSize, "困难模式");
    leaderboard_.show(MODE_HARD, 5 * gridSize + halfgridSize, 5 * gridSize);
}

void Game::showExit() {
    drawPagePair(0, 0);
    Sleep(500);
    closegraph();
    exit(0);
}

// ---------------------------------------------------------- 鼠标
// 命中区域与旧版完全一致（基于 640x816 逻辑坐标的按钮布局）
char Game::handleMouseClick() {
    while (MouseHit()) {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.uMsg != WM_LBUTTONDOWN) continue;
        int x = msg.x;
        int y = msg.y;
        if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize) {
            if (y > 5 * gridSize && y < 7 * gridSize) return '1';
            if (y > 7 * gridSize + halfgridSize && y < 9 * gridSize + halfgridSize) return '2';
            if (y > 10 * gridSize && y < 12 * gridSize) return '3';
            if (y > 12 * gridSize + halfgridSize && y < 14 * gridSize + halfgridSize) return '4';
            if (y > 15 * gridSize && y < 17 * gridSize) return '5';
            if (y > 17 * gridSize + halfgridSize && y < 19 * gridSize + halfgridSize) return 'd';
        }
        if (x > 14 * gridSize + halfgridSize && x < 16 * gridSize + halfgridSize &&
            y > 0 && y < gridSize) {
            return KEY_ESC;
        }
    }
    return 0;
}

namespace {
// 从键盘缓冲取一键：MouseHit 优先，其次 _getch
int readKey() {
    return _kbhit() ? _getch() : 0;
}
}  // namespace

// ---------------------------------------------------------- 各页按键
void Game::handleKeyMainMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1': currentPage_ = PAGE_CREATGAME_MENU; break;
        case '2': currentPage_ = PAGE_SAVE_MENU; break;
        case '3': currentPage_ = PAGE_LEADERBOARD; break;
        case '4': currentPage_ = PAGE_RULES; break;
        case '5': currentPage_ = PAGE_SETTINGS_MENU; break;
        case KEY_ESC: currentPage_ = PAGE_EXIT; break;
        default: break;
    }
}

void Game::handleKeyCreatGameMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '2': {
            char input[20] = "";
            InputBox(input, 20, "请输入新玩家名称");
            std::string playerName = input;
            startNewGame(playerName, MODE_EASY);
            currentPage_ = PAGE_MODE_MENU;
            break;
        }
        case KEY_ESC: currentPage_ = PAGE_MAIN_MENU; break;
        default: break;
    }
}

void Game::handleKeyModeMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    if (!player_) return;
    switch (key_) {
        case '1': player_->setGameMode(MODE_EASY); currentPage_ = PAGE_GAME; break;
        case '2': player_->setGameMode(MODE_NORMAL); currentPage_ = PAGE_GAME; break;
        case '3': player_->setGameMode(MODE_HARD); currentPage_ = PAGE_GAME; break;
        case KEY_ESC: currentPage_ = PAGE_MAIN_MENU; break;
        default: break;
    }
}

void Game::handleKeyGame() {
    int k = readKey();
    if (!k) return;
    key_ = k;
    const bool arrows = directKeys_;
    switch (key_) {
        case 224:   // 方向键的第一段前缀（_getch 会先返回它）
        case 0:
            break;
        default:
            break;
    }

    // 方向键 / WASD 双键位
    if (arrows) {
        switch (key_) {
            case KEY_LEFT: board_->tryMove(-1, 0); break;
            case KEY_RIGHT: board_->tryMove(1, 0); break;
            case KEY_DOWN: board_->tryMove(0, 1); break;
            case KEY_UP: board_->tryRotate(true); break;
            default: break;
        }
    } else {
        switch (key_) {
            case KEY_A: board_->tryMove(-1, 0); break;
            case KEY_D: board_->tryMove(1, 0); break;
            case KEY_S: board_->tryMove(0, 1); break;
            case KEY_W: board_->tryRotate(true); break;
            default: break;
        }
    }

    // 暂停键双方案：p 或空格
    if ((pauseWithP_ && key_ == 'p') || (!pauseWithP_ && key_ == ' ')) {
        currentPage_ = PAGE_PAUSE_MENU;
    }
}

void Game::handleKeyPauseMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1': currentPage_ = PAGE_GAME; break;
        case '2': {
            std::vector<std::string> saves = getSaveFiles();
            if (saves.size() >= 9) {
                outtextxy(6 * gridSize, 4 * gridSize, "存档已满");
                Sleep(1000);
                return;
            }
            saveGame();
            currentPage_ = PAGE_SAVE_MENU;
            break;
        }
        case '3': currentPage_ = PAGE_GAMEOVER; break;
        default: break;
    }
}

void Game::handleKeyGameOver() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    if (key_ != '2') return;
    currentPage_ = PAGE_MAIN_MENU;
    if (player_ && board_) {
        player_->setScore(board_->getScore());
        leaderboard_.add(*player_);
        leaderboard_.sortByScore();
        leaderboard_.save();
    }
    teardownGame();
}

void Game::handleKeySaveMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    std::vector<std::string> saves = getSaveFiles();
    switch (key_) {
        case KEY_ESC:
            currentPage_ = PAGE_MAIN_MENU;
            break;
        case 'd': {
            char input[20] = "";
            InputBox(input, 20, "请输入要删除的存档编号");
            int index = atoi(input) - 1;
            if (index >= 0 && index < (int)saves.size()) {
                deleteSave(index);
                outtextxy(2 * gridSize, 6 * gridSize, "删除成功");
            } else {
                outtextxy(2 * gridSize, 6 * gridSize, "无该存档");
            }
            Sleep(1000);
            break;
        }
        default:
            if (key_ >= '0' && key_ <= '9') {
                int index = key_ - '0' - 1;
                if (index >= 0 && index < (int)saves.size()) {
                    loadSave(saves[index]);
                    currentPage_ = PAGE_GAME;
                } else {
                    outtextxy(2 * gridSize, 6 * gridSize, "无该存档");
                    Sleep(1000);
                }
            }
            break;
    }
}

void Game::handleKeyLeaderboardPage() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    if (key_ == KEY_ESC) currentPage_ = PAGE_MAIN_MENU;
}

void Game::handleKeySettingMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1': currentPage_ = PAGE_MUSIC_MENU; break;
        case '2': currentPage_ = PAGE_SIZE_MENU; break;
        case '3': currentPage_ = PAGE_KEY_MENU; break;
        case KEY_ESC: currentPage_ = PAGE_MAIN_MENU; break;
        default: break;
    }
}

void Game::handleKeyRules() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    if (key_ == KEY_ESC) currentPage_ = PAGE_MAIN_MENU;
}

void Game::handleKeyMusicMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1':
            playBGM_ = !playBGM_;
            mciSendString(playBGM_ ? "play bgm repeat" : "stop bgm", nullptr, 0, nullptr);
            outtextxy(6 * gridSize, 4 * gridSize, "修改成功");
            Sleep(1000);
            break;
        case '2': case '3': case '4': {
            char cmd[128];
            sprintf(cmd, "open music/music%c.mp3 alias bgm", key_);
            mciSendString("close bgm", nullptr, 0, nullptr);
            mciSendString(cmd, nullptr, 0, nullptr);
            if (playBGM_) mciSendString("play bgm repeat", nullptr, 0, nullptr);
            outtextxy(6 * gridSize, 4 * gridSize, "修改成功");
            Sleep(1000);
            break;
        }
        case KEY_ESC: currentPage_ = PAGE_SETTINGS_MENU; break;
        default: break;
    }
}

void Game::handleKeySizeMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1': gridSize = SMALL_SIZE; halfgridSize = gridSize / 2; initGame(); break;
        case '2': gridSize = MEDIUM_SIZE; halfgridSize = gridSize / 2; initGame(); break;
        case '3': gridSize = LARGE_SIZE; halfgridSize = gridSize / 2; initGame(); break;
        case KEY_ESC: currentPage_ = PAGE_SETTINGS_MENU; break;
        default: break;
    }
}

void Game::handleKeyKeyMenu() {
    if (MouseHit()) key_ = handleMouseClick();
    if (int k = readKey()) key_ = k;
    switch (key_) {
        case '1':
            directKeys_ = !directKeys_;
            outtextxy(6 * gridSize, 4 * gridSize, "修改成功");
            Sleep(1000);
            break;
        case '2':
            pauseWithP_ = !pauseWithP_;
            outtextxy(6 * gridSize, 4 * gridSize, "修改成功");
            Sleep(1000);
            break;
        case KEY_ESC: currentPage_ = PAGE_SETTINGS_MENU; break;
        default: break;
    }
}

// ---------------------------------------------------------- 吉祥物
void Game::updateMascot() {
    // 旧版按循环次数取模，菜单页全速空转时吉祥物每秒翻转几十万次，
    // 肉眼看到的是"随机跳"。改成按真实时间每 600ms 翻转一次。
    unsigned long long now = GetTickCount64();
    if (now - mascotLastFlipMs_ >= 600) {
        mascotLastFlipMs_ = now;
        mascot_ = -mascot_;
    }
    (void)mascotTick_;
}

// ---------------------------------------------------------- 存档
void Game::startNewGame(const std::string& playerName, int gameMode) {
    player_ = std::make_unique<Player>(playerName, 0, gameMode);
    board_ = std::make_unique<Board>(0, playerName);
}

void Game::teardownGame() {
    board_ = std::make_unique<Board>(0, std::string());
    player_.reset();
}

void Game::saveGame() {
    std::filesystem::create_directories("save");   // 旧版目录不存在时直接写失败
    std::ofstream file("save/" + player_->getName() + ".txt");
    if (!file.is_open()) return;

    time_t t = time(nullptr);
    tm* now = localtime(&t);
    file << now->tm_year + 1900 << ' ' << now->tm_mon + 1 << ' ' << now->tm_mday << ' '
         << now->tm_hour << ' ' << now->tm_min << ' ' << now->tm_sec << '\n';
    file << player_->getName() << '\n' << board_->getScore() << '\n'
         << player_->getGameMode() << '\n';
    for (int r = 0; r < Board::HEIGHT; ++r) {
        for (int c = 0; c < Board::WIDTH; ++c) file << board_->at(r, c) << ' ';
        file << '\n';
    }

    auto writeBlock = [&](const Block* b) {
        if (!b) { file << -1 << '\n'; return; }
        file << b->shape << ' ' << b->color << ' ' << b->x << ' ' << b->y << ' '
             << b->state << '\n';
    };
    writeBlock(board_->current());
    writeBlock(board_->next());
}

void Game::loadSave(const std::string& filename) {
    std::ifstream file("save/" + filename);
    if (!file.is_open()) return;

    int year, month, day, hour, minute, second, score, mode;
    std::string name;
    file >> year >> month >> day >> hour >> minute >> second >> name >> score >> mode;

    player_ = std::make_unique<Player>(name, score, mode);
    board_ = std::make_unique<Board>(score, name);

    for (int r = 0; r < Board::HEIGHT; ++r)
        for (int c = 0; c < Board::WIDTH; ++c) {
            int v = 0;
            file >> v;
            board_->setAt(r, c, v);
        }

    auto readBlock = [&](bool isCurrent) {
        int shape = -1;
        file >> shape;
        if (shape == -1) return;
        int color = 0, x = 0, y = 0, state = 0;
        file >> color >> x >> y >> state;
        auto block = makeBlock({shape, color, x, y, state});
        if (isCurrent)
            board_->setCurrent(std::move(block));
        else
            board_->setNext(std::move(block));
    };
    readBlock(true);
    readBlock(false);
}

void Game::deleteSave(int index) {
    std::vector<std::string> saves = getSaveFiles();
    if (index < 0 || index >= (int)saves.size()) return;
    std::error_code ec;
    std::filesystem::remove("save/" + saves[index], ec);
}

std::vector<std::string> Game::getSaveFiles() {
    std::vector<std::string> files;
    std::filesystem::create_directories("save");
    for (const auto& entry : std::filesystem::directory_iterator("save")) {
        if (entry.path().extension() == ".txt")
            files.push_back(entry.path().filename().string());
    }

    // 按存档时间从新到旧排序（格式与旧版一致）
    std::vector<std::tuple<std::string, int, std::string>> infos;
    for (const std::string& f : files) {
        std::ifstream file("save/" + f);
        if (!file.is_open()) continue;
        int year, month, day, hour, minute, second, score;
        file >> year >> month >> day >> hour >> minute >> second >> score;
        std::ostringstream ss;
        ss << year << '-' << month << '-' << day << ' '
           << hour << ':' << minute << ':' << second;
        infos.emplace_back(f, score, ss.str());
    }
    std::sort(infos.begin(), infos.end(), [](const auto& a, const auto& b) {
        return std::get<2>(a) > std::get<2>(b);
    });

    std::vector<std::string> result;
    for (const auto& info : infos) result.push_back(std::get<0>(info));
    return result;
}

// ---------------------------------------------------------- 主循环
void Game::run() {
    initGame();
    mciSendString("open music/music1.mp3 alias bgm", nullptr, 0, nullptr);
    mciSendString("play bgm repeat", nullptr, 0, nullptr);

    while (true) {
        updateMascot();
        key_ = 0;
        switch (currentPage_) {
            case PAGE_MAIN_MENU:      showMainMenu(); handleKeyMainMenu(); break;
            case PAGE_CREATGAME_MENU: showCreatGameMenu(); handleKeyCreatGameMenu(); break;
            case PAGE_MODE_MENU:      showModeMenu(); handleKeyModeMenu(); break;
            case PAGE_GAME:           showGame(); break;
            case PAGE_PAUSE_MENU:     showPauseMenu(); handleKeyPauseMenu(); break;
            case PAGE_GAMEOVER:       showGameOver(); handleKeyGameOver(); break;
            case PAGE_SAVE_MENU:      showSaveMenu(); handleKeySaveMenu(); break;
            case PAGE_LEADERBOARD:    showLeaderboardPage(); handleKeyLeaderboardPage(); break;
            case PAGE_SETTINGS_MENU:  showSettingMenu(); handleKeySettingMenu(); break;
            case PAGE_RULES:          showRules(); handleKeyRules(); break;
            case PAGE_MUSIC_MENU:     showMusicMenu(); handleKeyMusicMenu(); break;
            case PAGE_SIZE_MENU:      showSizeMenu(); handleKeySizeMenu(); break;
            case PAGE_KEY_MENU:       showKeyMenu(); handleKeyKeyMenu(); break;
            case PAGE_EXIT:           showExit(); break;
            default: break;
        }
        // 菜单页限频：旧版这里全速空转，一个核直接跑满
        if (currentPage_ != PAGE_GAME) Sleep(30);
    }
}
