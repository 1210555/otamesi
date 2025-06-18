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
    vector<vector<bool>> Mine;
    vector<vector<string>> board;
    vector<vector<bool>> open;
    vector<vector<bool>> frag;
    vector<vector<string>> displayMine;

    public:
    Field(){
        open=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        frag=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        Mine=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        board=vector<vector<string>>(NUMrow,vector<string>(NUMcol,"*"));
        displayMine=vector<vector<string>>(NUMrow,vector<string>(NUMcol,"*"));

    }
    void minePlace(){
        int k=0;
        while(k<NUMmine){
            int mineRow=rand()%NUMrow;
            int mineCol=rand()%NUMcol;
            if(!Mine[mineRow][mineCol]){
                Mine[mineRow][mineCol]=true;
                k++;
            }
            cout<<"( "<<mineRow+1<<","<<mineCol+1<<" )"<<endl;//デバック用
        }
    }
    /*void Board(){
        cout<<"    ";
        for(int k=0;k<NUMcol;k++){
            cout<<setw(3)<<k+1;
        }
        cout<<endl<<"   ";
        for(int k=0;k<NUMcol;k++){
            cout<<"---";
        }
        cout<<"-"<<endl;
        for(int i=0;i<NUMrow;i++){
            cout<<setw(2)<<i+1<<" |";
            for(int j=0;j<NUMcol;j++){
                cout<<setw(3)<<board[i][j];
            }
            cout<<endl;
        }
    }*/
    bool MINE(int x,int y){
        return Mine[x][y];//start内で入力したx,yの位置を返す
    }//二次元配列作ってるのはFieldだけだからクラス間の受け渡しが必要（二次元配列をGame内でもつくればこの関数はいらない）
    //地雷数カウント
    int Count(int x,int y){
        int AroundMineCount=0;
        int nx,ny;
        int dx[8]={-1,-1,-1,0,0,1,1,1};
        int dy[8]={-1,0,1,-1,1,-1,0,1};

        for(int k=0;k<8;k++){//8はある行列の成分の周辺の要素を調べるためのループ
            nx=x+dx[k];
            ny=y+dy[k];
            if(nx>=0&&nx<NUMrow&&ny>=0&&ny<NUMcol){//&の時はどれか一つ満たせばよい
                if(Mine[nx][ny]){//地雷があるならif文にはいる
                    AroundMineCount++;
                }
            }
        }
        return AroundMineCount;
    }
    string ChangeCount(int x,int y){
        board[x][y]=to_string(Count(x,y));
        return board[x][y];
    }
    string ChangeFrag(int x,int y){
        board[x][y]="F";
        return board[x][y];
    }
    void Open(int x,int y){
        open[x][y]=true;
    }
    bool Opened(int x,int y){
        return open[x][y];
    }
    void Frag(int x,int y){
        frag[x][y]=true;
    }
    string CancelFrag(int x,int y){
        board[x][y]="*";//ちょっと強引かも
        frag[x][y]=false;
        return board[x][y];
    }
    bool Fragged(int x,int y){
        return frag[x][y];
    }
    int AutoRelease(int x,int y){
        int dx[8]={-1,-1,-1,0,0,1,1,1};
        int dy[8]={-1,0,1,-1,1,-1,0,1};

        if(x<0||x>=NUMrow||y<0||y>=NUMcol){
            return 0;
        }
        if(open[x][y]||frag[x][y]){
            return 0;
        }
        Open(x,y);
        board[x][y]=to_string(Count(x,y));
        int autoOpen=1;

        if(Count(x,y)==0 && !Mine[x][y]){
            for(int k=0;k<8;k++){
                int nx=x+dx[k];
                int ny=y+dy[k];
                if(nx>=0&&nx<NUMrow&&ny>=0&&ny<NUMcol){
                    if(!open[nx][ny]&&!frag[nx][ny]){
                        if(Count(nx,ny)==0){

                        autoOpen+=AutoRelease(nx,ny);
                    }else{
                        Open(nx,ny);
                        board[nx][ny]=to_string(Count(nx,ny));
                        autoOpen++;
                        }
                    }
                }
            }
        }
        return autoOpen;
    }
    void openMine(int x,int y){

        for(int i=0;i<NUMrow;i++){
            for(int j=0;j<NUMcol;j++){
                if(Mine[i][j]){
                    displayMine[i][j]="X";
                }else{
                    displayMine[i][j]=to_string(Count(i,j));
                }
            }
        }


        cout<<"    ";
        for(int k=0;k<NUMcol;k++){
            cout<<setw(3)<<k+1;
        }
        cout<<endl<<"   ";
        for(int k=0;k<NUMcol;k++){
            cout<<"---";
        }
        cout<<"-"<<endl;
        for(int i=0;i<NUMrow;i++){
            cout<<setw(2)<<i+1<<" |";
            for(int j=0;j<NUMcol;j++){
                cout<<setw(3)<<displayMine[i][j];
            }
            cout<<endl;
        }
    }
    
};

