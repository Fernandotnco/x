#include "cApp.h"
#include "wx\wx.h"
#include "StartMenu.h"

wxIMPLEMENT_APP(cApp);

cApp::cApp() {

}

cApp::~cApp() {

}

bool cApp::OnInit() {
	_frame1 = new StartMenu();
	_frame1->Show();
	return true;
}
