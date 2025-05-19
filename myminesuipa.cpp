#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<iomanip>

#define NUMrow 5
#define NUMcol 5
#define NUMmine 3
using namespace std;

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
    void Board(){
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
    }
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

    class Game{
        private:
        Field field;

        public:
        int start(int row,int col,int mine){
            char command;
            int x,y;
            int totalPlace=row*col;
            int safePlace=totalPlace-mine;
            int openNumber=0;
            int count;
            
            field.minePlace();
            field.Board();

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
    };

    int main(){
        srand(time(0));
        Game game;
        game.start(NUMrow,NUMcol,NUMmine);

        return 0;
    }
