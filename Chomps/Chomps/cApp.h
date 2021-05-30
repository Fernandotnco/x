#pragma once
#include "wx\wx.h"
#include "cMain.h"
#include "StartMenu.h"


class cApp:public wxApp
{

public:
	cApp();
	~cApp();

private:

	StartMenu* _frame1 = nullptr;

public:
	virtual bool OnInit();
};