class Tile{
        // SFMLを使ったグラフィックスの実装はここに追加できます。
        // ただし、SFMLのセットアップとウィンドウの管理が必要です。
    private:
        map<string,sf::Texture> textures;
        vector<vector<bool>> Mine;
        vector<vector<bool>> flagged;
        GameState state;

    public:
        Field field;
        bool isMine=false;
        bool isOpen=false;
        bool isFlagged=false;
        

    Tile(){
        Mine=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
        flagged=vector<vector<bool>>(NUMrow,vector<bool>(NUMcol,false));
    }

    void Yomikomi(){
        state = GameState::Playing;
        map<string,string> files={
            {"blue","resources/GroundBlue.jpg"},
            {"white","resources/GroundWhite.jpg"},
            {"mine","resources/Mine.jpg"},
            {"flag","resources/Flag2.png"},
            {"boom","resources/Boom.jpg"}
        };

            // テクスチャの読み込みとエラーチェック
        for(const auto& [key,path] : files){
            sf::Texture texture;
            if(!texture.loadFromFile(path)){
                cerr << "Error loading texture: " << path << endl;
                return;
            }
            textures.emplace(key, move(texture));
        }
    }
    int autoRelease(int x, int y,vector<vector<bool>>& opened) {    
        int OpenCount=1;    
        if(x<0||x>=NUMrow||y<0||y>=NUMcol){
            return 0; 
        }
        if(opened[x][y]){
            return 0;// すでに開いているかのチェック
        }
        opened[x][y] = true;
        if(field.MINE(x,y)){
            return 0;// 地雷があるかどうかのチェック
        }
        if(field.Count(x,y)!=0){
            return OpenCount;
        }
    
        int dx[8]= {-1,-1,-1,0,0,1,1,1};
        int dy[8]= {-1,0,1,-1,1,-1,0,1};

        for(int k=0;k<8;k++){
            int nx=x+dx[k];
            int ny=y+dy[k];
            if(nx>=0&&nx<NUMrow&&ny>=0&&ny<NUMcol){
                if(!opened[nx][ny]){
                    OpenCount+=autoRelease(nx, ny, opened);
                    //autoRelease(nx,ny,opened);おそらくいらないうまくいきそうなら消しとく
                }
            }
        }
        return OpenCount;
    }

