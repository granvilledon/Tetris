#include <iostream>
#include <graphics.h>
#include <conio.h>
#include <cstring>
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <windows.h>

using namespace std;

// ������
const int SMALL_SIZE = 20;
const int MEDIUM_SIZE = 30;
const int LARGE_SIZE = 40;
int gridSize = MEDIUM_SIZE;
int halfgridSize = gridSize / 2;

// ������״
const int SHAPE_O = 0;
const int SHAPE_I = 1;
const int SHAPE_L = 2;
const int SHAPE_J = 3;
const int SHAPE_S = 4;
const int SHAPE_Z = 5;
const int SHAPE_T = 6;

// �Զ�����ɫ
const int COLOR_BOARD = RGB(209, 150, 34);
const int COLOR_BLOCK = RGB(255, 219, 149);

// ���̰���
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_W = 119;
const int KEY_S = 115;
const int KEY_A = 97;
const int KEY_D = 100;
const int KEY_ESC = 27;

// ҳ�泣��
const int PAGE_EXIT = 0;
const int PAGE_MAIN_MENU = 1;
const int PAGE_CREATGAME_MENU = 2;
const int PAGE_MODE_MENU = 3;
const int PAGE_GAME = 4;
const int PAGE_PAUSE_MENU = 5;
const int PAGE_GAMEOVER = 6;
const int PAGE_SAVE_MENU = 7;
const int PAGE_LEADERBOARD = 8;
const int PAGE_SETTINGS_MENU = 9;
const int PAGE_RULES = 10;
const int PAGE_MUSIC_MENU = 11;
const int PAGE_SIZE_MENU = 12;
const int PAGE_KEY_MENU = 13;

// ��Ϸģʽ�����������ٶȣ�
const int MODE_EASY = 500;
const int MODE_NORMAL = 300;
const int MODE_HARD = 150;

// �������λ��(1��ʾ�ұߣ�-1��ʾ���)
const int MASCOT_RIGHT = 1;
const int MASCOT_LEFT = -1;

class Block {
protected:
    int x;
    int y;
    int shape;
    int color;
    int state;

public:
    Block(int _x, int _y, int _shape, int _color, int _state) : x(_x), y(_y), shape(_shape), color(_color),
                                                                state(_state) {
    }

    virtual ~Block() {
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

    int getShape() {
        return shape;
    }

    int getColor() {
        return color;
    }

    int getState() {
        return state;
    }

    virtual void moveLeft() {
        x--;
    }

    virtual void moveRight() {
        x++;
    }

    virtual void moveDown() {
        y++;
    }

    virtual void moveUp() {
        y--;
    }

    virtual void rotate() = 0;

    virtual void rerotate() = 0;

    virtual void draw() = 0;

    virtual void drawPreview() = 0;
};

class OBlock : public Block {
public:
    OBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_O, _color, 0) {
    }

    virtual void rotate() {
        // �����η��鲻��Ҫ��ת
    }

    virtual void rerotate() {
        // �����η��鲻��Ҫ��ת
    }

    virtual int getState() {
        // �����η�������ת״̬
        return 0;
    }

    virtual void draw() {
        // ���㷽�������
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + 2 * gridSize);
    }

    virtual void drawPreview() {
        // ���㷽�������
        int x1 = 12 * gridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + 2 * gridSize);
    }
};

class IBlock : public Block {
public:
    IBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_I, _color, _state) {
    }

    virtual void rotate() {
        state = (state + 1) % 2;
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 1;
        }
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����I�η���
            solidrectangle(x1, y1, x1 + 4 * gridSize, y1 + gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��I�η���
            solidrectangle(x1 + gridSize, y1 - gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����I�η���
            solidrectangle(x1, y1, x1 + 4 * gridSize, y1 + gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��I�η���
            solidrectangle(x1 + gridSize, y1 - gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }
};

class LBlock : public Block {
public:
    LBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_L, _color, _state) {
    }

    virtual void rotate() {
        state = (state + 1) % 4;
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 3;
        }
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����L�η���
            solidrectangle(x1 + 2 * gridSize, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 2 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + 3 * gridSize);
            solidrectangle(x1 + gridSize, y1 + 2 * gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize + halfgridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����L�η���
            solidrectangle(x1 + 2 * gridSize, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 2 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��L�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + 3 * gridSize);
            solidrectangle(x1 + gridSize, y1 + 2 * gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }
};

class JBlock : public Block {
public:
    JBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_J, _color, _state) {
    }

    virtual void rotate() {
        state = (state + 1) % 4;
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 3;
        }
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����J�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��J�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
            solidrectangle(x1, y1 + 2 * gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��J�η���
            solidrectangle(x1, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1 + 2 * gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��J�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 3 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize + halfgridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����J�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��J�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
            solidrectangle(x1, y1 + 2 * gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��J�η���
            solidrectangle(x1, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1 + 2 * gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��J�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 3 * gridSize);
        }
    }
};

class SBlock : public Block {
public:
    SBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_S, _color, _state) {
    }

    virtual void rotate() {
        state = (state + 1) % 2;
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 1;
        }
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����S�η���
            solidrectangle(x1 + gridSize, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 2 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��S�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize + halfgridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����S�η���
            solidrectangle(x1 + gridSize, y1, x1 + 3 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 2 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��S�η���
            solidrectangle(x1, y1, x1 + gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        }
    }
};

class ZBlock : public Block {
public:
    ZBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_Z, _color, _state) {
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 1;
        }
    }

    virtual void rotate() {
        state = (state + 1) % 2;
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����Z�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��Z�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 2 * gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 3 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize + halfgridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����Z�η���
            solidrectangle(x1, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1 + gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��Z�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 2 * gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 3 * gridSize);
        }
    }
};

class TBlock : public Block {
public:
    TBlock(int _x, int _y, int _color, int _state = 0) : Block(_x, _y, SHAPE_T, _color, _state) {
    }

    virtual void rotate() {
        state = (state + 1) % 4;
    }

    virtual void rerotate() {
        state = state - 1;
        if (state < 0) {
            state = 3;
        }
    }

    virtual void draw() {
        // ���㷽��������ֵ�����
        int x1 = x * gridSize;
        int y1 = y * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����T�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��T�η���
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��T�η���
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1 + 2 * gridSize, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��T�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
            solidrectangle(x1 + 2 * gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        }
    }

