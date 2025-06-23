#ifndef FIELD_HPP 
#define FIELD_HPP
#include <vector>
#include "Constants.hpp"

class Field {
private:
    std::vector<std::vector<bool>> mine;
    std::vector<std::vector<bool>> open;
    std::vector<std::vector<bool>> frag;

public:
    Field(); 
    void minePlace(); // 地雷の配置
    int Count(int x, int y) const; // 地雷数カウント
    bool Mined(int x, int y) const; // 地雷判定 
    void Open(int x, int y); // マスを開く
    bool Opened(int x, int y) const; // マスが開かれているか
    const std::vector<std::vector<bool>>& getOpen() const;
    void Flag(int x, int y); // フラグをトグルする
    bool Flagged(int x, int y) const; // フラグが立っているか
    const std::vector<std::vector<bool>>& getFlag() const; 
    int autoRelease(int x, int y); // オートリリース
};

#endif 