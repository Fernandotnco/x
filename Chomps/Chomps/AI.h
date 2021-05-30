#pragma once
#include "board.h"
#include<vector>
#include<string>
class AI
{
private:
	std::vector<int> _bordersX;
	std::vector<int> _bordersY;
	std::vector<int> Split(std::string s, char token);
	std::string _filename;
	void IncludeMoves(int prevMovx, int prevMovy);

public:

	AI();
	AI(std::string filename);
	void MakeMove(Board& board, int &prevMovx, int &prevMovy, bool trainAI);
	void MakeMove2(Board& board, int prevMovx, int prevMovy);
	int CountEssentials(Board& board, int axis);
	void SetFileName(std::string filename);
	void Reset();
	~AI();
};

