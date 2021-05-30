#include "cMain.h"
#include "wx\wx.h"
#include <string>
#include<iostream>
#include <wx/stattext.h>
#include "AI.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_IDLE(FirstMove)
wxEND_EVENT_TABLE()

cMain::cMain(int w, int h, bool players, bool train) : wxFrame(nullptr, wxID_ANY, "CHOMPS O JOGO", wxPoint(30,30),wxSize(800,600)) {
	cookies = new wxButton*[w * h];
	wxGridSizer* grid = new wxGridSizer(w, h, 0, 0);
	wxBoxSizer* ui = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* screen = new wxBoxSizer(wxVERTICAL);
	_board = new Board(w, h);
	_w = w;
	_h = h;
	_turn = 1;
	_vsAI = players;
	_trainAI = train;
	_gameOver = false;
	_consecWins = 0;
	_prevMovx = 0;
	_prevMovy = 0;
	AI _AI();
	AI _challenger();


	_turnShower = new wxStaticText(this, wxID_ANY, "Player's 1 turn");
	_turnCounter = new wxStaticText(this, wxID_ANY, "turn 1");
	_winsShower = new wxStaticText(this, wxID_ANY, "0");
	_turnCounter->SetWindowStyleFlag(wxALIGN_RIGHT | wxST_NO_AUTORESIZE);

	ui->Add(_turnShower, true, wxEXPAND);
	ui->Add(_turnCounter, true, wxEXPAND);
	if (_trainAI) {
		ui->Add(_winsShower, true, wxEXPAND);
	}
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			cookies[y * w + x] = new wxButton(this, 10000 + (y * w + x));
			cookies[y * _w + x]->SetLabel("O");
			grid->Add(cookies[y * w + x], 1, wxEXPAND | wxALL);

			cookies[y*w + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnCookieChosen, this);
		}
	}

	screen->Add(ui, false, wxEXPAND);
	screen->Add(grid, true, wxEXPAND);

	this->SetSizer(screen);
	screen->Layout();

}

void cMain::FirstMove(wxIdleEvent& event)
{

	if (!_gameOver) {
		if (_trainAI) {

			_AI.SetFileName("ai.txt");
			_challenger.SetFileName("challenger.txt");
			TrainAI();

		}
		if ((_vsAI && _turn == 1) && !_trainAI){
			_AI.SetFileName("ai.txt");
			_AI.MakeMove(*_board, _prevMovx, _prevMovy, true);
			UpdateGame();
		}
	}
	event.Skip();
}

cMain::~cMain() {
	delete[] cookies;
	delete _board;
}

void cMain::OnCookieChosen(wxCommandEvent& evt){
	try {
		if (!_trainAI) {
			if (_turn % 2 == 0 || !_vsAI) {

				_prevMovx = (evt.GetId() - 10000) % _w;
				_prevMovy= (evt.GetId() - 10000) / _w;
				_board->ChooseCookie(_prevMovx, _prevMovy);
				UpdateGame();

				if (!_gameOver && _vsAI) {
					_AI.MakeMove(*_board, _prevMovx, _prevMovy, true);
					UpdateGame();
				}

			}
		}

	}
	catch (std::exception& e) {
		std::cout << "Failed to do sometinh LOL" << e.what() << std::endl;
	}
	evt.Skip();
}

void cMain::UpdateGame()
{
	std::string p = std::to_string(_turn % 2 + 1);
	std::string wins = std::to_string(_consecWins);
	UpdateBoardDisplay();
	if (!_board->CheckPosition(0, 0)) {
		if (!_trainAI) {
			wxMessageBox("Player " + p + " Won!");
		}
		_turn++;
		_gameOver = true;
		if (_turn % 2 == 1) {
			_consecWins++;
		}
		else {
			_consecWins = 0;
		}

	}
	_turn++;
	_turnShower->SetLabelText("Player's " + p + " turn");
	_turnCounter->SetLabelText("turn " + std::to_string(_turn));
	_winsShower->SetLabelText(wins);
}

void cMain::UpdateBoardDisplay() {
	for (int x = 0; x < _w; x++) {
		for (int y = 0; y < _h; y++) {
			if (!_board->CheckPosition(x, y)) {
				cookies[y * _w + x]->Enable(false);
				cookies[y * _w + x]->SetLabel("");
			}
			else {
				cookies[y * _w + x]->Enable(true);
				cookies[y * _w + x]->SetLabel("O");
			}
		}
	}
}

void cMain::ReestartBoard()
{
	_gameOver = false;
	_turn = 1;
	_prevMovx = 0;
	_prevMovy = 0;
	delete _board;
	_AI.Reset();
	_challenger.Reset();
	_board = new Board(_w, _h);
	if (!_board->CheckPosition(_w - 1, _h - 1)) {
		throw std::invalid_argument("Invalid");
	}
	UpdateBoardDisplay();
}

void cMain::TrainAI()
{
	bool finnishTrain = false;
	while(finnishTrain == false) {

		while (!_gameOver) {
			if (_turn % 2 == 1) {
				_AI.MakeMove(*_board, _prevMovx, _prevMovy, true);
				UpdateGame();
				Sleep(1000);
			}
			else {
				_challenger.MakeMove(*_board, _prevMovx, _prevMovy, true);
				UpdateGame();
				Sleep(1000);
			}
		}
		std::string turnw = std::to_string(_turn);
		if (_turn == 4) {
			finnishTrain = true;
			wxMessageBox("Training completed");
		}
		else {
			ReestartBoard();
		}
	}
}
