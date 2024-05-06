#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <windows.h>

#define redBg "\033[30;41m"
#define greenBg "\033[30;42m"
#define resetColor "\033[0m"
#define dGrayBg "\033[30;100m"
#define lGrayBg "\033[30;47m"

using namespace std;

ifstream in("options.txt");

const int tab_count = 1;

int board[51][51];
short board_vision[51][51];
int board_size;
int mine_count;
int curr_i=-1, curr_j=-1;
bool won;
bool lost;
POINT pt;
int iTL, jTL, iBR, jBR;
double iUNIT, jUNIT;
bool areMinesGenerated;
// GetCursorPos(&pt); - pune pozitia curenta a mouse-ului fata de coltul din stg sus in pt

void tabs(){
    for(int i = 1; i<=tab_count; i++){cout << '\t';};
}

void readData(){
    in >> board_size;
    in >> mine_count;
}

bool generateMines(int ii=0, int jj=0){
    int current_mine_count = 0, it = 0;
    while(current_mine_count<mine_count && it<=1000000){
        it++;
        int i,j;
        i = rand()%board_size;
        j = rand()%board_size;
        if(board[i][j]==0 && !(i==ii && j==jj)){
            board[i][j]=-1;
            current_mine_count++;
        }
    }
    if(it==1000000 || current_mine_count!=mine_count) return 0;
    return 1;
}

void showBoard(){
    system("CLS");
    cout << resetColor;
    cout << '\n';
    for(int i = 0; i<board_size; i++){cout << resetColor;
        cout << resetColor;
        tabs();
        for(int j = 0; j<board_size; j++){
            if((i+j)%2==1) cout << lGrayBg;
            else cout << dGrayBg;
            if(board_vision[i][j]||won){
                if(board_vision[i][j]==2) cout << ' '<< "F ";
                else if(board[i][j]==-1) cout <<redBg<< ' '<< "X ";
                else if(board[i][j]==0) cout << resetColor << "   ";
                else cout <<resetColor<< ' ' << board[i][j] << ' ' ;
            }
            else cout << "   ";
        }
        cout << '\n';
    }
    cout << resetColor << '\n';
    cout.flush();
}

void calibrate(){
    showBoard();
    tabs();
    cout << "Starting Calibration, press ENTER when you are ready. ";
    cin.get();
    showBoard();
    cout << "\tPlace your cursor in the TOP LEFT corner of the board and press ENTER. ";
    cin.get();
    GetCursorPos(&pt);
    iTL=pt.y;
    jTL=pt.x;

    cout << "\tPlace your cursor in the BOTTOM RIGHT corner of the board and press ENTER. ";
    cin.get();
    GetCursorPos(&pt);
    iBR=pt.y;
    jBR=pt.x;
    iUNIT = (iBR-iTL)/(board_size*1.0f);
    jUNIT = (jBR-jTL)/(board_size*1.0f);
    showBoard();
}

void calculateMines(){
    for(int i = 0; i<board_size; i++)
        for(int j = 0; j<board_size; j++)
            if(board[i][j]==0)
                for(int ii = i-1; ii<=i+1; ii++)
                    for(int jj = j-1; jj<=j+1; jj++)
                        board[i][j]+=(board[ii][jj]==-1 && ii>=0 && jj>=0 && ii<board_size && jj<board_size);
}

bool winCheck(){
    int ct = 0;
    for(int i = 0; i<board_size; i++)
        for(int j = 0; j<board_size; j++)
            if(board_vision[i][j]==0) ct++;
    if(ct<=mine_count) return 1;
    return 0;
}

void fillPos(int i, int j, int last=0){
    if((board[i][j]==0 || last==0) && i>=0&& j>=0 && i<board_size && j<board_size && board_vision[i][j]==0){
        board_vision[i][j]=1;
        fillPos(i+1,j,board[i][j]);
        fillPos(i-1,j,board[i][j]);
        fillPos(i,j+1,board[i][j]);
        fillPos(i,j-1,board[i][j]);
    }
}

void getCurrCoords(){
    GetCursorPos(&pt);
    pt.x-=jTL;
    pt.y-=iTL;
    pt.x/=jUNIT;
    pt.y/=iUNIT;
    curr_i=pt.y;
    curr_j=pt.x;
}

void Flag(){
    getCurrCoords();
    board_vision[curr_i][curr_j]=2;
}

void Click(){
    getCurrCoords();
    if(areMinesGenerated==0)return;
    board_vision[curr_i][curr_j]=0;
    if(board[curr_i][curr_j]==-1){
            board_vision[curr_i][curr_j]=1;
        lost = 1;
    }
    fillPos(curr_i,curr_j);
    won = winCheck();
}

void getOption(){
    showBoard();
    char opt[1001];
    tabs();
    cout << "c - Start calibration\n";
    tabs();
    cout << "f or ' - Place flag\n";
    tabs();
    cout << "ENTER - Click\n";
    cin.getline(opt,1000);
    if(opt[0]=='c') calibrate();
    else if(opt[0]=='f' || opt[0]=='\'') Flag();
    else Click();
}

int main()
{

    readData();
    srand((unsigned) time(NULL)); // seteaza seedul la rand
    showBoard();
    calibrate();

    if(mine_count > board_size*board_size){
        cout << "error: Too many mines\n";
        return -1;
    }

    while(curr_i==-1 || curr_i>=board_size || curr_j>=board_size || curr_i<0 || curr_j<0) getOption();
    if(generateMines(curr_i,curr_j)==0){
        cout << "error: Mines take too long to place\n";
        return -1;
    }
    areMinesGenerated=1;

    calculateMines();
    fillPos(curr_i,curr_j);
    showBoard();
    won = winCheck();

    while(!won && !lost){
        getOption();
        if(lost) break;
        showBoard();
        if(won) break;
    }

    showBoard();
    cout << '\n';
    tabs();
    if(lost==1) cout << '\t' << redBg << "You lost!" << resetColor;
    else cout << '\t'<< greenBg  << "You won!" << resetColor;
    cout << "\n\n";

    return 0;
}
