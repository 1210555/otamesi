#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
//盤面の設定
#define NUMrow 18//縦の長さ
#define NUMcol 36//横の長さ
#define NUMmine 60//総地雷数
//ゲーム状態の分岐
enum class GameState {
    Playing,
    GameOver,
    Win,
    Ready
};

#endif