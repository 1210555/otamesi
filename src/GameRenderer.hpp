#ifndef GAMERENDERER_HPP
#define GAMERENDERER_HPP
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include "Constants.hpp"
#include "Field.hpp"

class GameRenderer {
private:
    std::map<std::string, sf::Texture> textures;
    //UIエリアによるY座標のずれ
    int uiOffset;

public:
    GameRenderer(int offset);

    //ゲームの状態情報をconst参照として受け取る
    void display(
        sf::RenderWindow& window,
        int tileSize,
        GameState currentState,// 現在のゲームの状態 (Playing/GameOver/Win)
        const sf::Font& font,
        const Field& field,
        const std::vector<std::vector<bool>>& opened,// 開かれているマスの状態
        const std::vector<std::vector<bool>>& flagged// フラグが立っているマスの状態
    );
};

#endif // GAMERENDERER_HPP