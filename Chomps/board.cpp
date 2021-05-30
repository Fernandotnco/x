#include"board.h"
#include<stdexcept>
#include<iostream>

using std::cout;
using std::endl;

Board::Board(int w, int h){

    _allocated = false;
    if(w <= 0 || h <= 0){
        throw std::invalid_argument("The size of the board must be greater than 0");
    }

    _width = w;
    _height = h;
    _remainingCookies = w*h;

    //creating and allocating the board
    //Allocating the columns
    try{
        _board = new bool*[w];
    }
    catch(std::bad_alloc& ba){
        cout << "Failed to create the board: couldn't create columns.\n" << ba.what()  << endl;
        throw;
        return;
    }
    //allocating each line
    for(int i = 0; i < w; i++){
        try{
            _board[i] = new bool[h];
        }
        catch(std::bad_alloc& ba){
            cout << "Failed to create the board: couldn't create line " << i << endl << ba.what()  << endl;
            throw; 
            return;           
        }
    }

    //defining true for every element of the board
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            _board[i][j] = true;
        }
    }

    _allocated = true;
}

void Board::ChooseCookie(int w, int h){
    if(_allocated == false){
        throw std::out_of_range("The board isn't allocated correctly");
    }
    if(w < 0 || w >= _width || h < 0 || h >= _height){
        cout << "Trying to choose cookie (" << w << ", "<< h << "), but the board is of size (" << _width << ", "<< _height << ")" << endl; 
        throw std::invalid_argument("Invalid position on the board");
    }
    for(int i = w; i < _width; i++){
        for(int j = h; j < _height; j++){
            cout << "Eating cookie(" << i << ", "<< j << ")" << endl;;
            _board[i][j] = false;
            _remainingCookies --;
        }
    }
}

int Board::RemainingCookies()const{
    return _remainingCookies;
}

bool Board::CheckPosition(int w, int h)const{
    if(_allocated == false){
        throw std::out_of_range("The board isn't allocated correctly");
    }
    if(w < 0 || w >= _width || h < 0 || h >= _height){
        cout << "Trying to choose cookie (" << w << ", "<< h << "), but the board is of size (" << _width << ", "<< _height << ")" << endl; 
        throw std::invalid_argument("Invalid position on the board");
    }

    return _board[w][h];
}

void Board::PrintBoard()const{
    if(_allocated == false){
        throw std::out_of_range("The board isn't allocated correctly");
    }

    for(int i = 0; i < _width; i++){
        for(int j = 0; j < _height; j ++){
            //cout << _board[i][j];
            if(_board[i][j] == true){
                cout << 'O';
            }
            else{
                cout << " ";
            }
            cout <<"   ";
        }
        cout << endl;
    }

}

Board::~Board(){
    for(int i = 0; i < _width; i++){
        delete [] _board[i];
    }
    delete [] _board;
}
