#include "GameRenderer.hpp"
#include <iostream>

GameRenderer::GameRenderer(int offset) : uiOffset(offset) {
    // テクスチャファイルへパスを定義
    std::map<std::string, std::string> files = {
        {"white", "resources/GroundWhite.jpg"},
        {"flag",  "resources/Flag2.png"},
        {"mine","resources/Mine.jpg"},
        {"boom",  "resources/Boom.jpg"}
    };

    // 全てのテクスチャをループで読み込み、textures マップに格納
    for(const auto& pair : files){
        sf::Texture texture;
        if(!texture.loadFromFile(pair.second)){//パスにアクセス
            std::cerr << "Error loading texture: " << pair.second << std::endl;
        }
        textures.emplace(pair.first, std::move(texture));//キーにアクセス
    }
}

void GameRenderer::display(sf::RenderWindow& window, int tileSize, GameState currentState, const sf::Font& font, const Field& field, const std::vector<std::vector<bool>>& opened, const std::vector<std::vector<bool>>& flagged) {
    sf::Sprite sprite;
    sf::RectangleShape tile(sf::Vector2f(static_cast<float>(tileSize - 2), static_cast<float>(tileSize - 2))); // float キャストを追加

    for (int i = 0; i < NUMrow; i++){
        for (int j = 0; j < NUMcol; j++){
            tile.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
            
            if(opened[i][j]){
                tile.setFillColor(sf::Color::White);//開いたマスは白
            }else{
                tile.setFillColor(sf::Color(200, 200, 200));//開いて無いマスの色
            }
            window.draw(tile);

            if (currentState == GameState::GameOver) {
                if (field.Mined(i, j)) {
                    sprite.setTexture(textures["boom"]);
                    sprite.setScale(static_cast<float>(tileSize) / textures["boom"].getSize().x,
                                    static_cast<float>(tileSize) / textures["boom"].getSize().y);
                    sprite.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
                    window.draw(sprite);
                } else if (opened[i][j] && field.Count(i,j) > 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(field.Count(i, j)));
                    text.setCharacterSize(24);
                    // 数字の色
                    if (field.Count(i, j) == 1){
                        text.setFillColor(sf::Color::Blue);
                    }
                    else if (field.Count(i, j) == 2){
                        text.setFillColor(sf::Color::Green);
                    }
                    else if (field.Count(i, j) >= 3){
                        text.setFillColor(sf::Color::Red);
                    }
                    
                    sf::FloatRect textBounds = text.getLocalBounds();
                    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                    text.setPosition(static_cast<float>(j * tileSize + tileSize / 2.0f), static_cast<float>(i * tileSize + tileSize / 2.0f + uiOffset));
                    window.draw(text);
                }
                else if (field.Mined(i, j) && !opened[i][j] && !flagged[i][j]) { // 踏んでない地雷は地雷画像
                    sprite.setTexture(textures["mine"]);
                    sprite.setScale(static_cast<float>(tileSize) / textures["mine"].getSize().x,
                                    static_cast<float>(tileSize) / textures["mine"].getSize().y);
                    sprite.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
                    window.draw(sprite);
                }
                else if (flagged[i][j]) {
                    sprite.setTexture(textures["flag"]);
                    sprite.setScale(static_cast<float>(tileSize) / textures["flag"].getSize().x,
                                    static_cast<float>(tileSize) / textures["flag"].getSize().y);
                    sprite.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
                    window.draw(sprite);
                }

            } else if (currentState == GameState::Win) {
                if (!field.Mined(i, j)) {
                    int count = field.Count(i, j);
                    if (count > 0) { // 数字が1以上の場合は数字を表示
                        sf::Text text;
                        text.setFont(font);
                        text.setString(std::to_string(count));
                        text.setCharacterSize(24);
                        if (field.Count(i, j) == 1){
                            text.setFillColor(sf::Color::Blue);
                        }
                        else if (field.Count(i, j) == 2){
                            text.setFillColor(sf::Color::Green);
                        }
                        else if (field.Count(i, j) >= 3){
                            text.setFillColor(sf::Color::Red);
                        }
                        
                        sf::FloatRect textBounds = text.getLocalBounds();
                        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                        text.setPosition(static_cast<float>(j * tileSize + tileSize / 2.0f), static_cast<float>(i * tileSize + tileSize / 2.0f + uiOffset));
                        window.draw(text);
                    }
                } else {
                    sprite.setTexture(textures["mine"]); 
                    sprite.setScale(static_cast<float>(tileSize) / textures["mine"].getSize().x,static_cast<float>(tileSize) / textures["mine"].getSize().y);
                    sprite.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
                    window.draw(sprite);
                }
            } else { //PlayingかReadyのとき
                if (flagged[i][j]) { // フラグ表示
                    sprite.setTexture(textures["flag"]);
                    sprite.setScale(static_cast<float>(tileSize) / textures["flag"].getSize().x,static_cast<float>(tileSize) / textures["flag"].getSize().y);
                    sprite.setPosition(static_cast<float>(j * tileSize), static_cast<float>(i * tileSize + uiOffset));
                    window.draw(sprite);
                } else if (opened[i][j]) { 
                    int count = field.Count(i, j);
                    if (count > 0) { // 周囲の地雷数が1以上の場合は数字を表示
                        sf::Text text;
                        text.setFont(font);
                        text.setString(std::to_string(count));
                        text.setCharacterSize(24);
                        if (field.Count(i, j) == 1){
                            text.setFillColor(sf::Color::Blue);
                        }
                        else if (field.Count(i, j) == 2){
                            text.setFillColor(sf::Color::Green);
                        }
                        else if (field.Count(i, j) >= 3){
                            text.setFillColor(sf::Color::Red);
                        }
                        
                        sf::FloatRect textBounds = text.getLocalBounds();
                        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                        text.setPosition(static_cast<float>(j * tileSize + tileSize / 2.0f), static_cast<float>(i * tileSize + tileSize / 2.0f + uiOffset));
                        window.draw(text);
                    }
                }
            }
        }
    }
}