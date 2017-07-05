#include "Boggle.h"
#include "bogglegui.h"
#include "shuffle.h"
#include "strlib.h"

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

Boggle::Boggle(Lexicon& dictionary, string boardText) : board(4, 4) {
    if (boardText.empty())  // random board if boardText is empty
      boardText = getRandomBoard();
    this->boardText = boardText;
    setBoardText(); // Populating the board grid
    this->dictionary = dictionary;
    humanScore = 0;
    computerScore = 0;
}

string Boggle::getRandomBoard() {
    string boardText = "";
    shuffle(CUBES, 16);
    for (string cube : CUBES) {
        boardText += cube[randomInteger(0,5)];
    }
    return toUpperCase(boardText);
}

void Boggle::setBoardText() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int index = j + (4 * i);
            board[i][j] = boardText[index];
		}
	}
}

char Boggle::getLetter(int row, int col) {
	if (board.inBounds(row, col)) {
        return board.get(row, col);
    }
    else{
        error("Boggle::getLetter-- index out of bounds."); // throw 20;
        return EOF;
    }

}

bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if (!wordsFound.contains(word) && dictionary.contains(word) && word.length() >= 4 && word.length() <= 16)
        return true;
	else
    return false;
}

bool Boggle::_DFSRecurse(string word, Grid<bool> & visited, int r, int c) {
    if (word.empty())
      return true;

    for (int dr = -1; dr < 2; dr++) { // adj cubes
        for (int dc = -1; dc < 2; dc++) {
            if (board.inBounds(r+dr,c+dc) && board[r+dr][c+dc] == word[0] && visited[r+dr][c+dc] != 1) {
                visited[r+dr][c+dc] = true; // visit
                  BoggleGUI::setAnimationDelay(100);
                  BoggleGUI::setHighlighted(r+dr, c+dc, true);
                  if (_DFSRecurse(word.substr(1), visited, r+dr, c+dc))
                    return true;
                visited[r+dr][c+dc] = false; // backtrack
                BoggleGUI::setHighlighted(r+dr, c+dc, false);
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearch(string word) {
    Grid<bool> visited(4, 4);
    visited.fill(false);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (board[r][c] == word[0]) {
                if (_DFSRecurse(word, visited, r, c)) {
                    wordsFound.add(word);
                    return true;
                }
            }
        }
    }
    return false;
}

Set<string> Boggle::getWords() {
    return wordsFound;
}

int Boggle::getScoreHuman() {
    return humanScore;
}

void Boggle::addScoreHuman(int n) {
    humanScore += n;
}

void Boggle::computerHelper(Set<string> & result, Grid<bool> & visited, string word, int r, int c) {
    if (dictionary.contains(word) &&
            !wordsFound.contains(word) &&
            word.length() > 3)
        result.add(word);

    if (dictionary.containsPrefix(word)) {
        for (int dr = -1; dr < 2; dr++) {
            for (int dc = -1; dc < 2; dc++) {
                if (board.inBounds(r+dr,c+dc) && visited[r+dr][c+dc] != 1) {
                    visited[r+dr][c+dc] = true; // visit
                      word += board[r+dr][c+dc];
                      computerHelper(result, visited, word, r+dr, c+dc);
                    visited[r+dr][c+dc] = false; // backtrack
                    word.pop_back();
                }
            }
        }
    }
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    string word;
    Grid<bool> visited(4, 4);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            word += board[r][c]; // visit
            visited[r][c] = true;
              computerHelper(result, visited, word, r, c);
            visited[r][c] = false; // backtrack
            word.pop_back();
        }
    }
    return result;
}

int Boggle::getScoreComputer() {
    return computerScore;
}

void Boggle::addScoreComputer(int n) {
    computerScore += n;
}

string Boggle::getText() {
    return boardText;
}

ostream& operator<<(ostream& out, Boggle & boggle) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            out << boggle.boardText[c + 4 * r];
        }
        out << endl;
    }
    return out;
}
