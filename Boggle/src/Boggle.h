// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header
/*
  What functions are available for boggle objects

declare private instance variables here

No code only function prototypes

Don’t change public functions
*/

////////////////////////////////////////////////////////////////////////////

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "vector.h"


#define BOARD_SIDE_LEN 4

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    void shuffleBoard();
    void printBoard();
    bool isInBounds(int _i, int _j);
    bool doDFS(int i, int j, string word, bool visited[BOARD_SIDE_LEN][BOARD_SIDE_LEN], int idx);
    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    char board[4][4];
    Vector<int> cubeNumber;
    Lexicon gameDict;
    // bool dfsSearch(int r, int c, string word);
    // vector<int>v = {1, 3, 5, 7};
    // TODO: add any other member functions/variables necessary
};

#endif // _boggle_h
