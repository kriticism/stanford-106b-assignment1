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
#include "bogglegui.h"
#include "shuffle.h"
#include "random.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

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
    // initialize gui here
    BoggleGUI::initialize(BOARD_SIDE_LEN, BOARD_SIDE_LEN);

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
    // cout<<"\nAfter initialization, board is:\n";
    printBoard();
    BoggleGUI::labelAllCubes(getBoardString());
}

void Boggle::shuffleBoard(){
    shuffle(cubeNumber);
    for(int i=0; i<16; i++){
        board[(i/4)][(i%4)] = CUBES[cubeNumber[i]][randomInteger(0, 5)];
    }

}

char Boggle::getLetter(int row, int col) {
    return board[row][col];
}

/*
 * Checks if a a given string (word) exists on board
 */
bool Boggle::checkWord(string word) {
    // TODO: implement
    bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN];
    for(int i=0; i<BOARD_SIDE_LEN; i++) for(int j=0; j<BOARD_SIDE_LEN; j++) visited[i][j] = false;

    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            if(board[i][j] == word[0]){
                cout<<"\n initiating search for "<<word<<" at "<<i<<j;
                visited[i][j] = true;
                // BoggleGUI::setHighlighted(i, j, true);
                bool retVal = doDFS(i,j, word, visited, 1);
                visited[i][j] = false;
                // BoggleGUI::setHighlighted(i, j, false);

                if(retVal){
                    cout<<endl<<word<<" found at "<<i<<j;
                    cout<<"You found a new word \""<<word<<"\"!";
                    return true;
                }
            }
        }
    }
    cout<<endl<<word<<" not found on board";
    return false;
}

/*
 * Checks if the string word entered by human exists on the board or not.
*/
bool Boggle::humanWordSearch(string word) {
    // return false if word not in dictionary
    if(!gameDict.contains(word)){
        cout<<endl<<word<<": is not in dictionary.";
        return false;
    }
    return checkWord(word);
}

int Boggle::getScoreHuman() {
    int score = 0;
    score = humanWords.size();
    return score;
}

/*
 * at least 4 letters long
 * is a word found inthe English dictionary
 * can be formed by connecting neighboring letter cubes (using any given cube only once)
 * has not already been formed by the player in this game (even if there are multiple paths on the board to form the same word, the word is counted at most once)
*/
Set<string> Boggle::computerWordSearch() {

    Set<string> result;
    bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN];
    for(int i=0; i<BOARD_SIDE_LEN; i++) for(int j=0; j<BOARD_SIDE_LEN; j++) visited[i][j] = false;

    for(int i=0; i<BOARD_SIDE_LEN; i++){
        for(int j=0; j<BOARD_SIDE_LEN; j++){
            visited[i][j] = true;
            BoggleGUI::setHighlighted(i, j, true);
            doDFSAndAddWords(i,j,string(1, board[i][j]), visited, result);
            BoggleGUI::setHighlighted(i, j, false);
            visited[i][j] = false;
        }
    }

    result.removeAll(humanWords);             // remove all the human-found words now
    return result;
}

int Boggle::getScoreComputer() {
    int score = 0;
    score = computerWords.size();
    return score;
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
bool Boggle::doDFS(int i, int j, string word, bool (&visited)[BOARD_SIDE_LEN][BOARD_SIDE_LEN], int idx ){
    if(idx == (int) word.size()){
        cout<<"\n Full word "<<word<<" matched";
        return true;
    }

    for(int k=0; k<8; k++){
        int _i = i+dr[k], _j = j+dc[k];

        if(isInBounds(_i, _j) && !visited[_i][_j] && board[_i][_j] == word[idx]){
            if(word == "SWAM")
                cout<<endl<<"Checking "<<_i<<_j;
            visited[_i][_j] = true;
            bool retVal = doDFS(_i, _j, word, visited, idx+1);
            visited[_i][_j] = false;
            if(retVal){
                cout<<"\n word found during dfs";
                return true;
            }
        }
    }

    return false;
}


void Boggle::doDFSAndAddWords(int i, int j, string strTillHere, bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN], Set<string> &result){
    if(strTillHere.size() >=4 && gameDict.contains(strTillHere))
        result.add(strTillHere);

    if(gameDict.containsPrefix(strTillHere)){ // there is no word having prefix strTillHere return
        for(int k=0; k<8; k++){
            int _i = i+dr[k], _j = j+dc[k];
            if(isInBounds(_i, _j)){
                if(!visited[_i][_j] ){
                    visited[_i][_j] = true;
                    doDFSAndAddWords(_i, _j, strTillHere+board[_i][_j], visited, result);
                    visited[_i][_j] = false;
                }
            }
        }
    }

    return;
}

Vector<string> Boggle::splitStringToWords(string text, char sep) {
    Vector<string> tokens;
        size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != string::npos) {
            if (end != start) {
              tokens.push_back(text.substr(start, end - start));
            }
            start = end + 1;
        }
        if (end != start) {
           tokens.push_back(text.substr(start));
        }
        return tokens;
}

string Boggle::printHumanWords(){
    return humanWords.toString();
}

string Boggle::printComputerWords(){
    return computerWords.toString();
}

void Boggle::addWordsToHumanList(Vector<string> validWords){
    for (Vector<string>::iterator it = validWords.begin() ; it != validWords.end(); ++it){
        humanWords.add(*it);
        BoggleGUI::recordWord(*it, BoggleGUI::HUMAN);
        BoggleGUI::setScore(getScoreHuman(), BoggleGUI::HUMAN);
    }
}

void Boggle::addWordsToComputerList(Set<string> validWords){
    for (Set<string>::iterator it = validWords.begin() ; it != validWords.end(); ++it){
        computerWords.add(*it);
        BoggleGUI::recordWord(*it, BoggleGUI::COMPUTER);
        BoggleGUI::setScore(getScoreComputer(), BoggleGUI::COMPUTER);
    }
}

string Boggle::getBoardString(){
    string boardString = "";
    for(int i=0; i<BOARD_SIDE_LEN; i++) for(int j=0; j<BOARD_SIDE_LEN; j++) boardString += board[i][j];
    return boardString;
}
