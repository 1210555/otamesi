#include "Field.hpp" 
#include <cstdlib> 
#include <ctime> 

Field::Field() {
    mine = std::vector<std::vector<bool>>(NUMrow, std::vector<bool>(NUMcol, false));
    open = std::vector<std::vector<bool>>(NUMrow, std::vector<bool>(NUMcol, false));
    frag = std::vector<std::vector<bool>>(NUMrow, std::vector<bool>(NUMcol, false));
}

void Field::minePlace() {
    int k = 0;
    while (k < NUMmine) {
        int mineRow = rand() % NUMrow;
        int mineCol = rand() % NUMcol;
        if (!mine[mineRow][mineCol]) {
            mine[mineRow][mineCol] = true;
            k++;
        }
    }
}

int Field::Count(int x, int y) const {
    int AroundMineCount = 0;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (nx >= 0 && nx < NUMrow && ny >= 0 && ny < NUMcol) {
            if (mine[nx][ny]) {
                AroundMineCount++;
            }
        }
    }
    return AroundMineCount;
}

bool Field::Mined(int x, int y) const {
    return mine[x][y];
}

void Field::Open(int x, int y) {
    open[x][y] = true;
}

bool Field::Opened(int x, int y) const {
    return open[x][y];
}

const std::vector<std::vector<bool>>& Field::getOpen() const {
    return open;
}

void Field::Flag(int x, int y) {
    frag[x][y] = !frag[x][y];
}

bool Field::Flagged(int x, int y) const {
    return frag[x][y];
}

const std::vector<std::vector<bool>>& Field::getFlag() const {
    return frag;
}

int Field::autoRelease(int x, int y) {
    int OpenCount = 1;
    if (x < 0 || x >= NUMrow || y < 0 || y >= NUMcol) {
        return 0;
    }
    if (Opened(x, y) || Flagged(x, y)) {
        return 0;
    }
    Open(x, y);
    if (Mined(x, y)) {
        return 0;
    }
    if (Count(x, y) != 0) {
        return OpenCount;
    }

    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (nx >= 0 && nx < NUMrow && ny >= 0 && ny < NUMcol) {
            if (!Opened(nx, ny) && !Flagged(nx, ny)) { // オートリリース中にフラグ付きは開かない
                OpenCount += autoRelease(nx, ny);
            }
        }
    }
    return OpenCount;
}
