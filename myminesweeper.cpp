#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include <SFML/Graphics.hpp>

#define NUMrow 5
#define NUMcol 5
#define NUMmine 3
using namespace std;

enum class GameState {
    Playing,
    GameOver,
    Win
};

class Field{
    private:
    vector<vector<bool>> mine;
    vector<vector<bool>> open;
    vector<vector<bool>> frag;

    public:
    Field(){
        mine=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        open=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        frag=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
    }
    void minePlace(){
        int k=0;
        while(k<NUMmine){
            int mineRow=rand()%NUMrow;
            int mineCol=rand()%NUMcol;
            if(!mine[mineRow][mineCol]){
                mine[mineRow][mineCol]=true;
                k++;
            }
            cout<<"( "<<mineRow+1<<","<<mineCol+1<<" )"<<endl;//地雷の位置（デバック用）
        }
    }
    //地雷数カウント
    int Count(int x,int y)const{
        int AroundMineCount=0;
        int nx,ny;
        int dx[8]={-1,-1,-1,0,0,1,1,1};
        int dy[8]={-1,0,1,-1,1,-1,0,1};

        for(int k=0;k<8;k++){//8はある行列の成分の周辺の要素を調べるためのループ
            nx=x+dx[k];
            ny=y+dy[k];
            if(nx>=0&&nx<NUMrow&&ny>=0&&ny<NUMcol){//&の時はどれか一つ満たせばよい
                if(mine[nx][ny]){//地雷があるならif文にはいる
                    AroundMineCount++;
                }
            }
        }
        return AroundMineCount;
    }
    bool Mined(int x,int y)const{
        return mine[x][y];
        }
    void Open(int x,int y){
        open[x][y]=true;
    }
    bool Opened(int x,int y)const{
        return open[x][y];
    }
    const vector<vector<bool>>& getOpen() const{
        return open;
    }
    void Flag(int x,int y){
        frag[x][y]=!frag[x][y];
    }
    bool Flagged(int x,int y)const{
        return frag[x][y];
    }
    const vector<vector<bool>>& getFlag()const{
        return frag;
    }

    int autoRelease(int x, int y) {    
        int OpenCount=1;    
        if(x<0||x>=NUMrow||y<0||y>=NUMcol){
            return 0; 
        }
        if(Opened(x,y)||Flagged(x,y)){
            return 0;// すでに開いているかのチェック
        }
        Open(x,y);
        if(Mined(x,y)){
            return 0;// 地雷があるかどうかのチェック
        }
        if(Count(x,y)!=0){
            return OpenCount;
        }
    
        int dx[8]= {-1,-1,-1,0,0,1,1,1};
        int dy[8]= {-1,0,1,-1,1,-1,0,1};

        for(int k=0;k<8;k++){
            int nx=x+dx[k];
            int ny=y+dy[k];
            if(nx>=0&&nx<NUMrow&&ny>=0&&ny<NUMcol){
                if(!Opened(nx,ny)){
                    OpenCount+=autoRelease(nx, ny);
                }
            }
        }
        return OpenCount;
    }
};

class Output{
    private:
    map<string,sf::Texture> textures;
    int UIOffset; // UIのオフセット値
    
    public:
    Output(int offset):UIOffset(offset){
    map<string,string> files={
            {"white","resources/GroundWhite.jpg"},
            {"mine","resources/Mine.jpg"},
            {"flag","resources/Flag2.png"},
            {"boom","resources/Boom.jpg"}
        };

        for(const auto& [key,path] : files){
            sf::Texture texture;
            if(!texture.loadFromFile(path)){
                cerr << "Error loading texture: " << path << endl;
                return;
            }
            textures.emplace(key, move(texture));
        }
    }