    void display(){
        Yomikomi();
        field.minePlace();
        sf::RenderWindow window(sf::VideoMode(NUMrow * 50, NUMcol * 50), "MineSweeper");
        int tileSize = 50; 
        int TotalPlace=NUMrow*NUMcol;
        int SafePlace=TotalPlace-NUMmine;
        int OpenNumber=0;
        vector<vector<bool>> opened(NUMrow, vector<bool>(NUMcol, false));
        sf::Font font;
        if (!font.loadFromFile("resources/arial.ttf")) {
            cerr << "Error loading font" << endl;
            return;
        }
            
        while (window.isOpen()) {
        sf::Event event;        
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed){
                    window.close();
                }
                //左クリックの処理
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = mousePos.x / tileSize;
                    int y = mousePos.y / tileSize;
                    cout<< "Clicked at: (" << x << ", " << y << ")" << endl;

                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if(flagged[y][x]){
                            continue;
                        }else if(field.MINE(y,x)){
                            state = GameState::GameOver;
                            for(int i = 0; i < NUMrow; i++){
                                for(int j = 0; j < NUMcol; j++){
                                    opened[i][j] = true;
                                }
                            }
                        }else{
                            OpenNumber+=autoRelease(y, x, opened);
                            cout << "OpenNumber: " << OpenNumber << endl; // デバッグ用
                            if(SafePlace==OpenNumber){
                                state= GameState::Win;
                                for(int i = 0; i < NUMrow; i++){
                                    for(int j = 0; j < NUMcol; j++){
                                        opened[i][j] = true;
                                    }
                                }
                            }
                        }
                    }
                //右クリックの処理
                }else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Sprite sprite;
                    int x = mousePos.x / tileSize;
                    int y = mousePos.y / tileSize;
                    cout<< "Flagged at: (" << x << ", " << y << ")" << endl;
                    if (x >= 0 && x < NUMcol && y >= 0 && y < NUMrow) {
                        if(!opened[y][x]){ // まだ開いていないタイルに対してフラグを設定
                            flagged[y][x]=!flagged[y][x]; //フラグをつけると外す
                        }
                    }
                }
            }
            window.clear();
            for(int i=0;i<NUMrow;i++){
                for(int j=0;j<NUMcol;j++){
                    sf::Sprite sprite;
                    sf::RectangleShape tile(sf::Vector2f(tileSize - 2, tileSize - 2));
                    tile.setPosition(j * tileSize, i * tileSize);
                    tile.setFillColor( sf::Color(200,200,200));
                    window.draw(tile);

                    if(state==GameState::GameOver && field.MINE(i,j)){
                        sprite.setTexture(textures["boom"]);
                        sf::Vector2u textureSize = textures["boom"].getSize(); // 元の画像サイズ
                        float scaleX = tileSize / static_cast<float>(textureSize.x);
                        float scaleY = tileSize / static_cast<float>(textureSize.y);
                        sprite.setScale(scaleX, scaleY);
                        sprite.setPosition(j * tileSize, i * tileSize);
                        window.draw(sprite);
                        cout<< "Boom! You hit a mine at (" << i + 1 << ", " << j + 1 << ")." << endl; // デバッグ用
                    }else if(state==GameState::Win && !field.MINE(i,j)){
                                            sf::Text wintext;
                    wintext.setFont(font);
                    wintext.setString("You win!");
                    wintext.setCharacterSize(48);
                    wintext.setFillColor(sf::Color::Black);

                    sf::FloatRect textRect = wintext.getLocalBounds();
                    wintext.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                    wintext.setPosition(NUMcol * tileSize / 2.0f, NUMrow * tileSize / 2.0f);
                    window.draw(wintext);
                    }else if(opened[i][j]){
                        if(!field.MINE(i,j)){
                            //周囲の地雷数表示
                            int count = field.Count(i, j);
                            if(count >= 0){
                            sf::Text text;
                            text.setFont(font);
                            text.setString(to_string(field.Count(i,j)));
                            text.setCharacterSize(24);
                            text.setFillColor(sf::Color::Blue);
                            text.setPosition(j * tileSize + 15, i * tileSize + 10);
                            window.draw(text);
                            }
                        }
                    }else if(flagged[i][j]){
                        sprite.setTexture(textures["flag"]);
                        sf::Vector2u textureSize = textures["flag"].getSize(); // 元の画像サイズ
                        float scaleX = tileSize / static_cast<float>(textureSize.x);
                        float scaleY = tileSize / static_cast<float>(textureSize.y);
                        sprite.setScale(scaleX, scaleY);
                        sprite.setPosition(j * tileSize, i * tileSize);
                        window.draw(sprite);
                    }
                }
            }
                window.display();
        }
    }    
};
        //window.close();

        
        

    /*class Game{
        private:
        Field field;
        Tile tile;

        public:
        int start(int row,int col,int mine){
            char command;
            int x,y;
            int totalPlace=row*col;
            int safePlace=totalPlace-mine;
            int openNumber=0;
            int count;
            
            field.minePlace();
            //field.Board();
            tile.Yomikomi(); // タイルの読み込み
           // tile.display(); // タイルの表示

            while(1){
                cout<<"Please input a command (o/f) : ";
                cin>>command;
                if(command!='o'&&command!='f'){
                    cout<<"Please input 'o'or'f'"<<endl;
                    continue;
                }
                cout<<"Please input a row and a col : ";
                cin>>x>>y;
                x--;
                y--;
                if(x<0||x>=row||y<0||y>=col){
                    cout<<"Please input '1 ~ "<<NUMrow<<"'."<<endl;
                    continue;
                }
                if(command=='o'){
                    if(field.Fragged(x,y)){
                        cout<<"This place is already fragged."<<endl;
                        continue;
                    }else if(field.MINE(x,y)){
                        field.openMine(x,y);
                        cout<<"Boom You lost."<<endl;
                        break;
                    }else if(field.Opened(x,y)){//同じ座標を入力したときこのif文内にはいりcontinueで戻る
                        cout<<"This place is already opened."<<endl;
                        continue;
                    }else if(field.Count(x,y)==0){
                        openNumber+=field.AutoRelease(x,y);
                    }else{
                        field.Open(x,y);
                        field.ChangeCount(x,y);
                        openNumber++;
                    }
                }
                if(command=='f'){
                    if(field.Fragged(x,y)){
                        field.CancelFrag(x,y);
                    }else{
                        field.Frag(x,y);
                        field.ChangeFrag(x,y);
                    }
                }
                if(safePlace==openNumber){
                    field.openMine(x,y);
                    cout<<"Clear!"<<endl;
                    break;
                }
                field.Board();
                cout<<"("<<safePlace<<" , "<<openNumber<<")"<<endl;//デバック用
            }
            return 0;
        }
    };*/
    

    int main(){
        srand(time(0));
        Tile tile;
        tile.display(); // タイルの表示を開始
       // Game game;
       // game.start(NUMrow,NUMcol,NUMmine);

        return 0;
    }
