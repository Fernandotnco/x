#include "AI.h"
#include<stdexcept>
#include<windows.h>
#include<fstream>
#include<string>
#include<algorithm> 

std::vector<int> AI::Split(std::string s, char token)
{
	std::vector<int> tokens;
	int tokenStart = 0;
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == token) {
			tokens.push_back(stoi(s.substr(tokenStart, i - tokenStart)));
			tokenStart = i + 1;
		}
	}

	return tokens;
}

void AI::IncludeMoves(int prevMovx, int prevMovy)
{
	for (int i = 0; i < _bordersX.size(); i++) {
		if (prevMovx <= _bordersX[i]) {
			if (-prevMovy >= _bordersY[i]) {
				_bordersX.erase(_bordersX.begin() + i);
				_bordersY.erase(_bordersY.begin() + i);
				i--;
			}
		}
	}

	_bordersX.push_back(prevMovx);
	sort(_bordersX.begin(), _bordersX.end());
	_bordersY.push_back(-prevMovy);
	sort(_bordersY.begin(), _bordersY.end());
}

AI::AI() {

}

AI::AI(std::string filename)
{
	_filename = filename;
}


void AI::MakeMove2(Board& board, int prevMovx, int prevMovy)
{
	int essentialsTop = CountEssentials(board, 1);
	int essentialsLeft = CountEssentials(board, 0);
	bool key = board.CheckPosition(1, 1);
	int extras = board.RemainingCookies() - 1 - essentialsTop - essentialsLeft - (key ? 1 : 0);
	bool square = essentialsTop == essentialsLeft;
	int essentialsDiff;
	bool biggestEven;
	int maxEssentials;
	if (essentialsTop > essentialsLeft) {
		essentialsDiff = essentialsTop - essentialsLeft;
		biggestEven = (essentialsTop % 2) == 0;
		maxEssentials = essentialsTop;
	}
	else {
		essentialsDiff = essentialsLeft - essentialsTop;
		biggestEven = (essentialsLeft % 2) == 0;
		maxEssentials = essentialsLeft;
	}
	bool singleEssentialsColumn = !(board.CheckPosition(1, 2) && board.CheckPosition(2, 1));
	Sleep(rand()%1800 + 700);
	try {


		if (square) {
			if (key) {
				board.ChooseCookie(1, 1);
				return;
			}
			else {
				board.ChooseCookie(0, 0);
			}
		}

		if (!key) {
			if (square) {
				board.ChooseCookie(0, 0);
				return;
			}
			if (essentialsTop > essentialsLeft) {
				board.ChooseCookie(essentialsLeft + 1, 0);
				return;
			}
			else {
				board.ChooseCookie(0, essentialsTop + 1);
				return;
			}
		}

		if (!biggestEven && essentialsDiff == 1) {
			if (extras == 0) {
				if (essentialsTop > essentialsLeft) {
					board.ChooseCookie(essentialsLeft, 0);
					return;
				}
				board.ChooseCookie(0, essentialsTop);
				return;
			}
			else if (board.CheckPosition(2, 1)) {
				board.ChooseCookie(2, 1);
				return;
			}
		if (singleEssentialsColumn && extras == maxEssentials - 2) {
			if (essentialsTop > essentialsLeft) {
				board.ChooseCookie(2, 0);
				return;
			}
			board.ChooseCookie(0, 2);
			return;
		}

		if (singleEssentialsColumn && extras == maxEssentials - 3)  {
			if (essentialsLeft == 1) {
				board.ChooseCookie(extras + 1, 1);
				return;
			}
			else if (essentialsTop == 1) {
				board.ChooseCookie(extras + 1, 2);
				return;
			}
		}


		}
		board.ChooseCookie(0, 0);
	}
	catch (std::exception& e) {
		board.ChooseCookie(0, 0);
	}
}

