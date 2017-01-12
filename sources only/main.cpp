/*
blackout
the load shedding program made by anup pokhrel, manish sharma and aavaas gajurel
copyleft (c) the three programmers above

*/
#include"common.h"
#include"blackoutframe.h"

//this is the main application class
class BlackOutApp: public wxApp
{
	virtual bool OnInit();
public:
	MyFrame *MainFrame;
};



IMPLEMENT_APP(BlackOutApp)


bool BlackOutApp::OnInit()
{
	MainFrame= new MyFrame( "Blackout", 
		wxPoint(50,50), wxSize(WINWIDTH,WINHEIGHT) );
	MainFrame->Show(TRUE);
	SetTopWindow(MainFrame);
	return TRUE;
} 