    void display(sf::RenderWindow& window,int tileSize,GameState currentState, sf::Font& font,const Field& field,const vector<vector<bool>>& opened,const vector<vector<bool>>& flagged){ 

        sf::Sprite sprite;
        sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));

        for(int i=0;i<NUMrow;i++){
            for(int j=0;j<NUMcol;j++){
                tile.setPosition(j*tileSize,i*tileSize+UIOffset);
                tile.setFillColor(sf::Color(200,200,200));
                window.draw(tile);
                    //ゲームオーバー時の処理
                if(currentState == GameState::GameOver){
                    if(!field.Mined(i,j)){
                        int count = field.Count(i, j);
                        if (count >= 0) { // 周囲の地雷数が1以上の場合は数字を表示
                            sf::Text text;
                            text.setFont(font); // 受け取ったfontを使用
                            text.setString(to_string(count));
                            text.setCharacterSize(24);
                            //数字ごとに色変化
                            if (count == 1){
                                text.setFillColor(sf::Color::Blue);
                            }else if (count == 2){
                                text.setFillColor(sf::Color::Green);
                            }else if (count >= 3){
                                text.setFillColor(sf::Color::Red);
                            }
                            // 数字の位置をタイルの中央に調整 (UIOffsetを考慮)
                            sf::FloatRect textBounds = text.getLocalBounds();
                            text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                            text.setPosition(j * tileSize + tileSize / 2.0f, i * tileSize + tileSize / 2.0f + UIOffset);
                            window.draw(text);
                        }
                    }else{
                        sprite.setTexture(textures["boom"]);
                        sf::Vector2u textureSize = textures["boom"].getSize(); // 元の画像サイズ
                        float scaleX = tileSize / static_cast<float>(textureSize.x);
                        float scaleY = tileSize / static_cast<float>(textureSize.y);
                        sprite.setScale(scaleX, scaleY);
                        sprite.setPosition(j * tileSize, i * tileSize + UIOffset);
                        window.draw(sprite);
                        //cout<< "Boom! You hit a mine at (" << i + 1 << ", " << j + 1 << ")." << endl; // デバッグ用
                    }
                }else if(currentState == GameState::Win){
                            sprite.setTexture(textures["white"]);
                            sf::Vector2u textureSize = textures["white"].getSize(); // 元の画像サイズ
                            float scaleX = tileSize / static_cast<float>(textureSize.x);
                            float scaleY = tileSize / static_cast<float>(textureSize.y);
                            sprite.setScale(scaleX, scaleY);
                            sprite.setPosition(j * tileSize, i * tileSize + UIOffset);
                            window.draw(sprite);
                }else{
                    if (flagged[i][j]) {
                    // フラグ表示
                        sprite.setTexture(textures["flag"]);
                        sf::Vector2u textureSize = textures["flag"].getSize();
                        float scaleX = tileSize / static_cast<float>(textureSize.x);
                        float scaleY = tileSize / static_cast<float>(textureSize.y);
                        sprite.setScale(scaleX, scaleY);
                        sprite.setPosition(j * tileSize, i * tileSize + UIOffset); 
                        window.draw(sprite);
                    } else if (opened[i][j]) {
                    // 開いているマス（地雷ではない）
                    // 周囲の地雷数表示のロジックをここに書く
                        if (!field.Mined(i, j)) {
                            int count = field.Count(i, j);
                            if (count >= 0) { // 周囲の地雷数が1以上の場合は数字を表示
                                sf::Text text;
                                text.setFont(font); // 受け取ったfontを使用
                                text.setString(to_string(count));
                                text.setCharacterSize(24);
                                //数字ごとに色変化
                                if (count == 1){
                                    text.setFillColor(sf::Color::Blue);
                                }else if (count == 2){
                                    text.setFillColor(sf::Color::Green);
                                }else if (count >= 3){
                                    text.setFillColor(sf::Color::Red);
                                }
                                // 数字の位置をタイルの中央に調整 (UIOffsetを考慮)
                                sf::FloatRect textBounds = text.getLocalBounds();
                                text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                                text.setPosition(j * tileSize + tileSize / 2.0f, i * tileSize + tileSize / 2.0f + UIOffset);
                                window.draw(text);
                            }
                        }
                    }
                }
            }
        }
    
    }
};

class GameUI{
    private:
    //sf::Font &Font;
    sf::Text leftInstructionText;
    sf::Text rightInstructionText;
    sf::Text gameOverText;
    sf::Text winText;
    int windowWidth; // ウィンドウの幅
    int windowHeight; // ウィンドウの高さ
    int UIOffset; // UIのオフセット値

    public:
    GameUI(int windowWidth,int windowHeight,int offset):windowWidth(windowWidth),windowHeight(windowHeight),UIOffset(offset){}
    
