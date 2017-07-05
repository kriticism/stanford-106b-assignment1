#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"
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

    friend ostream& operator<<(ostream& out, Boggle& boggle);

    ////////////////////////////
    string getText(); // Getter for boardText variable
    Set<string> getWords(); // Getter for wordsFounds variable
    void addScoreComputer(int n); // Updater for computerScore variable
    void addScoreHuman(int n); // Updater for humanScore variable

private:
    Lexicon dictionary;
    Grid<char> board; // Grid<char> is used to store the board
    Set<string> wordsFound; // Set<string> is used to store the words found by the human
    string boardText; /* String is used as the boardText that is either randomly generated
       or created by user. It is used to populate the Grid<char> */
    int humanScore;
	  int computerScore;
    bool _DFSRecurse(string word, Grid<bool> & visited, int r, int c); // Helper function for humanWordSearch()
    void computerHelper(Set<string> & result, Grid<bool> & checked, string word, int r, int c); // Helper function for computerWordSearch()
    string getRandomBoard(); // Generates a random boardText from the CUBE arrays
	 void setBoardText(); // Populates board with boardText
};

#endif // _boggle_h
