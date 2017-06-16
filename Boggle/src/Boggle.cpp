// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

/*
 *  implement
 */

////////////////////////////////////////////////////////////////////////////

#include "Boggle.h"
#include "shuffle.h"
#include "random.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

static int dr[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
static int dc[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    gameDict = dictionary;
    if(boardText == ""){                 // case of random letters on board
        for(int i=0; i<16; i++)
            cubeNumber.push_back(i);
        shuffleBoard();
    }
    else{                                // case of custom string on board
        for(int i=0; i<16; i++){
            board[(i/4)][(i%4)] = boardText[i];
        }
    }
    cout<<"\nAfter initialization, board is:\n";
    printBoard();
}

void Boggle::shuffleBoard(){
    shuffle(cubeNumber);
    for(int i=0; i<16; i++){
        board[(i/4)][(i%4)] = CUBES[cubeNumber[i]][randomInteger(0, 5)];
    }

}

char Boggle::getLetter(int row, int col) {
    return board[row][col];
    // return '?';   // remove this
}

/*
 * Checks if a a given string (word) exists on board
 */
bool Boggle::checkWord(string word) {
    // TODO: implement
    bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN];
    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            visited[i][j] = false;
        }
    }

    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            if(board[i][j] == word[0])
                if(doDFS(i,j, word, visited, 1))
                    return true;
        }
    }
    return false;
}

/*
 * Checks if the string word entered by human exists on the board or not.
*/
bool Boggle::humanWordSearch(string word) {
    // return false if not in dictionary
    if(!gameDict.contains(word))
        return false;
    // else check if it is on board too
    bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN];
    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            visited[i][j] = false;
        }
    }

    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            if(board[i][j] == word[0]){
                if( doDFS(i,j,word,visited,1) ){
                    return true;
                }
            }
        }
    }
    return false;
}

int Boggle::getScoreHuman() {
    // TODO: implement
    return 0;   // remove this
}

Set<string> Boggle::computerWordSearch() {
    // TODO: implement
    Set<string> result;   // remove this
    return result;        // remove this
}

int Boggle::getScoreComputer() {
    // TODO: implement
    return 0;   // remove this
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    // TODO: implement
    return out;
}



void Boggle::printBoard(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
      cout<<endl;
}


bool Boggle::isInBounds(int _i, int _j){
    return(0<=_i && _i<BOARD_SIDE_LEN && 0<_j && _j< BOARD_SIDE_LEN);
}

/*
 * Continue DFSing from index i,j, in a bid to match the index idx of the string word on the board.
 * If a match is found, return true.
*/
bool Boggle::doDFS(int i, int j, string word, bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN], int idx ){
    if(idx == word.size()){
        cout<<"\n Full word matched";
        return true;
    }

    for(int k=0; k<8; k++){
        int _i = i+dr[k], _j = j+dc[k];
        if(isInBounds(_i, _j)){
            if(!visited[_i][_j] && board[_i][_j] == word[idx]){
                visited[_i][_j] = true;
                bool retVal = doDFS(_i, _j, word, visited, idx+1);
                if(retVal){
                    cout<<"\n word found during dfs";
                    return true;
                }
                visited[_i][_j] = false;
            }
        }
    }

    return false; // TODO write when this happens
}
