#include "lexicon.h"
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglegui.h"

using namespace std;

bool checkInput(string boardText) {
    if (boardText.size() != 16) return false;
	for (char c : boardText) {
        if (!isalpha(c)) return false;
	}
	return true;
}

string inputForBoard() {
    string boardText;
	while (true) {
        boardText = getLine("Type the 16 letters to appear on the board: ");
		if (checkInput(boardText)) break;
		else cout << "That is not a valid 16-letter board string. Try again." << endl;
	}
    // Always return an uppercase board text as a standard
	return toUpperCase(boardText);
}

void printRoundDetails(string prompt, Boggle & myBoard) {
	clearConsole();
	cout << prompt << endl;
    cout << myBoard << endl;
    cout << "Your words (" << myBoard.getWords().size() << "): ";
    cout << myBoard.getWords() << endl;
    cout << "Your score: " << myBoard.getScoreHuman() << endl;
}

string playOneRound(Boggle & myBoard) {
    // Once the next word is typed in, clear the highlighting from the previoues
    string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
    BoggleGUI::clearHighlighting();

    // Case the input is just "Enter", break
    if (word.empty()) {
        return "";
    }
    // Make sure the word is "legal"
    else if (!myBoard.checkWord(word)) {
        return "You must enter an unfound 4+ letter word from the dictionary.";
    }
    // If word can be found, update score and GUI
    else if (myBoard.humanWordSearch(word)) {
        myBoard.addScoreHuman(word.length() - 3);
        BoggleGUI::setScore(word.length() - 3, BoggleGUI::HUMAN);
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
		return "You have found a new word! \"" + word + "\"";
    }
    // Else, return that the word can't be found
    else {
        return "That word can't be formed on this board.";
    }
}

void computerUpdate(Boggle & myBoard, Set<string> result) {
    for (string word : result) {
        myBoard.addScoreComputer(word.length() - 3);
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(myBoard.getScoreComputer(), BoggleGUI::COMPUTER);

    cout << "My words (" << result.size() << "): " << result << endl;
    cout << "My score: " << myBoard.getScoreComputer() << endl;
}

void computerRound(Boggle & myBoard) {
    cout << endl << "It's my turn!" << endl;
    // "Computer" finds words left and then callss computerUpdate()
    Set<string> computerWords = myBoard.computerWordSearch();
    computerUpdate(myBoard, computerWords);
}

void declareWinner(Boggle & myBoard) {
    if (myBoard.getScoreComputer() > myBoard.getScoreHuman()) {
        string result = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        cout << result << endl;
        BoggleGUI::setStatusMessage(result);
    } else {
        string result = "WOW, you defeated me! Congratulations!";
        cout << result  << endl;
        BoggleGUI::setStatusMessage(result);
    }
}

void playOneGame(Lexicon& dictionary) {
    string boardText = getYesOrNo("Do you want to generate a random board? ") ? "" : inputForBoard();
    Boggle myBoard(dictionary, boardText);
  	string prompt = "It's your turn!";

      // Setting up the GUI
      BoggleGUI::initialize(4, 4);
      BoggleGUI::labelAllCubes(myBoard.getText());

      // Infinite Round Loop
  	while (true) {
          BoggleGUI::setStatusMessage(prompt);
          printRoundDetails(prompt, myBoard);
  		prompt = playOneRound(myBoard);
  		if (prompt == "") break;
  	}

      computerRound(myBoard);
      declareWinner(myBoard);
}
