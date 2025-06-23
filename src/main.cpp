#include <iostream>
#include <ctime>
#include <cstdlib>

//クラスのヘッダーをインクルード
#include "Constants.hpp"
#include "Game.hpp"

int main() {
    srand(static_cast<unsigned int>(time(0))); 
    Game game;
    game.Run();

    return 0;
}