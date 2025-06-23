#include "GameUI.hpp" // 対応するヘッダーファイルをインクルード

GameUI::GameUI(int windowWidth, int windowHeight, int offset): windowWidth(windowWidth), windowHeight(windowHeight), uiOffset(offset) {
    // リスタートボタンの形状の初期化
    restartButtonShape.setSize(sf::Vector2f(100.0f, 30.0f));
    restartButtonShape.setFillColor(sf::Color(100, 100, 250)); // 青っぽい
    restartButtonShape.setOutlineThickness(2);
    restartButtonShape.setOutlineColor(sf::Color::White);
    restartButtonShape.setPosition(static_cast<float>(windowWidth - 10 - restartButtonShape.getSize().x),static_cast<float>(uiOffset / 2.0f - restartButtonShape.getSize().y / 2.0f));

}

void GameUI::setFont(const sf::Font& loadedFont) {
    // 左クリック説明
    leftInstructionText.setFont(loadedFont);
    leftInstructionText.setString("Left Click: Open Tile");
    leftInstructionText.setCharacterSize(20);
    leftInstructionText.setFillColor(sf::Color::White);
    leftInstructionText.setPosition(10.0f, 10.0f);

    // 右クリック説明
    rightInstructionText.setFont(loadedFont);
    rightInstructionText.setString("Right Click: Flag Tile");
    rightInstructionText.setCharacterSize(20);
    rightInstructionText.setFillColor(sf::Color::White);
    rightInstructionText.setPosition(10.0f, 40.0f);

    // ゲームオーバーメッセージの設定
    gameOverText.setFont(loadedFont);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(30); // サイズ
    gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect goRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(goRect.width / 2.0f, goRect.top + goRect.height / 2.0f);
    gameOverText.setPosition(windowWidth/10.0f , uiOffset / 2);//UI部分右上に表示

    // 勝利メッセージの設定
    winText.setFont(loadedFont);
    winText.setString("You Win!");
    winText.setCharacterSize(48);
    winText.setFillColor(sf::Color::Green);
    sf::FloatRect winRect = winText.getLocalBounds();
    winText.setOrigin(winRect.width / 2.0f, winRect.top + winRect.height / 2.0f);
    winText.setPosition(windowWidth/10.0f , uiOffset / 2);//UI部分右上に表示

    // リスタートボタン
    restartButtonText.setFont(loadedFont);
    restartButtonText.setString("RESTART");
    restartButtonText.setCharacterSize(18);
    restartButtonText.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = restartButtonText.getLocalBounds();
    restartButtonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    restartButtonText.setPosition(static_cast<float>(restartButtonShape.getPosition().x + restartButtonShape.getSize().x / 2.0f),
                                  static_cast<float>(restartButtonShape.getPosition().y + restartButtonShape.getSize().y / 2.0f));
}

void GameUI::Draw(sf::RenderWindow& window, GameState currentState) const {
    // Playingのみ操作説明テキストを描画
    if (currentState == GameState::Playing) {
        window.draw(leftInstructionText);
        window.draw(rightInstructionText);
    }
    if (currentState == GameState::GameOver || currentState == GameState::Win || currentState == GameState::Ready) {
        window.draw(restartButtonShape); 
        window.draw(restartButtonText);

        if (currentState == GameState::GameOver) {
            window.draw(gameOverText);
        }else if (currentState == GameState::Win) {
            window.draw(winText);
        }
    }
}

bool GameUI::isRestartButtonClicked(const sf::Vector2i& mousePos) const {
    // マウス位置がボタンの描画領域内にあるかを判定
    return restartButtonShape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}