    virtual void drawPreview() {
        // ���㷽��������ֵ�����
        int x1 = 11 * gridSize + halfgridSize;
        int y1 = 3 * gridSize;

        // ���������ɫ
        setfillcolor(color);

        if (state == 0) {
            // ����T�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + gridSize);
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        } else if (state == 1) {
            // ������ת90�Ⱥ��T�η���
            solidrectangle(x1, y1 + gridSize, x1 + gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 2) {
            // ������ת180�Ⱥ��T�η���
            solidrectangle(x1, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
            solidrectangle(x1 + gridSize, y1 + 2 * gridSize, x + 2 * gridSize, y1 + 3 * gridSize);
        } else if (state == 3) {
            // ������ת270�Ⱥ��T�η���
            solidrectangle(x1 + gridSize, y1, x1 + 2 * gridSize, y1 + 3 * gridSize);
            solidrectangle(x1 + 2 * gridSize, y1 + gridSize, x1 + 3 * gridSize, y1 + 2 * gridSize);
        }
    }
};

class Board {
private:
    int width;
    int height;
    int **board;
    int score;
    string playerName;
    Block *currentBlock;
    Block *nextBlock;

public:
    Board(int _score, string _playerName) : width(10), height(20), board(nullptr), score(_score),
                                            playerName(_playerName), currentBlock(nullptr), nextBlock(nullptr) {
        // ��ʼ������
        board = new int *[height];
        for (int i = 0; i < height; i++) {
            board[i] = new int[width];
            for (int j = 0; j < width; j++) {
                board[i][j] = 0;
            }
        }
    }

    ~Board() {
        // �ͷ��ڴ�
        for (int i = 0; i < height; i++) {
            delete[] board[i];
        }
        delete[] board;
        delete currentBlock;
        delete nextBlock;
    }

    int getHeight() {
        return height;
    }

    int getWidth() {
        return width;
    }

    int get(int x, int y) {
        return board[x][y];
    }

    int getScore() {
        return score;
    }

    Block *getCurrentBlock() {
        return currentBlock;
    }

    Block *getNextBlock() {
        return nextBlock;
    }

    string getPlayername() {
        return playerName;
    }

    void set(int x, int y, int value) {
        board[x][y] = value;
    }

    void setCurrentBlock(Block *block) {
        currentBlock = block;
    }

    void setNextBlock(Block *block) {
        nextBlock = block;
    }

    void draw() {
        // ��������
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                // ������ӵ�����
                int x1 = j * gridSize;
                int y1 = i * gridSize;
                int x2 = (j + 1) * gridSize;
                int y2 = (i + 1) * gridSize;

                // ���������ɫ��������ɫ
                if (board[i][j] == 1) {
                    setfillcolor(COLOR_BOARD);
                } else {
                    setfillcolor(COLOR_BLOCK);
                }
                setlinecolor(BLACK);

                // ���Ƹ���
                fillrectangle(x1, y1, x2, y2);
            }
        }

        // ���Ƶ�ǰ����
        currentBlock->draw();

        // ����Ԥ�洰��
        setlinecolor(BLACK);
        rectangle(11 * gridSize, 2 * gridSize, 15 * gridSize, 6 * gridSize);
        outtextxy(11 * gridSize, gridSize, _T("Next Block"));

        // ������һ������
        if (nextBlock != nullptr) {
            nextBlock->drawPreview();
        }
        // ���Ʒ���
        string _score = "Score: " + to_string(score);
        outtextxy(11 * gridSize, 6 * gridSize + halfgridSize, _T(_score.c_str()));
        // �����������
        string _playername = "Player: " + playerName;
        outtextxy(11 * gridSize, 8 * gridSize, _T(_playername.c_str()));
    }

    void generateBlock() {
        // ��� nextBlock Ϊ�գ�������һ���µķ���
        if (nextBlock == nullptr) {
            switch (rand() % 7) {
                case SHAPE_O:
                    nextBlock = new OBlock(width / 2 - 1, 0, BLUE);
                    break;
                case SHAPE_I:
                    nextBlock = new IBlock(width / 2 - 1, 0, GREEN);
                    break;
                case SHAPE_L:
                    nextBlock = new LBlock(width / 2 - 1, 0, RED);
                    break;
                case SHAPE_J:
                    nextBlock = new JBlock(width / 2 - 1, 0, MAGENTA);
                    break;
                case SHAPE_S:
                    nextBlock = new SBlock(width / 2 - 1, 0, DARKGRAY);
                    break;
                case SHAPE_Z:
                    nextBlock = new ZBlock(width / 2 - 1, 0, LIGHTCYAN);
                    break;
                case SHAPE_T:
                    nextBlock = new TBlock(width / 2 - 1, 0, CYAN);
                    break;
            }
        }

        currentBlock = nextBlock; // �� currentBlock ����Ϊ nextBlock
        // �ٴ�����һ���µķ��鸳ֵ�� nextBlock
        switch (rand() % 7) {
            case SHAPE_O:
                nextBlock = new OBlock(width / 2 - 1, 0, BLUE);
                break;
            case SHAPE_I:
                nextBlock = new IBlock(width / 2 - 1, 0, GREEN);
                break;
            case SHAPE_L:
                nextBlock = new LBlock(width / 2 - 1, 0, RED);
                break;
            case SHAPE_J:
                nextBlock = new JBlock(width / 2 - 1, 0, MAGENTA);
                break;
            case SHAPE_S:
                nextBlock = new SBlock(width / 2 - 1, 0, DARKGRAY);
                break;
            case SHAPE_Z:
                nextBlock = new ZBlock(width / 2 - 1, 0, LIGHTCYAN);
                break;
            case SHAPE_T:
                nextBlock = new TBlock(width / 2 - 1, 0, CYAN);
                break;
        }
    }

