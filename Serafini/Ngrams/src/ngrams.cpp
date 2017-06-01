/*
* Author: Kriti Singh | ks2259@cornell.edu
* Date: May 30, 2017
* Towards assignment for Stanford's CS106B as a preparatory course.
*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "hashset.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

void print_intro();
void read_file_and_create_n_grams(HashMap<Vector<string>,Vector<string> > &N_gram_map, ifstream &input, int N);

int main() {
    print_intro();
    string input_file_name = getLine("Input file name? ");
    ifstream input; input.open(input_file_name.c_str());
    while(input.fail()){
        cout << "Unable to open that file.  Try again.";
        input_file_name = getLine("\nInput file name? ");
        input.open(input_file_name.c_str());
    }
    string input_str_for_N;
    input_str_for_N = getLine("Value of N? ");
    int N;
    while(true){
        if(!stringIsReal(input_str_for_N)){ // not integer
            cout<<"Illegal integer format. Try again.";
            input_str_for_N = getLine("\nValue of N? ");
        }else{ // integer
            N = stringToInteger(input_str_for_N);
            if(N<2){
                cout<<"N must be 2 or greater.";
            }else
                break;
            input_str_for_N = getLine("\nValue of N? ");
        }
    }

    HashMap<Vector<string>,Vector<string> > N_gram_map;


    read_file_and_create_n_grams(N_gram_map, input, N);
    int num_of_words_to_generate;
    string str_for_num_of_words_to_generate;
    while (true){
        str_for_num_of_words_to_generate = getLine("\n# of random words to generate (0 to quit)? ");

        if(!stringIsReal(str_for_num_of_words_to_generate)){ // not integer
            cout<<"Illegal integer format. Try again.";
         }
        else { // valid integer input
            num_of_words_to_generate = stringToInteger(str_for_num_of_words_to_generate);
            if (num_of_words_to_generate == 0){ // stop
                cout << "Exiting." << endl;
                break;
            }
            else if(num_of_words_to_generate<N){ // words less than N
                cout<<"Must be at least "<<N<<" words.";
            }
            else{ // print non zero words
                cout << endl;
                cout << "... ";
                    Vector<string> window = N_gram_map.keys()[randomInteger(0,N_gram_map.size()-1)]; // randomly chosen key
                    for (int i = 0; i < num_of_words_to_generate; i++){
                        cout << window[0] << " ";
                        string temp = N_gram_map[window][randomInteger(0,N_gram_map[window].size()-1)];
                        for (int j = 0; j < window.size()-1; j++)
                            window[j] = window[j+1];
                        window[window.size()-1] = temp;
                    }
                cout << "..." << endl;
            }
        }
    }
    return 0;
}

void print_intro(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl<<endl;
}

void read_file_and_create_n_grams(HashMap<Vector<string>,Vector<string> > &N_gram_map, ifstream &input, int N){
    Vector<string> window;
    string line;
    Vector<string> front_word;
    while(window.size() < N-1){
        input >> line;
        window.add(line);
    }
    front_word = window;
    while (true){
        input >> line;
        if (input.fail()) break;
        N_gram_map[window].add(line);
        // update window
        for (int i = 0; i < window.size()-1; i++) // shift back
            window[i] = window[i+1];
        window[window.size()-1] = line; // append new
    }
    // wraparound
    for (int i = 0; i < front_word.size(); i++){
        N_gram_map[window].add(front_word[i]);
        for (int i = 0; i < window.size()-1; i++)
            window[i] = window[i+1];
        window[window.size()-1] = front_word[i];
    }
}
