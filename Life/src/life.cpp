#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void printIntro();
char take_action_input();
void print_population(Grid<char> currentPop);
void update_population_from_file(Grid<char> &currentPop, string input_file_name);
int num_of_neigh(Grid<char> currentPop, int i, int j);
void _tick(Grid<char> &currentPop);
void process_user_input(Grid<char> &currentPop, char action_input);

int main() {
    printIntro();

    cout<< "Grid input file name? ";
    string input_file_name;
    getline(cin, input_file_name);

    Grid<char> currentPop;
    bool user_file_exists = fileExists(input_file_name);
    while(!user_file_exists){
        cout<<"Unable to open that file.  Try again.\n";
        cout<< "Grid input file name? ";
        getline(cin, input_file_name);
        user_file_exists = fileExists(input_file_name);
    }

    // file exists
    update_population_from_file(currentPop, input_file_name);
    print_population(currentPop);

    char user_ip = take_action_input();
    while(true){
        if(user_ip == 'q'){
            break;
        }else{
            process_user_input(currentPop, user_ip);
            user_ip = take_action_input();
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

void printIntro() {
    cout<< "Welcome to the CS 106B Game of Life,"<<endl;
    cout<< "a simulation of the lifecycle of a bacteria colony."<<endl;
    cout<< "Cells (X) live and die by the following rules:"<<endl;
    cout<< "- A cell with 1 or fewer neighbors dies."<<endl;
    cout<< "- Locations with 2 neighbors remain stable."<<endl;
    cout<< "- Locations with 3 neighbors will create life."<<endl;
    cout<< "- A cell with 4 or more neighbors dies."<<endl<<endl;

}

char take_action_input(){
    cout<<"a)nimate, t)ick, q)uit? ";
    string action_input;

    getline(cin, action_input);
    while( ! (action_input == "a" || action_input == "A" || action_input == "a" || action_input == "t" || action_input == "T" || action_input == "q" || action_input == "Q") ){
        cout<<"Invalid choice; please try again.\n";
        cout<<"a)nimate, t)ick, q)uit? ";
        getline(cin, action_input);
    }
    return tolower(action_input[0]);
}

void print_population(Grid<char> currentPop){
    int r = currentPop.numRows(), c = currentPop.numCols();
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            cout<<currentPop[i][j];
        }
        cout<<endl;
    }
}

void update_population_from_file(Grid<char> &currentPop, string input_file_name){
    // read file
    ifstream input_file_stream;
    if (openFile(input_file_stream, input_file_name)){ // file opened successfully
        string rows; getline( input_file_stream, rows );
        string cols; getline( input_file_stream, cols );
        int _rows = stringToInteger(rows), _cols = stringToInteger(cols);
        currentPop.resize( _rows, _cols);
        // copy stuff
        for(int i=0; i<_rows; i++){
            string ith_row; getline( input_file_stream, ith_row);
            for(int j=0; j<_cols; j++){
                currentPop[i][j] = ith_row[j];
            }
    }
        input_file_stream.close();
    }

}

int num_of_neigh(Grid<char> currentPop, int i, int j){
    int r = currentPop.numRows(), c = currentPop.numCols();
    int count = 0;
    int dx[] = {-1, -1, -1, 0, +1, +1, +1, 0};
    int dy[] = {-1, 0, +1, +1, +1, 0, -1, -1};
    for(int a=0; a<8; a++){
            int tx = i+dx[a], ty = j+dy[a];
            if( (-1<tx && tx<r) && (-1<ty && ty<c)){
                if(currentPop[tx][ty] == 'X') // living neighbour
                    count++;
            }
    }
    return count;
}

void _tick(Grid<char> &currentPop){
    // computes + prints next gen
    int r = currentPop.numRows(), c = currentPop.numCols();
    char temp_grid[r][c];

    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
           temp_grid[i][j] = currentPop[i][j];
        }
    }

    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
                int neigh = num_of_neigh(currentPop, i, j);
                if(neigh <= 1){ // dies in next gen
                    temp_grid[i][j] = '-';
                }else if(neigh == 2){ // stable
                    // no change
                }else if(neigh == 3){
                    temp_grid[i][j] = 'X';
                }else if(neigh >= 4){
                    temp_grid[i][j] = '-';
                }
        }
    }
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            currentPop[i][j] = temp_grid[i][j];
        }
    }
    print_population(currentPop);
}

void process_user_input(Grid<char> &currentPop, char action_input){
    if(action_input == 'a' || action_input == 'A'){
        string frames;
        cout<<"How many frames? ";
        getline(cin, frames);
        while(!stringIsReal(frames)){
            cout<<"Illegal integer format. Try again.\n";
            cout<<"How many frames? ";
            getline(cin, frames);
        }
        int _frames = stringToInteger(frames);
        for(int i=0; i<_frames; i++){
            clearConsole();
            _tick(currentPop);
            pause(50);

        }
    }
    else if(action_input == 't' || action_input == 'T'){
        _tick(currentPop);
    }
}