    bool isCollideWithLeft() {
        // ��ȡ��ǰ�����λ�ú���״
        int x = currentBlock->getX();
        int shape = currentBlock->getShape();
        int state = 0;

        // ���ݷ�����״�ж��Ƿ������߽���ײ
        switch (shape) {
            case SHAPE_O:
                if (x < 0) {
                    return true;
                }
                break;
            case SHAPE_I:
                // ���I�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 1 < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_L:
                // ���L�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_J:
                // ���J�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_S:
                // ���S�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_Z:
                // ���Z�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_T:
                // ���T�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x + 1 < 0) {
                        return true;
                    }
                }
                break;
            default:
                break;
        }

        return false;
    }

    bool isCollideWithRight() {
        // ��ȡ��ǰ�����λ�ú���״
        int x = currentBlock->getX();
        int shape = currentBlock->getShape();
        int state = 0;

        // ���ݷ�����״�ж��Ƿ����Ҳ�߽���ײ
        switch (shape) {
            case SHAPE_O:
                if (x + 2 > width) {
                    return true;
                }
                break;
            case SHAPE_I:
                // ���I�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 4 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                }
                break;
            case SHAPE_L:
                // ���L�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x + 2 > width) {
                        return true;
                    }
                }
                break;
            case SHAPE_J:
                // ���J�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x + 2 > width) {
                        return true;
                    }
                }
                break;
            case SHAPE_S:
                // ���S�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                }
                break;
            case SHAPE_Z:
                // ���Z�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                }
                break;
            case SHAPE_T:
                // ���T�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 1) {
                    if (x + 2 > width) {
                        return true;
                    }
                } else if (state == 2) {
                    if (x + 3 > width) {
                        return true;
                    }
                } else if (state == 3) {
                    if (x + 3 > width) {
                        return true;
                    }
                }
                break;
            default:
                break;
        }

        return false;
    }

    bool isCollideWithTop() {
        // ��ȡ��ǰ�����λ�ú���״
        int y = currentBlock->getY();
        int shape = currentBlock->getShape();
        int state = 0;

        // ���ݷ�����״�ж��Ƿ���ײ���ײ
        switch (shape) {
            case SHAPE_O:
                if (y < 0) {
                    return true;
                }
                break;
            case SHAPE_I:
                // ���I�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y - 1 < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_L:
                // ���L�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_J:
                // ���J�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_S:
                // ���S�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_Z:
                // ���Z�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y < 0) {
                        return true;
                    }
                }
                break;
            case SHAPE_T:
                // ���T�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y < 0) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y + 1 < 0) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y < 0) {
                        return true;
                    }
                }
                break;
            default:
                break;
        }

        return false;
    }

    bool isCollideWithBottom() {
        // ��ȡ��ǰ�����λ�ú���״
        int y = currentBlock->getY();
        int shape = currentBlock->getShape();
        int state = 0;

        // ���ݷ�����״�ж��Ƿ���ײ���ײ
        switch (shape) {
            case SHAPE_O:
                if (y + 2 > height) {
                    return true;
                }
                break;
            case SHAPE_I:
                // ���I�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 1 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            case SHAPE_L:
                // ���L�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            case SHAPE_J:
                // ���J�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            case SHAPE_S:
                // ���S�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            case SHAPE_Z:
                // ���Z�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            case SHAPE_T:
                // ���T�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (y + 2 > height) {
                        return true;
                    }
                } else if (state == 1) {
                    if (y + 3 > height) {
                        return true;
                    }
                } else if (state == 2) {
                    if (y + 3 > height) {
                        return true;
                    }
                } else if (state == 3) {
                    if (y + 3 > height) {
                        return true;
                    }
                }
                break;
            default:
                break;
        }

        return false;
    }

    bool isCollideWithBlock() {
        // ��ȡ��ǰ�����λ�ú���״
        int x = currentBlock->getX();
        int y = currentBlock->getY();
        int shape = currentBlock->getShape();
        int state = 0;

        // ���ݷ�����״�ж��Ƿ�������������ײ
        switch (shape) {
            case SHAPE_O:
                if (board[y][x] || board[y][x + 1] || board[y + 1][x] || board[y + 1][x + 1]) {
                    return true;
                }
                break;
            case SHAPE_I:
                // ���I�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y][x] || board[y][x + 1] || board[y][x + 2] || board[y][x + 3]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[max(y - 1, 0)][x + 1] || board[y][x + 1] || board[y + 1][x + 1] || board[y + 2][x + 1]) {
                        return true;
                    }
                }
                break;
            case SHAPE_L:
                // ���L�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y + 1][x] || board[y + 1][x + 1] || board[y + 1][x + 2] || board[y][x + 2]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[y][x] || board[y][x + 1] || board[y + 1][x + 1] || board[y + 2][x + 1]) {
                        return true;
                    }
                } else if (state == 2) {
                    if (board[y][x] || board[y][x + 1] || board[y][x + 2] || board[y + 1][x]) {
                        return true;
                    }
                } else if (state == 3) {
                    if (board[y][x] || board[y + 1][x] || board[y + 2][x] || board[y + 2][x + 1]) {
                        return true;
                    }
                }
                break;
            case SHAPE_J:
                // ���J�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y][x] || board[y + 1][x] || board[y + 1][x + 1] || board[y + 1][x + 2]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[y][x + 1] || board[y + 1][x + 1] || board[y + 2][x + 1] || board[y + 2][x]) {
                        return true;
                    }
                } else if (state == 2) {
                    if (board[y][x] || board[y][x + 1] || board[y][x + 2] || board[y + 1][x + 2]) {
                        return true;
                    }
                } else if (state == 3) {
                    if (board[y][x] || board[y][x + 1] || board[y + 1][x] || board[y + 2][x]) {
                        return true;
                    }
                }
                break;
            case SHAPE_S:
                // ���S�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y][x + 1] || board[y][x + 2] || board[y + 1][x] || board[y + 1][x + 1]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[y][x] || board[y + 1][x] || board[y + 1][x + 1] || board[y + 2][x + 1]) {
                        return true;
                    }
                }
                break;
            case SHAPE_Z:
                // ���Z�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y][x] || board[y][x + 1] || board[y + 1][x + 1] || board[y + 1][x + 2]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[y][x + 1] || board[y + 1][x] || board[y + 1][x + 1] || board[y + 2][x]) {
                        return true;
                    }
                }
                break;
            case SHAPE_T:
                // ���T�η�����ת���״̬
                state = currentBlock->getState();
                if (state == 0) {
                    if (board[y][x + 1] || board[y + 1][x] || board[y + 1][x + 1] || board[y + 1][x + 2]) {
                        return true;
                    }
                } else if (state == 1) {
                    if (board[y][x + 1] || board[y + 1][x] || board[y + 1][x + 1] || board[y + 2][x + 1]) {
                        return true;
                    }
                } else if (state == 2) {
                    if (board[y + 1][x] || board[y + 1][x + 1] || board[y + 1][x + 2] || board[y + 2][x + 1]) {
                        return true;
                    }
                } else if (state == 3) {
                    if (board[y][x + 1] || board[y + 1][x + 1] || board[y + 2][x + 1] || board[y + 1][x + 2]) {
                        return true;
                    }
                }
                break;
            default:
                break;
        }

        return false;
    }

    void fixBlock() {
        // ��ȡ��ǰ�����λ�ú���״
        int x = currentBlock->getX();
        int y = currentBlock->getY();
        int shape = currentBlock->getShape();
        int state = currentBlock->getState();

        // ���ݷ�����״������������
        switch (shape) {
            case SHAPE_O:
                board[y][x] = 1;
                board[y][x + 1] = 1;
                board[y + 1][x] = 1;
                board[y + 1][x + 1] = 1;
                break;
            case SHAPE_I:
                if (state == 0) {
                    board[y][x] = 1;
                    board[y][x + 1] = 1;
                    board[y][x + 2] = 1;
                    board[y][x + 3] = 1;
                } else if (state == 1) {
                    board[y - 1][x + 1] = 1;
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                }
                break;
            case SHAPE_L:
                if (state == 0) {
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                    board[y][x + 2] = 1;
                } else if (state == 1) {
                    board[y][x] = 1;
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                } else if (state == 2) {
                    board[y][x] = 1;
                    board[y][x + 1] = 1;
                    board[y][x + 2] = 1;
                    board[y + 1][x] = 1;
                } else if (state == 3) {
                    board[y][x] = 1;
                    board[y + 1][x] = 1;
                    board[y + 2][x] = 1;
                    board[y + 2][x + 1] = 1;
                }
                break;
            case SHAPE_J:
                if (state == 0) {
                    board[y][x] = 1;
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                } else if (state == 1) {
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                    board[y + 2][x] = 1;
                } else if (state == 2) {
                    board[y][x] = 1;
                    board[y][x + 1] = 1;
                    board[y][x + 2] = 1;
                    board[y + 1][x + 2] = 1;
                } else if (state == 3) {
                    board[y][x] = 1;
                    board[y + 1][x] = 1;
                    board[y + 2][x] = 1;
                    board[y][x + 1] = 1;
                }
                break;
            case SHAPE_S:
                if (state == 0) {
                    board[y][x + 1] = 1;
                    board[y][x + 2] = 1;
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                } else if (state == 1) {
                    board[y][x] = 1;
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                }
                break;
            case SHAPE_Z:
                if (state == 0) {
                    board[y][x] = 1;
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                } else if (state == 1) {
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x] = 1;
                    board[y + 2][x] = 1;
                }
                break;
            case SHAPE_T:
                if (state == 0) {
                    board[y][x + 1] = 1;
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                } else if (state == 1) {
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                    board[y + 1][x] = 1;
                } else if (state == 2) {
                    board[y + 1][x] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                    board[y + 2][x + 1] = 1;
                } else if (state == 3) {
                    board[y][x + 1] = 1;
                    board[y + 1][x + 1] = 1;
                    board[y + 2][x + 1] = 1;
                    board[y + 1][x + 2] = 1;
                }
                break;
        }

        // ɾ����ǰ����
        delete currentBlock;

        // ��������
        removeFullLines();
    }

    void removeFullLines() {
        // ���ڼ�Ȿ�������˼���
        int scoreFlag = 0;

        for (int i = height - 1; i >= 0; i--) {
            // ����i���Ƿ�����
            bool isFull = true;
            for (int j = 0; j < width; j++) {
                if (board[i][j] == 0) {
                    isFull = false;
                    break;
                }
            }

            // �����i�б����������������в������������������һ�в����·���
            if (isFull) {
                //����һ�л��1�֣�����3�֣�����6�֣�����10��
                scoreFlag++;
                switch (scoreFlag) {
                    case 1:
                        score = score + 1;
                        break;
                    case 2:
                        score = score + 2;
                        break;
                    case 3:
                        score = score + 3;
                        break;
                    case 4:
                        score = score + 4;
                        break;
                    default:
                        break;
                }

                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < width; j++) {
                        board[k][j] = board[k - 1][j];
                    }
                }

                // ��������i��
                i++;
            }
        }
    }

    bool isGameOver() {
        // ������̶����Ƿ��з���
        for (int j = 0; j < width; j++) {
            if (board[0][j] == 1) {
                return true;
            }
        }
        return false;
    }

    void handleKey(int key, int directKeyFlag) {
        switch (directKeyFlag) {
            case 1:
                switch (key) {
                    case KEY_LEFT:
                        currentBlock->moveLeft();

                        if (isCollideWithLeft() || isCollideWithBlock()) {
                            currentBlock->moveRight();
                        }
                        break;
                    case KEY_RIGHT:
                        currentBlock->moveRight();

                        if (isCollideWithRight() || isCollideWithBlock()) {
                            currentBlock->moveLeft();
                        }
                        break;
                    case KEY_DOWN:
                        currentBlock->moveDown();

                        if (isCollideWithBottom() || isCollideWithBlock()) {
                            currentBlock->moveUp();
                        }
                        break;
                    case KEY_UP:
                        currentBlock->rotate();

                        if (isCollideWithBlock() || isCollideWithRight() || isCollideWithLeft() || isCollideWithBottom()
                            || isCollideWithTop()) {
                            currentBlock->rerotate();
                        }
                        break;
                }
                break;
            case -1:
                switch (key) {
                    case KEY_A:
                        currentBlock->moveLeft();
                        if (isCollideWithLeft() || isCollideWithBlock()) {
                            currentBlock->moveRight();
                        }
                        currentBlock->moveUp();
                        if (isCollideWithTop() || isCollideWithBlock()) {
                            currentBlock->moveDown();
                        }
                        break;
                    case KEY_D:
                        currentBlock->moveRight();
                        if (isCollideWithRight() || isCollideWithBlock()) {
                            currentBlock->moveLeft();
                        }
                        currentBlock->moveUp();
                        if (isCollideWithTop() || isCollideWithBlock()) {
                            currentBlock->moveDown();
                        }
                        break;
                    case KEY_S:
                        currentBlock->moveDown();

                        if (isCollideWithBottom() || isCollideWithBlock()) {
                            currentBlock->moveUp();
                        }
                        break;
                    case KEY_W:
                        currentBlock->rotate();
                        if (isCollideWithBlock() || isCollideWithRight() || isCollideWithLeft() || isCollideWithBottom()
                            || isCollideWithTop()) {
                            currentBlock->rerotate();
                        }
                        currentBlock->moveUp();
                        if (isCollideWithTop() || isCollideWithBlock()) {
                            currentBlock->moveDown();
                        }
                        break;
                }
                break;
            default:
                break;
        }
    }

    void update() {
        // �����Զ�����
        currentBlock->moveDown();

        if ((isCollideWithBottom() || isCollideWithBlock())) {
            // �����ƶ�
            currentBlock->moveUp();

            // ���û����в������Ȳ��̶�
            if (!_kbhit()) {
                // �̶�����
                fixBlock();
                for (int i = 0; i < height; i++) {
                    for (int j = 0; j < width; j++) {
                        cout << board[i][j] << " ";
                    }
                    cout << endl;
                }

                // �����µķ���
                generateBlock();
            }
        }
    }
};

