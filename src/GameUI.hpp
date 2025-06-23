#ifndef GAMEUI_HPP
#define GAMEUI_HPP
#include <SFML/Graphics.hpp>
#include "Constants.hpp"

class GameUI {
    private:
        // sf::Font &Font; // setFontで後から設定
        sf::Text leftInstructionText;
        sf::Text rightInstructionText;
        sf::Text gameOverText;
        sf::Text winText;
        sf::RectangleShape restartButtonShape;
        sf::Text restartButtonText;
        int windowWidth;
        int windowHeight;
        int uiOffset;

    public:
        GameUI(int windowWidth, int windowHeight, int offset);

    void setFont(const sf::Font& loadedFont);
    void Draw(sf::RenderWindow& window, GameState currentState) const;
    bool isRestartButtonClicked(const sf::Vector2i& mousePos) const;
};

#endif