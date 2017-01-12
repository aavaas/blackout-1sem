#ifndef ALARMDIALOG_H
#define ALARMDIALOG_H

#include"common.h"
#include<wx/animate.h>
#include<wx/sound.h>


#define ID_ALARMTEXT 500
#define ID_BLINK_TIMER 501
#define ID_CLOSE_BUTTON 502
#define ID_SHUTDOWN_BUTTON 503


class AlarmDialog : public wxDialog
{
	wxSound* snd;
	wxStaticText* AlarmText;
	wxButton *ShutdownButton,*CloseButton;
public:
    AlarmDialog(wxWindow* parent,bool sound);
	void OnClose(wxCloseEvent& event);
	void OnCloseButton(wxCommandEvent& event);
	void OnShutdownButton(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

#endif