class Player {
private:
    string name;
    int score;
    int gamemode;

public:
    Player(string _name, int _score = 0, int _gamemode = MODE_EASY) : name(_name), score(_score), gamemode(_gamemode) {
    }

    ~Player() {
    }

    const string &getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    int getGameMode() {
        return gamemode;
    }

    void setScore(int _score) {
        score = _score;
    }

    void setGameMode(int _gamemode) {
        gamemode = _gamemode;
    }
};

class Leaderboard {
private:
    vector<Player *> players;

public:
    ~Leaderboard() {
        // �ͷ��ڴ�
        for (Player *player: players) {
            delete player;
        }
    }

    void addPlayer(Player *player) {
        players.push_back(player);
    }

    void load() {
        // �������а�
        players.clear();
        ifstream file("leaderboard.txt");
        if (!file.is_open()) {
            return;
        }

        string playerName;
        int score;
        int gameMode;
        while (file >> playerName >> score >> gameMode) {
            players.push_back(new Player(playerName, score, gameMode));
        }

        file.close();
    }

    void save() {
        // �洢���а�
        ofstream file("leaderboard.txt");
        if (!file.is_open()) {
            return;
        }

        for (const auto &player: players) {
            file << player->getName() << " " << player->getScore() << " " << player->getGameMode() << endl;
        }
        file.close();
    }

