#include "board.h"
#include"board.h"
#include<stdexcept>
#include<iostream>
#include<stdlib.h>
#include<time.h>

using std::cout;
using std::endl;

Board::Board() {
    _width = 0;
    _height = 0;
    _remainingCookies = 0;
}

Board::Board(int w, int h) {
    _allocated = false;
    if (w <= 0 || h <= 0) {
        throw std::invalid_argument("The size of the board must be greater than 0");
    }

    _width = w;
    _height = h;
    _remainingCookies = w * h;

    //creating and allocating the board
    //Allocating the columns
    try {
        _board = new bool* [w];
    }
    catch (std::bad_alloc& ba) {
        cout << "Failed to create the board: couldn't create columns.\n" << ba.what() << endl;
        throw;
        return;
    }
    //allocating each line
    for (int i = 0; i < w; i++) {
        try {
            _board[i] = new bool[h];
        }
        catch (std::bad_alloc& ba) {
            cout << "Failed to create the board: couldn't create line " << i << endl << ba.what() << endl;
            throw;
            return;
        }
    }

    //defining true for every element of the board
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            _board[i][j] = true;
        }
    }

    _allocated = true;
}

void Board::ChooseCookie(int w, int h) {

    if (_allocated == false) {
        throw std::out_of_range("The board isn't allocated correctly");
    }
    if (w < 0 || w >= _width || h < 0 || h >= _height) {
        cout << "Trying to choose cookie (" << w << ", " << h << "), but the board is of size (" << _width << ", " << _height << ")" << endl;
        throw std::invalid_argument("Invalid position on the board");
    }

    for (int i = w; i < _width; i++) {
        for (int j = h; j < _height; j++) {
            cout << "Eating cookie(" << i << ", " << j << ")" << endl;;
            if (_board[i][j]) {
                _remainingCookies--;
            }
            this->_board[i][j] = false;
        }
    }

    
}

void Board::ChooseRandomnly(int &prevMovx, int &prevMovy, int consecWins)
{
    if (_remainingCookies == 1) {
        ChooseCookie(0, 0);
        prevMovx = 0;
        prevMovy = 0;
        return;
    }
    srand(consecWins);
    int cookie = rand() % (_remainingCookies - 1) + 1;
    int n = 0;
    for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
            if (this->_board[x][y] == true) {
                if (n == cookie) {
                    prevMovx = x;
                    prevMovy = y;
                    if (!CheckPosition(x, y)) {
                        throw std::invalid_argument("no cookie damn");
                    }
                    ChooseCookie(x, y);
                    return;
                }
                n++;
            }
        }
    }
}
int Board::RemainingCookies()const {
    return _remainingCookies;
}

bool Board::CheckPosition(int w, int h)const {
    if (_allocated == false) {
        throw std::out_of_range("The board isn't allocated correctly");
    }
    if (w < 0 || w >= _width || h < 0 || h >= _height) {
        cout << "Trying to choose cookie (" << w << ", " << h << "), but the board is of size (" << _width << ", " << _height << ")" << endl;
        throw std::invalid_argument("Invalid position on the board");
    }

    return _board[w][h];
}

void Board::PrintBoard()const {
    if (_allocated == false) {
        throw std::out_of_range("The board isn't allocated correctly");
    }

    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            //cout << _board[i][j];
            if (_board[i][j] == true) {
                cout << 'O';
            }
            else {
                cout << " ";
            }
            cout << "   ";
        }
        cout << endl;
    }

}

int Board::BoardWidth() {
    return _width;
}

int Board::BoardHeight() {
    return _height;

}

Board::~Board() {
    for (int i = 0; i < _width; i++) {
        delete[] _board[i];
    }
    delete[] _board;
}

