#include "Game.hpp" 
#include <iostream>
#include <cstdlib> 

Game::Game()
    : UI_AREA_HEIGHT(70),//UI部分の高さ
      font(),             // sf::Font をデフォルト構築 (ロードは後で行う)
      window(sf::VideoMode(NUMcol * 50, NUMrow * 50 + UI_AREA_HEIGHT), "MineSweeper"),
      state(GameState::Ready), // 初期状態を Ready に設定
      initialOpenNumber(0),
      initialTotalPlace(NUMrow * NUMcol),
      initialSafePlace(initialTotalPlace - NUMmine),
      field(),
      gameRenderer(UI_AREA_HEIGHT),
      gameUI(NUMcol * 50, NUMrow * 50 + UI_AREA_HEIGHT, UI_AREA_HEIGHT)
{
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Error loading font: resources/arial.ttf" << std::endl;
        window.close();
        exit(EXIT_FAILURE); // フォントロード失敗時は終了
    }
    // ロードしたフォントを GameUI に設定
    gameUI.setFont(font);
}

// ゲームの状態をリセットする関数
void Game::resetGame() {
    field = Field(); //Fieldオブジェクトを再構築しリセット
    field.minePlace(); //新しく地雷配置
    OpenNumber = initialOpenNumber; //OpenNumberをリセット(0 に戻す)
    state = GameState::Playing; //状態をPlayingに戻す
}

void Game::Run() {
    int tileSize = 50; //タイルサイズ(ゲーム全体で共通の定数)
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // マウスのクリック座標の取得とUI部分の調整
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int x = mousePos.x / tileSize;
            int y = (mousePos.y - UI_AREA_HEIGHT) / tileSize;

            // 左クリックの処理
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // UI エリアのクリックの処理
                if (mousePos.y >= 0 && mousePos.y < UI_AREA_HEIGHT) {
                    if (state == GameState::GameOver || state == GameState::Win || state == GameState::Ready) {
                        if (gameUI.isRestartButtonClicked(mousePos)) {
                            resetGame(); // ゲームをリセットして開始
                        }
                    }
                }else if (state == GameState::Playing) {
                    // クリック座標が有効な範囲内かチェック
                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if (field.Flagged(y, x)) {
                            continue;
                        } else if (field.Opened(y, x)) {
                            continue;
                        } else if (field.Mined(y, x)) {
                            state = GameState::GameOver;
                        } else {
                            OpenNumber += field.autoRelease(y, x); 
                            if (OpenNumber == initialSafePlace) {
                                state = GameState::Win;
                            }
                        }
                    }
                }
            }
            // 右クリック
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (state == GameState::Playing) {
                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if (!field.Opened(y, x)) {
                            field.Flag(y, x); //フラグをトグル
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::Black);
        if (state == GameState::Playing || state == GameState::GameOver || state == GameState::Win) {
            gameRenderer.display(window, tileSize, state, font, field, field.getOpen(), field.getFlag());
        }
        gameUI.Draw(window, state);
        window.display();
    }
}