    void sortPlayers() {
        // ����������
        sort(players.begin(), players.end(), [](const Player *a, const Player *b) {
            return a->getScore() > b->getScore();
        });
    }

    void show(int gameMode, int x, int y) {
        // ����Ϸģʽ�������а�
        int rank = 1;
        for (const auto &player: players) {
            if (player->getGameMode() == gameMode) {
                stringstream ss;
                ss << rank++ << ". " << player->getName() << " " << player->getScore();
                string text = ss.str();
                outtextxy(x, y, text.c_str());
                y += gridSize;
                if (rank > 10) {
                    break;
                }
            }
        }
    }
};

class Game {
private:
    Board *board; // ���̶���
    Player *player; // ��Ҷ���
    Leaderboard *leaderboard; // ���а����
    int currentPage; // ��ǰҳ��
    int key = 0; // �û�����
    vector<IMAGE> images; // �洢ͼƬ������
    bool playBGM; // ���ֲ���״̬
    int mascotoLocation; // ������λ��
    int mascotoLocationFlag; // ������λ�ñ�־
    int directKeyFlag; // �������־
    int pauseKeyFlag; // ��ͣ����־
public:
    Game() : board(nullptr), player(nullptr), leaderboard(new Leaderboard), currentPage(PAGE_MAIN_MENU), key(0),
             playBGM(true), mascotoLocation(MASCOT_RIGHT), mascotoLocationFlag(0), directKeyFlag(1), pauseKeyFlag(1) {
    }

    ~Game() {
        delete board;
        delete player;
        delete leaderboard;
    }

    void initGame() {
        // ����ͼƬ
        int width = 16 * gridSize;
        int height = 20 * gridSize;
        images.clear();
        IMAGE img;
        loadimage(&img, "./picture/PAGE_EXIT.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MAIN_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MAIN_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_CREATGAME_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_CREATGAME_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MODE_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MODE_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_GAME1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_GAME2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_PAUSE_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_PAUSE_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_GAMEOVER1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_GAMEOVER2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SAVE_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SAVE_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_LEADERBOARD1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_LEADERBOARD2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SETTINGS_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SETTINGS_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_RULES1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_RULES2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MUSIC_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_MUSIC_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SIZE_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_SIZE_MENU2.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_KEY_MENU1.jpg", width, height);
        images.push_back(img);
        loadimage(&img, "./picture/PAGE_KEY_MENU2.jpg", width, height);
        images.push_back(img);

        // �������а�
        leaderboard->load();
        leaderboard->sortPlayers();

        // ��ʼ����ͼ����
        initgraph(width, height,EX_SHOWCONSOLE);

        // �����Ļ
        setbkcolor(WHITE);
        cleardevice();

        // ����������ʽ����ɫ
        settextstyle(gridSize, 0, _T("΢���ź�"));
        settextcolor(BLACK);
        setbkmode(TRANSPARENT);

        // ���������
        srand(time(0));
    }

    void showExit() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        putimage(0, 0, &images[0]);

        EndBatchDraw();

        Sleep(500);

