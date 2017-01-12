#include"alarmdialog.h"

BEGIN_EVENT_TABLE(AlarmDialog, wxDialog)
	EVT_CLOSE(AlarmDialog::OnClose)
	EVT_BUTTON(ID_CLOSE_BUTTON, AlarmDialog::OnCloseButton)
	EVT_BUTTON(ID_SHUTDOWN_BUTTON, AlarmDialog::OnShutdownButton)
	END_EVENT_TABLE()

	AlarmDialog::AlarmDialog(wxWindow* parent,bool sound): wxDialog(parent, wxID_ANY, "Alarm",wxDefaultPosition, wxSize(270, 150),wxDEFAULT_DIALOG_STYLE)
{
	this->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	this->SetForegroundColour(wxColour(0,0,0));

	wxPanel *panel = new wxPanel(this, -1);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	// Create the controls.
	panel->SetBackgroundColour( wxColour( 255, 255, 255 ) );
	panel->SetForegroundColour(wxColour(0,0,0));

	wxAnimationCtrl *an = new wxAnimationCtrl(panel, wxID_ANY, wxAnimation(wxT("bell.gif"), wxANIMATION_TYPE_ANY), wxPoint(0,0));
	an->Play();


	wxFont font(30, wxDEFAULT, wxNORMAL, wxBOLD);
	AlarmText=new wxStaticText(panel, ID_ALARMTEXT, wxT("Alarm!!!"),wxPoint(110, 20));
	AlarmText->SetFont(font);


	ShutdownButton=new wxButton(panel, ID_SHUTDOWN_BUTTON, wxT("Shut Down"),wxPoint(40, 90));
	CloseButton=new wxButton(panel, ID_CLOSE_BUTTON, wxT("Close"),wxPoint(160, 90));

	vbox->Add(panel, 1);
	SetSizer(vbox);
	Centre();

	snd=new wxSound("beep.wav");
	if(sound)
		snd->Play(wxSOUND_ASYNC | wxSOUND_LOOP);
}
void AlarmDialog::OnCloseButton(wxCommandEvent& event)
{
	Close(true);
}

void AlarmDialog::OnShutdownButton(wxCommandEvent& event)
{
	ShutDown();
}

void AlarmDialog::OnClose(wxCloseEvent& event)
{
	snd->Stop();
	delete snd;
	event.Skip();
}