void AI::MakeMove(Board& board, int &prevMovx, int &prevMovy, bool trainAI) {
	bool invertedBoard = (board.BoardHeight() > board.BoardWidth());
	
	if (_bordersX.size() == 0) {
		if (!invertedBoard) {
			IncludeMoves(0, board.BoardHeight());
			IncludeMoves(board.BoardWidth(), 0);
		}
		else {
			IncludeMoves(0, board.BoardWidth());
			IncludeMoves(board.BoardHeight(), 0);
		}
	}
	std::vector<int> prevBorderX = _bordersX;
	std::vector<int> prevBorderY = _bordersY;
	if (prevMovx != 0 || prevMovy != 0) {
		if (!invertedBoard) {
			IncludeMoves(prevMovx, prevMovy);
		}
		else {
			IncludeMoves(prevMovy, prevMovx);
		}
	}

	std::ifstream loseConditions;
	std::string linex;
	std::string liney;
	std::vector<int> loseCondX;
	std::vector<int> loseCondY;
	std::vector<int> auxBordersX = _bordersX;
	std::vector<int> auxBordersY = _bordersY;
	bool ret;
	bool lose = true;
	bool invalidMove = true;

	for (int x = 0; x < board.BoardWidth(); x++) {
		for (int y = 0; y < board.BoardHeight(); y++) {
			if (x != 0 || y != 0) {
				invalidMove = false;
				if (board.CheckPosition(x, y)) {
					if (!invertedBoard) {
						IncludeMoves(x, y);
					}
					else {
						IncludeMoves(y, x);
					}
					ret = false;

					if (_bordersX.size() == 1) {
						if ((_bordersX[0] == 0 && _bordersY[0] < -1) || (_bordersY[0] == 0 && _bordersX[0] > 1)) {
							ret = true;
						}
					}
					else if (_bordersX.size() == 2) {
						if ((_bordersX[0] == 0 && _bordersY[0] < -1) && (_bordersY[1] == 0 && _bordersX[1] > 1)) {
							ret = true;
						}
					}
					if (!ret) {
						loseConditions.open(_filename);
						while (std::getline(loseConditions, linex) && !invalidMove) {
							std::getline(loseConditions, liney);

							loseCondX = Split(linex, ',');
							loseCondY = Split(liney, ',');

							if (loseCondX == _bordersX && loseCondY == _bordersY) {
								invalidMove = true;
							}
						}
						loseConditions.close();
						if (!invalidMove) {
							board.ChooseCookie(x, y);
							prevMovx = x;
							prevMovy = y;
							return;
						}
					}
				}
				
			}
			_bordersX = auxBordersX;
			_bordersY = auxBordersY;
		}
	}
	if (!trainAI) {
		//Sleep(1500);
	}
	if (trainAI) {

		std::ofstream writeConditions;
		writeConditions.open(_filename, std::ios_base::app);
		writeConditions << std::endl;
		for (int i = 0; i < prevBorderX.size(); i++) {
			writeConditions << prevBorderX[i] << ',';
		}
		writeConditions << std::endl;
		for (int i = 0; i < prevBorderY.size(); i++) {
			writeConditions << prevBorderY[i] << ',';
		}

		writeConditions.close();
	}
	board.ChooseCookie(0, 0);
}

int AI::CountEssentials(Board& board, int axis)
{
	if (axis > 1 && axis < 1) {
		throw std::invalid_argument("axis must be either 0 or 1");
	}
	int count = 0;
	int size;
	if (axis == 0) {
		size = board.BoardWidth();
		for (int i = 1; i < size; i++) {
			if (board.CheckPosition(i, 0)) {
				count++;
			}
		}
	}
	else {
		size = board.BoardHeight();
		for (int i = 1; i < size; i++) {
			if (board.CheckPosition(0, i)) {
				count++;
			}
		}
	}
	return count;
}

void AI::SetFileName(std::string filename)
{
	_filename = filename;
	std::fstream file;
	file.open(filename);
	std::string s;
	if (file.peek() == std::ifstream::traits_type::eof()) {
		file << "0," << std::endl << "-1," << std::endl << "1," << std::endl << "0,";
	}

	file.close();
}

void AI::Reset()
{
	_bordersX.clear();
	_bordersY.clear();
}

AI::~AI() {
}
