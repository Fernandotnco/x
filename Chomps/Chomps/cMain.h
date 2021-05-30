#pragma once
#include "wx\wx.h"
#include <wx/spinctrl.h>
#include "board.h"
#include <wx/stattext.h>
#include "AI.h"


class cMain : public wxFrame
{
private:
	int _w;
	int _h;
	Board *_board;
	int _turn;
	wxStaticText* _turnCounter = nullptr;
	wxStaticText* _turnShower = nullptr;
	wxStaticText* _winsShower = nullptr;
	bool _vsAI;
	bool _trainAI;
	AI _AI;
	AI _challenger;
	bool _gameOver;
	int _consecWins;
	int _prevMovx;
	int _prevMovy;
public:
	cMain(int w, int h, bool players, bool train);
	void FirstMove(wxIdleEvent& event);
	~cMain();

	wxButton** cookies = nullptr;
	void OnCookieChosen(wxCommandEvent& evt);

	void UpdateGame();

	void UpdateBoardDisplay();

	void ReestartBoard();

	void TrainAI();

	wxDECLARE_EVENT_TABLE();
};