        exit(0);
    }

    void showMainMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[1]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[2]);
                break;
        }

        EndBatchDraw();
    }

    void showCreatGameMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[3]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[4]);
                break;
        }

        EndBatchDraw();
    }

    void showModeMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[5]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[6]);
                break;
        }

        EndBatchDraw();
    }

    void showGame() {
        // �ж��Ƿ���ڵ�ǰ����
        if (board->getCurrentBlock() == nullptr) {
            board->generateBlock();
        }

        while (currentPage == PAGE_GAME) {
            //�ı伪����״̬
            mascotoLocation = -mascotoLocation;

            // �����û�����
            handleKeyGame();

            // ������Ϸ
            BeginBatchDraw();

            // �����Ļ
            cleardevice();

            // �������˵�����ͼƬ
            switch (mascotoLocation) {
                case MASCOT_RIGHT:
                    putimage(0, 0, &images[7]);
                    break;
                case MASCOT_LEFT:
                    putimage(0, 0, &images[8]);
                    break;
            }

            // ��������
            board->draw();

            EndBatchDraw();

            // ��������
            board->update();

            // ��ʱ
            if (_kbhit()) {
                Sleep(100);
            } else {
                Sleep(player->getGameMode());
            }
            if (board->isGameOver()) {
                currentPage = PAGE_GAMEOVER;
            }
        }
    }

    void showPauseMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[9]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[10]);
                break;
        }

        EndBatchDraw();
    }

    void showGameOver() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[11]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[12]);
                break;
        }

        EndBatchDraw();
    }

    void showSaveMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[13]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[14]);
                break;
        }

        // ��ȡ���д浵�ļ�
        vector<string> saveFiles = getSaveFiles();

        // ��ʾ���д浵�ļ�
        int y = 7 * gridSize;
        for (int i = 0; i < saveFiles.size(); i++) {
            // �򿪴浵�ļ�
            ifstream file("save/" + saveFiles[i]);
            if (!file.is_open()) {
                continue;
            }

            // ��ȡ����ʱ��
            int year, month, day, hour, minute, second;
            file >> year >> month >> day >> hour >> minute >> second;

            // ��ȡ�����Ϣ
            string playerName;
            int score;
            file >> playerName;
            file >> score;

            // ��ȡ��Ϸ�Ѷ�
            int gameMode;
            file >> gameMode;

            // �ر��ļ�
            file.close();

            // ���ƴ浵��Ϣ
            stringstream ss;
            ss << i + 1 << ". " << playerName << " " << score << " ";
            string gameModeText;
            if (gameMode == MODE_EASY) {
                gameModeText = "��";
            } else if (gameMode == MODE_NORMAL) {
                gameModeText = "һ��";
            } else if (gameMode == MODE_HARD) {
                gameModeText = "����";
            }
            ss << gameModeText << " ";
            ss << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second;
            string text = ss.str();
            outtextxy(2 * gridSize, y, text.c_str());
            y += gridSize;
        }

        EndBatchDraw();
    }

    void showLeaderboard() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[15]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[16]);
                break;
        }

        // ��ʾ��ģʽ���а�
        outtextxy(0, 4 * gridSize, _T("��ģʽ"));
        leaderboard->show(MODE_EASY, 0, 5 * gridSize);

        // ��ʾһ��ģʽ���а�
        outtextxy(12 * gridSize, 4 * gridSize, _T("һ��ģʽ"));
        leaderboard->show(MODE_NORMAL, 11 * gridSize, 5 * gridSize);

        // ��ʾ����ģʽ���а�
        outtextxy(6 * gridSize, 4 * gridSize, _T("����ģʽ"));
        leaderboard->show(MODE_HARD, 5 * gridSize + halfgridSize, 5 * gridSize);

        EndBatchDraw();
    }

    void showSettingMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[17]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[18]);
                break;
        }

        EndBatchDraw();
    }

    void showRules() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[19]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[20]);
                break;
        }

        EndBatchDraw();
    }

    void showMusicMenu() {
        BeginBatchDraw();
        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[21]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[22]);
                break;
        }

        EndBatchDraw();
    }

    void showSizeMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[23]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[24]);
                break;
        }

        EndBatchDraw();
    }

    void showKeyMenu() {
        BeginBatchDraw();

        // �����Ļ
        cleardevice();

        // �������˵�����ͼƬ
        switch (mascotoLocation) {
            case MASCOT_RIGHT:
                putimage(0, 0, &images[25]);
                break;
            case MASCOT_LEFT:
                putimage(0, 0, &images[26]);
                break;
        }

        EndBatchDraw();
    }

    char handleMouseClick() {
        MOUSEMSG msg = GetMouseMsg();
        if (msg.uMsg == WM_LBUTTONDOWN) {
            int x = msg.x;
            int y = msg.y;
            if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 5 * gridSize && y < 7 *
                gridSize) {
                return '1';
            } else if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 7 * gridSize +
                       halfgridSize && y < 9 * gridSize + halfgridSize) {
                return '2';
            } else if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 10 * gridSize && y <
                       12 * gridSize) {
                return '3';
            } else if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 12 * gridSize +
                       halfgridSize && y < 14 * gridSize + halfgridSize) {
                return '4';
            } else if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 15 * gridSize && y <
                       17 * gridSize) {
                return '5';
            } else if (x > 5 * gridSize + halfgridSize && x < 10 * gridSize + halfgridSize && y > 17 * gridSize +
                       halfgridSize && y < 19 * gridSize + halfgridSize) {
                return 'd';
            } else if (x > 14 * gridSize + halfgridSize && x < 16 * gridSize + halfgridSize && y > 0 && y < gridSize) {
                return KEY_ESC;
            }
        }
        return '\0'; //�޸�bug
    }

    void handleKeyMainMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // ��ʼ����Ϸ
                currentPage = PAGE_CREATGAME_MENU;
                break;
            case '2': // ��ȡ�浵
                currentPage = PAGE_SAVE_MENU;
                break;
            case '3': // ���а�
                currentPage = PAGE_LEADERBOARD;
                break;
            case '4': // ��Ϸ����
                currentPage = PAGE_RULES;
                break;
            case '5': // ����
                currentPage = PAGE_SETTINGS_MENU;
                break;
            case KEY_ESC: // �˳���Ϸ
                currentPage = PAGE_EXIT;
            default:
                break;
        }
    }

    void handlekeyCreatGameMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '2': {
                // ��ʼ����Ϸ
                char input[20];
                InputBox(input, 20, "���������������");
                string playerName(input);
                player = new Player(playerName);
                board = new Board(0, playerName);
                currentPage = PAGE_MODE_MENU;
            }
            break;
            case KEY_ESC: // �˳���Ϸ
                currentPage = PAGE_MAIN_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeyModeMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // ��ģʽ
                player->setGameMode(MODE_EASY);
                currentPage = PAGE_GAME;
                break;
            case '2': // һ��ģʽ
                player->setGameMode(MODE_NORMAL);
                currentPage = PAGE_GAME;
                break;
            case '3': // ����ģʽ
                player->setGameMode(MODE_HARD);
                currentPage = PAGE_GAME;
                break;
            case KEY_ESC: // �˳���������Ϸ�˵�
                currentPage = PAGE_MAIN_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeyGame() {
        if (_kbhit()) {
            key = _getch();
            board->handleKey(key, directKeyFlag);
            switch (pauseKeyFlag) {
                case 1:
                    switch (key) {
                        case 'p': // ��ͣ��Ϸ
                            currentPage = PAGE_PAUSE_MENU;
                            break;
                    }
                    break;
                case -1:
                    switch (key) {
                        case ' ': // ��ͣ��Ϸ
                            currentPage = PAGE_PAUSE_MENU;
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void handleKeyPauseMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // ������Ϸ
                currentPage = PAGE_GAME;
                break;
            case '2': {
                // ������Ϸ
                // ��ȡ���д浵�ļ�
                vector<string> saveFiles = getSaveFiles();

                // ����浵���������� 9 �������ٱ����µĴ浵������ʾ��ʾ��Ϣ
                if (saveFiles.size() >= 9) {
                    outtextxy(6 * gridSize, 4 * gridSize, "�浵����");
                    Sleep(1000);
                    return;
                } else {
                    saveGame();
                    currentPage = PAGE_SAVE_MENU;
                }
            }
            break;
            case '3': // ������Ϸ
                currentPage = PAGE_GAMEOVER;
                break;
        }
    }

    void handleKeyGameOver() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '2': // �������˵�
                currentPage = PAGE_MAIN_MENU;
                player->setScore(board->getScore());
                leaderboard->addPlayer(player);
                leaderboard->sortPlayers();
                leaderboard->save();
                deleteGame();
                break;
            default:
                break;
        }
    }

    void handleKeySaveMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case KEY_ESC: // �������˵�
                currentPage = PAGE_MAIN_MENU;
                break;
            case 'd': {
                // ɾ���浵
                char input[20];
                InputBox(input, 20, "������Ҫɾ���Ĵ浵���");
                int index = atoi(input) - 1;
                vector<string> saveFiles = getSaveFiles();
                if (index >= 0 && index < saveFiles.size()) {
                    deleteSave(index);
                    outtextxy(2 * gridSize, 6 * gridSize, "ɾ���ɹ�");
                    Sleep(1000);
                } else {
                    outtextxy(2 * gridSize, 6 * gridSize, "�޸ô浵");
                    Sleep(1000);
                }
            }
            break;
            default:
                if (key >= '0' && key <= '9') {
                    int index = key - '0' - 1;
                    vector<string> saveFiles = getSaveFiles();
                    if (index < saveFiles.size()) {
                        loadSave(saveFiles[index]);
                        currentPage = PAGE_GAME;
                    } else {
                        outtextxy(2 * gridSize, 6 * gridSize, "�޸ô浵");
                        Sleep(1000);
                    }
                }
                break;
        }
    }

    void handleKeyLeaderboard() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case KEY_ESC: // �������˵�
                currentPage = PAGE_MAIN_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeySettingMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // �������ֲ˵�
                currentPage = PAGE_MUSIC_MENU;
                break;
            case '2': // �����С�˵�
                currentPage = PAGE_SIZE_MENU;
                break;
            case '3': // ���밴���˵�
                currentPage = PAGE_KEY_MENU;
                break;
            case KEY_ESC:
                currentPage = PAGE_MAIN_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeyRules() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case KEY_ESC: // �������˵�
                currentPage = PAGE_MAIN_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeyMusicMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // ��/�ر�����
                playBGM = !playBGM;
                if (playBGM) {
                    mciSendString("play bgm repeat", nullptr, 0, nullptr);
                } else {
                    mciSendString("stop bgm", nullptr, 0, nullptr);
                }
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case '2': // �л�����һ������
                mciSendString("close bgm", nullptr, 0, nullptr);
                mciSendString("open music/music1.mp3 alias bgm", nullptr, 0, nullptr);
                if (playBGM) {
                    mciSendString("play bgm repeat", nullptr, 0, nullptr);
                }
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case '3': // �л����ڶ�������
                mciSendString("close bgm", nullptr, 0, nullptr);
                mciSendString("open music/music2.mp3 alias bgm", nullptr, 0, nullptr);
                if (playBGM) {
                    mciSendString("play bgm repeat", nullptr, 0, nullptr);
                }
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case '4': // �л�������������
                mciSendString("close bgm", nullptr, 0, nullptr);
                mciSendString("open music/music3.mp3 alias bgm", nullptr, 0, nullptr);
                if (playBGM) {
                    mciSendString("play bgm repeat", nullptr, 0, nullptr);
                }
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case KEY_ESC: // �������ò˵�
                currentPage = PAGE_SETTINGS_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeySizeMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // С����
                gridSize = SMALL_SIZE;
                initGame();
                break;
            case '2': // �н���
                gridSize = MEDIUM_SIZE;
                initGame();
                break;
            case '3': // �����
                gridSize = LARGE_SIZE;
                initGame();
                break;
            case KEY_ESC: // �������ò˵�
                currentPage = PAGE_SETTINGS_MENU;
                break;
            default:
                break;
        }
    }

    void handleKeyKeyMenu() {
        if (MouseHit()) {
            key = handleMouseClick();
        }
        if (_kbhit()) {
            key = _getch();
        }
        switch (key) {
            case '1': // �л������
                directKeyFlag = -directKeyFlag;
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case '2': // �л���ͣ��
                pauseKeyFlag = -pauseKeyFlag;
                outtextxy(6 * gridSize, 4 * gridSize, "�޸ĳɹ�");
                Sleep(1000);
                break;
            case KEY_ESC: // �������ò˵�
                currentPage = PAGE_SETTINGS_MENU;
                break;
            default:
                break;
        }
    }

    void updateMascotoLcation() {
        mascotoLocationFlag++;
        if (mascotoLocationFlag % 600 == 0) {
            mascotoLocation = -mascotoLocation;
        }
    }

    void saveGame() {
        // �����ļ���
        string filename = "save/" + player->getName() + ".txt";

        // ���ļ�
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "�޷����ļ�" << endl;
            return;
        }

        // ��ȡ��ǰʱ��
        time_t t = time(nullptr);
        tm *currentTime = localtime(&t);

        // ���浱ǰʱ��
        file << currentTime->tm_year + 1900 << " ";
        file << currentTime->tm_mon + 1 << " ";
        file << currentTime->tm_mday << " ";
        file << currentTime->tm_hour << " ";
        file << currentTime->tm_min << " ";
        file << currentTime->tm_sec << endl;

        // ���������Ϣ
        file << player->getName() << endl;
        file << board->getScore() << endl;

        // ������Ϸ�Ѷ�
        file << player->getGameMode() << endl;

        // ����������Ϣ
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                file << board->get(i, j) << " ";
            }
            file << endl;
        }

        // ���浱ǰ������Ϣ
        Block *currentBlock = board->getCurrentBlock();
        if (currentBlock != nullptr) {
            file << currentBlock->getShape() << " ";
            file << currentBlock->getColor() << " ";
            file << currentBlock->getX() << " ";
            file << currentBlock->getY() << " ";
            file << currentBlock->getState() << endl;
        } else {
            file << -1 << endl;
        }

        // ������һ��������Ϣ
        Block *nextBlock = board->getNextBlock();
        if (nextBlock != nullptr) {
            file << nextBlock->getShape() << " ";
            file << nextBlock->getColor() << " ";
            file << nextBlock->getX() << " ";
            file << nextBlock->getY() << " ";
            file << nextBlock->getState() << endl;
        } else {
            file << -1 << endl;
        }

        // �ر��ļ�
        file.close();
    }

    void loadSave(const string &filename) {
        ifstream file("save/" + filename);
        if (!file.is_open()) {
            cout << "�޷����ļ�" << endl;
            return;
        }

        // ��ȡ����ʱ��
        int year, month, day, hour, minute, second;
        file >> year >> month >> day >> hour >> minute >> second;

        // ��ȡ�����Ϣ
        string playerName;
        int score;
        file >> playerName;
        file >> score;

        // ��ȡ��Ϸ�Ѷ�
        int gameMode;
        file >> gameMode;

        // ������Ҷ���
        player = new Player(playerName, score, gameMode);

        // �������̶���
        board = new Board(score, playerName);

        // ��ȡ������Ϣ
        for (int i = 0; i < board->getHeight(); i++) {
            for (int j = 0; j < board->getWidth(); j++) {
                int value;
                file >> value;
                board->set(i, j, value);
            }
        }

        // ��ȡ��ǰ������Ϣ
        int shape, color, x, y, state;
        file >> shape;
        if (shape != -1) {
            file >> color >> x >> y >> state;
            switch (shape) {
                case SHAPE_O:
                    board->setCurrentBlock(new OBlock(x, y, color, state));
                    break;
                case SHAPE_I:
                    board->setCurrentBlock(new IBlock(x, y, color, state));
                    break;
                case SHAPE_L:
                    board->setCurrentBlock(new LBlock(x, y, color, state));
                    break;
                case SHAPE_J:
                    board->setCurrentBlock(new JBlock(x, y, color, state));
                    break;
                case SHAPE_S:
                    board->setCurrentBlock(new SBlock(x, y, color, state));
                    break;
                case SHAPE_Z:
                    board->setCurrentBlock(new ZBlock(x, y, color, state));
                    break;
                case SHAPE_T:
                    board->setCurrentBlock(new TBlock(x, y, color, state));
                    break;
                default:
                    break;
            }
        }

        // ��ȡ��һ��������Ϣ
        file >> shape;
        if (shape != -1) {
            file >> color >> x >> y >> state;
            switch (shape) {
                case SHAPE_O:
                    board->setNextBlock(new OBlock(x, y, color, state));
                    break;
                case SHAPE_I:
                    board->setNextBlock(new IBlock(x, y, color, state));
                    break;
                case SHAPE_L:
                    board->setNextBlock(new LBlock(x, y, color, state));
                    break;
                case SHAPE_J:
                    board->setNextBlock(new JBlock(x, y, color, state));
                    break;
                case SHAPE_S:
                    board->setNextBlock(new SBlock(x, y, color, state));
                    break;
                case SHAPE_Z:
                    board->setNextBlock(new ZBlock(x, y, color, state));
                    break;
                case SHAPE_T:
                    board->setNextBlock(new TBlock(x, y, color, state));
                    break;
                default:
                    break;
            }
        }

        // �ر��ļ�
        file.close();
    }

    void deleteSave(int index) {
        // ��ȡ���д浵�ļ�
        vector<string> saveFiles = getSaveFiles();

        // ɾ��ָ���Ĵ浵�ļ�
        string filePath = "save/" + saveFiles[index];
        remove(filePath.c_str());
    }

    vector<string> getSaveFiles() {
        vector<string> saveFiles;
        for (const auto &entry: filesystem::directory_iterator("save")) {
            if (entry.path().extension() == ".txt") {
                saveFiles.push_back(entry.path().filename().string());
            }
        }

        vector<tuple<string, int, string> > saveInfos;
        for (const string &saveFile: saveFiles) {
            ifstream file("save/" + saveFile);
            if (!file.is_open()) {
                continue;
            }

            int year, month, day, hour, minute, second;
            file >> year >> month >> day >> hour >> minute >> second;

            string playerName;
            int score;
            file >> playerName;
            file >> score;

            stringstream ss;
            ss << year << "-" << month << "-" << day << " " << hour << ":" << minute << ":" << second;
            string saveTime = ss.str();

            saveInfos.push_back(make_tuple(saveFile, score, saveTime));

            file.close();
        }

        sort(saveInfos.begin(), saveInfos.end(),
             [](const tuple<string, int, string> &a, const tuple<string, int, string> &b) {
                 return get<2>(a) > get<2>(b);
             });

        vector<string> result;
        for (const auto &saveInfo: saveInfos) {
            result.push_back(get<0>(saveInfo));
        }

        return result;
    }

    void deleteGame() {
        delete board;
        delete player;
    }

    void run() {
        // ��ʼ����Ϸ
        initGame();
        // ���ű�������
        mciSendString("open music/music1.mp3 alias bgm", nullptr, 0, nullptr);
        mciSendString("play bgm repeat", nullptr, 0, nullptr);
        while (true) {
            updateMascotoLcation();
            key = 0; // �����û�����
            switch (currentPage) {
                case PAGE_MAIN_MENU:
                    showMainMenu();
                    handleKeyMainMenu();
                    break;
                case PAGE_CREATGAME_MENU:
                    showCreatGameMenu();
                    handlekeyCreatGameMenu();
                    break;
                case PAGE_GAME:
                    showGame();
                    break;
                case PAGE_MODE_MENU:
                    showModeMenu();
                    handleKeyModeMenu();
                    break;
                case PAGE_PAUSE_MENU:
                    showPauseMenu();
                    handleKeyPauseMenu();
                    break;
                case PAGE_GAMEOVER:
                    showGameOver();
                    handleKeyGameOver();
                    break;
                case PAGE_SAVE_MENU:
                    showSaveMenu();
                    handleKeySaveMenu();
                    break;
                case PAGE_LEADERBOARD:
                    showLeaderboard();
                    handleKeyLeaderboard();
                    break;
                case PAGE_SETTINGS_MENU:
                    showSettingMenu();
                    handleKeySettingMenu();
                    break;
                case PAGE_RULES:
                    showRules();
                    handleKeyRules();
                    break;
                case PAGE_MUSIC_MENU:
                    showMusicMenu();
                    handleKeyMusicMenu();
                    break;
                case PAGE_SIZE_MENU:
                    showSizeMenu();
                    handleKeySizeMenu();
                    break;
                case PAGE_KEY_MENU:
                    showKeyMenu();
                    handleKeyKeyMenu();
                    break;
                case PAGE_EXIT:
                    showExit();
                default:
                    break;
            }
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