    void setFont(const sf::Font& loadedFont){
        leftInstructionText.setFont(loadedFont);
        leftInstructionText.setString("Left Click: Open Tile");
        leftInstructionText.setCharacterSize(20);
        leftInstructionText.setFillColor(sf::Color::Blue);
        leftInstructionText.setPosition(10, 10);

        rightInstructionText.setFont(loadedFont);
        rightInstructionText.setString("Right Click: Flag Tile");
        rightInstructionText.setCharacterSize(20);
        rightInstructionText.setFillColor(sf::Color::Blue);
        rightInstructionText.setPosition(10, 40);

        gameOverText.setFont(loadedFont);
        gameOverText.setString("Game Over!");
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);

        sf::FloatRect goRect = gameOverText.getLocalBounds();
        cout<<"goRect.left is "<<goRect.left<<" , goRect.width is "<<goRect.width<<endl;
        gameOverText.setOrigin(goRect.width / 2.0f, goRect.top + goRect.height / 2.0f);
        gameOverText.setPosition(windowWidth-1 / 2.0f, UIOffset/2);

        winText.setFont(loadedFont);
        winText.setString("You Win!");
        winText.setCharacterSize(48);
        winText.setFillColor(sf::Color::Green);

        sf::FloatRect winRect = winText.getLocalBounds();
        winText.setOrigin(winRect.left+winRect.width / 2.0f, winRect.top + winRect.height / 2.0f);
        winText.setPosition(windowWidth / 2.0f, (windowHeight - UIOffset) / 2.0f + UIOffset);
    }
    
    void Draw(sf::RenderWindow&window,GameState currentState){

        if(currentState!=GameState::GameOver){
            window.draw(leftInstructionText);
            window.draw(rightInstructionText);
        }
        if(currentState == GameState::GameOver){
            // ゲームオーバーテキスト描画
            window.draw(gameOverText);
        }else if(currentState == GameState::Win){
            // 勝利テキスト描画
            window.draw(winText);
        }
    }
};

class Game{
    private:
    const int UI_AREA_HEIGHT=70; // UIエリアの高さ
    sf::Font font;
    sf::RenderWindow window;
    Field field;
    GameState state;
    Output output;
    GameUI gameUI;
    
    public:
    Game()
        :UI_AREA_HEIGHT(70), // UIエリアの高さ
        font(),
        window(sf::VideoMode(NUMcol * 50,NUMrow * 50 + UI_AREA_HEIGHT), "MineSweeper"),
        state(GameState::Playing),
        field(),
        output(UI_AREA_HEIGHT),
        gameUI(NUMcol * 50, NUMrow * 50 + UI_AREA_HEIGHT, UI_AREA_HEIGHT) // GameUIの初期化
    {
        if(!font.loadFromFile("resources/arial.ttf")) {
            cerr << "Error loading font" << endl;
            window.close();
            exit(EXIT_FAILURE);
        }
        gameUI.setFont(font);
        field.minePlace();
    }

    void Run(){
        int tileSize = 50;
        int OpenNumber = 0;
        int TotalPlace = NUMrow * NUMcol;
        int SafePlace = TotalPlace - NUMmine;

        while (window.isOpen()) {
        sf::Event event;        
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int x = mousePos.x / tileSize;
                int y = (mousePos.y - UI_AREA_HEIGHT) / tileSize;//UIエリア分引く

                //左クリックの処理
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

                    cout<< "Clicked at: (" << x << ", " << y << ")" << endl;

                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if(field.Flagged(y,x)){
                            continue;
                        }else if(field.Mined(y,x)){
                            state = GameState::GameOver;
                            //field.revealAllTiles();
                        }else{
                            OpenNumber+=field.autoRelease(y, x);
                            //cout << "OpenNumber: " << OpenNumber << endl; // デバッグ用
                            if(SafePlace==OpenNumber){
                                state= GameState::Win;
                                //field.revealAllTiles();
                            }
                        }
                    }
                //右クリックの処理
                }else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
                    //cout<< "Flagged at: (" << x << ", " << y << ")" << endl;デバック用
                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if(!field.Opened(y,x)){ // まだ開いていないタイルに対してフラグを設定
                            field.Flag(y,x);
                        }
                    }
                }
            }
            window.clear();
            output.display(window, tileSize,state, font,field,field.getOpen(), field.getFlag());
            gameUI.Draw(window, state);
            window.display();
        }
    }
};

    int main(){
        srand(time(0));
        Game game;
        game.Run();

        return 0;
    }
