#ifndef BLACKOUTFRAME_H
#define BLACKOUTFRAME_H

#include"common.h"
#include"lsmanager.h"
#include"alarmdialog.h"

#define WINWIDTH 300
#define WINHEIGHT 130
#define EXPWINHEIGHT 360

#define ID_CLOSEBUTTON 100
#define ID_SETTINGSBUTTON 102
#define ID_TIMEREMAIN 103
#define ID_WXMSG 104
#define REFRESH_TIMER 105
#define ID_WXALARMTIME 106
#define ID_TURNALARMONCHECK 107
#define ID_SAVEBUTTON 108
#define ID_CANCELBUTTON 109
#define ID_RESIZETIMER 110

class MyFrame: public wxFrame
{
	int framestate;//0 decreasing, 1 increasing, 2 stable
	int frameheight,framewidth;

	bool alarmdialog;
	bool maximized;
	wxPoint LastPoint;
	bool isDragging;
	bool ButtonsShowing;
	wxBitmapButton *CloseButton, *SettingsButton;
	//static control to show the time remaining
	wxStaticText *TimeRemaining,*Message;
	wxTimer *RefreshTimer,*ResizeTimer;
	wxComboBox *TimeCombo,*BeforeAfterCombo,*GroupCombo; 

	wxCheckBox *TurnAlarmCheck,*EnableAudioCheck,*AutoShutdownCheck;
	//we store these just to disable them when necessary
	vector<wxStaticText*> AlarmStatics;

	wxButton *SaveButton,*CancelButton;

	CLoadSheddingSchedule lsm;

	void RefreshSettings();
public:

	MyFrame(const wxString& title, 
		const wxPoint& pos, const wxSize& size);

	void OnLeftDown(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& WXUNUSED(evt));
	void OnMouseMove(wxMouseEvent& evt);

	void OnCloseButton(wxCommandEvent& event);
	void OnSaveButton(wxCommandEvent& event);
	void OnMaxMinButton(wxCommandEvent& event);
	void OnRefresh(wxTimerEvent& event);
	void OnResizeTimer(wxTimerEvent& event);
	void OnAlarmCheck(wxCommandEvent& event);

	void CheckAlarm();

	void LoadGlobalSettings();
	void SaveGlobalSettings();
	DECLARE_EVENT_TABLE()
};

#endif