/*
* Author: Kriti Singh | ks2259@cornell.edu
* Date: May 29, 2017
* Towards assignment for Stanford's CS106B as a preparatory course.
*/
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "stack.h"
#include "lexicon.h"
#include "simpio.h"
#include "queue.h"
#include "filelib.h"


using namespace std;

void print_intro();
void run_word_ladder();
string read_user_word_by_sequence(int seq);
Lexicon openDictionary();
bool are_valid_words(string w1, string w2, Lexicon &dic);
void print_ladder(Stack<string> &soln, string w1, string w2);
void find_shortest_ladder(string w1, string w2, Lexicon &dic);


int main() {
    print_intro();
    run_word_ladder();
    cout << "Have a nice day." << endl;
    return 0;
}

void print_intro(){
    cout<< "Welcome to CS 106B Word Ladder.\n";
    cout<< "Please give me two English words, and I will change the\n";
    cout<< "first into the second by changing one letter at a time.\n\n";
}

void run_word_ladder() {
    Lexicon dictionary = openDictionary(); // new lexicon from a txt file
    while (true) {
        cout<<endl;
        string word1 = read_user_word_by_sequence(1);
        if (word1 == "") return;
        string word2 = read_user_word_by_sequence(2);
        if (word2 == "") return;
        if (are_valid_words(word1, word2, dictionary)) {
            find_shortest_ladder(word1, word2, dictionary);
        }
    }
}


// create Lexicon data structure.
Lexicon openDictionary() {
    ifstream input;
    Lexicon lexicon_ds;
    while (true) {
        string filename = getLine("Dictionary file name? ");
        openFile(input, filename);
        if (input.is_open()) {
            lexicon_ds.addWordsFromFile(filename);
            break;
        } else {
            cout << "File does not exist. Try again." << endl;
        }
    }
    return lexicon_ds;
}

string read_user_word_by_sequence(int seq) {
    cout<<"Word #"<<seq<<" (or Enter to quit): ";
    string w = getLine();
    return toLowerCase(w);
}


bool are_valid_words(string w1, string w2, Lexicon &dic) {
    if (w1 == w2) {
        cout << "The two words must be different." << endl;
        return false;
    }
    if (!dic.contains(w1) || !dic.contains(w2)) {
        cout << "The two words must be found in the dictionary." << endl;
        return false;
    }
    if (w1.length() != w2.length()) {
        cout << "The two words must be the same length." << endl;
        return false;
    }
    return true;
}


void find_shortest_ladder(string w1, string w2, Lexicon &dic) {
    Queue<Stack<string> > word_ladders; //empty queue of stacks
    Stack<string> start;
    start.push(w1);
    word_ladders.enqueue(start);

    Set<string> visited_words; // used words so that they are not reused
    visited_words.add(w1);

    while (!word_ladders.isEmpty()) {
        Stack<string> partial_ladder = word_ladders.dequeue(); // Dequeue the partial-ladder stack from the front of the queue
        string top_word = partial_ladder.peek(); // get ToS
        for (int i = 0; i < (int) top_word.length(); i++) { // Iterate through each letter of the word
            string all_letters = "abcdefghijklmnopqrstuvwxyz";
            string temp_top_word = top_word;
            for (int j = 0; j < (int) all_letters.length(); j++) {
                if (top_word[i] != all_letters[j]) {
                    temp_top_word[i] = all_letters[j]; // if letter different from original letter, replace

                    if (temp_top_word == w2) { // soln found, print stack, return
                        partial_ladder.push(temp_top_word);
                        print_ladder(partial_ladder, w1, w2);
                        visited_words.clear();
                        return;
                    }
                    if (dic.contains(temp_top_word) && !visited_words.contains(temp_top_word)) {
                        Stack<string> temp_partial_stack = partial_ladder;
                        temp_partial_stack.push(temp_top_word); // Add temp_top_word to partial_ladder
                        word_ladders.enqueue(temp_partial_stack); // Add partial_ladder to queue
                        visited_words.add(temp_top_word);
                    }
                }
            }
        }
    }
    cout << "No word ladder found from "<<w2<<" back to "<<w1<<"." << endl;
}

void print_ladder(Stack<string> &soln, string w1, string w2) {
    cout << "A ladder from " << w2 << " back to " << w1 << ":" << endl;
    while (!soln.isEmpty()) {
        cout << soln.pop() << " ";
    }
    cout << "\n";
}
