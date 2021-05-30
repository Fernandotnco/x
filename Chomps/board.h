#ifndef BOARD_H
#define BOARD_H

class Board{

private:

    //Width of the board
    int _width;

    //Height of the board
    int _height;

    //Number of cookies still on the board
    int _remainingCookies;

    //Representes the board. A True indicates there's a cookie in the position, and a False indicates otherwise
    //Note that positions ond the board grow to the right and down, so the top-left corner is position (0,0), and the bottom-right is (_width, _height)
    bool** _board;

    //true if the board's correctly allocated, false otherwise
    bool _allocated;

public:

    //Creates a board of _width w and _height h
    Board(int w, int h);

    //Choose the cookie of position (w, h) in the board.
    //According the chomps rules, it then removes all cookie which  positions x,y are such that
    //x >= w and y >= y from the board
    void ChooseCookie(int w, int h);

    //return the number of cookies still on the board
    int RemainingCookies() const;

    //Verifies if there's a cookie in position (w, h)
    bool CheckPosition(int w, int h) const;

    void PrintBoard()const;

    //Destructor
    ~Board();


};



#endif