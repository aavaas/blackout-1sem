#include"blackoutframe.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_LEFT_DOWN(MyFrame::OnLeftDown)
	EVT_LEFT_UP(MyFrame::OnLeftUp)
	EVT_MOTION(MyFrame::OnMouseMove)
	EVT_BUTTON(ID_CLOSEBUTTON, MyFrame::OnCloseButton)
	EVT_BUTTON(ID_SAVEBUTTON, MyFrame::OnSaveButton)
	EVT_BUTTON(ID_SETTINGSBUTTON, MyFrame::OnMaxMinButton)
	EVT_BUTTON(ID_CANCELBUTTON, MyFrame::OnMaxMinButton)//same thing
	EVT_TIMER(REFRESH_TIMER, MyFrame::OnRefresh)
	EVT_TIMER(ID_RESIZETIMER, MyFrame::OnResizeTimer)
	END_EVENT_TABLE()


	void MyFrame::OnResizeTimer(wxTimerEvent& event)
{
	//wxMessageBox("hello world");
	if(framestate==1)//increasing
	{

		if(frameheight>=EXPWINHEIGHT)
		{
					wxMessageBox("up boy");
			ResizeTimer->Stop();
			framestate=2;//normal
			maximized=true;
		}
		else
		{
			wxMessageBox("down boy");
			frameheight+=200;
			this->SetClientSize(WINWIDTH,frameheight);
			ResizeTimer->Stop();
		}
	}
	else if(framestate==0)//decreasing
	{
		if(frameheight <= WINHEIGHT)
		{
			ResizeTimer->Stop();
			framestate=2;//normal
			maximized=false;
		}
		else
		{
			frameheight-=2;
			this->SetClientSize(WINWIDTH,frameheight);
		}
	}
}
void MyFrame::OnMaxMinButton(wxCommandEvent& event)
{
	if(maximized)
	{
		//this->SetClientSize(WINWIDTH,WINHEIGHT);

		framestate=0;
		if(!ResizeTimer->IsRunning())
			ResizeTimer->Start(50);
	}
	else
	{
		framestate=1;
		//this->SetClientSize(WINWIDTH,EXPWINHEIGHT);
		if(!ResizeTimer->IsRunning())
			ResizeTimer->Start(50);
	}
}
void MyFrame::OnCloseButton(wxCommandEvent& event)
{
	//minimize the application to tray
	Close(TRUE);
	return;
}
void MyFrame::OnSaveButton(wxCommandEvent& event)
{
	try
	{
		SaveGlobalSettings();
		//we have to reload the fgroup after saving of data
		lsm.SetGroup(GroupCombo->GetSelection()+1);
	}
	catch(...)
	{
		wxMessageBox("Error while Saving global settings!", "Error",wxOK | wxICON_ERROR, this);
	}
}
// Event handler for left mouse button click
void MyFrame::OnLeftDown(wxMouseEvent& evt)
{
	isDragging=true;
	CaptureMouse();
	wxPoint pos = ClientToScreen(evt.GetPosition());
	wxPoint origin = GetPosition();
	int dx =  pos.x - origin.x;
	int dy = pos.y - origin.y;
	LastPoint = wxPoint(dx, dy);
}

// Event handler for the left mouse button release
void MyFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
	if (HasCapture())      // If mouse has been captured, release it on button release
	{
		ReleaseMouse();
	}
	isDragging=false;
}

// Event handler for mouse movements to allow window dragging
void MyFrame::OnMouseMove(wxMouseEvent& evt)
{
	wxPoint pt = evt.GetPosition();
	if(!isDragging)
	{
		//see if the mouse falls inside some controls. If so, show/hide the controls
		if(pt.x >= WINWIDTH-100 && pt.y <= 50)
		{
			//show the buttons if they are not showing
			if(!ButtonsShowing)
			{
				CloseButton->Show();
				SettingsButton->Show();
				ButtonsShowing=true;
			}
		}
		else
		{
			if(ButtonsShowing)
			{
				CloseButton->Hide();
				SettingsButton->Hide();
				ButtonsShowing=false;
			}
		}
		return;
	}

	if (evt.Dragging() && evt.LeftIsDown())   // If you are dragging the window
	{
		wxPoint pos = ClientToScreen(pt);

		// Move window to match mouse movements
		Move(wxPoint(pos.x - LastPoint.x, pos.y - LastPoint.y));
	}
}


void MyFrame::OnRefresh(wxTimerEvent& event)
{
	RefreshSettings();
}
void MyFrame::RefreshSettings()
{
	wxString timelabel;

	bool isblackout=false;
	CTimeSecond btime=lsm.IsBlackout(isblackout);
	int usersetmins[]={1,2,3,4,5,6,7,8,9,10,20,30,40,50,59};///user allowed alarm time minutes


	if(btime.hour == 0 && btime.minute == 0)
	{
		Message->SetLabel("No more Blackout today!");
		timelabel.Printf("%02d:%02d:%02d",0,0,0);
	}
	else if(isblackout)
	{
		CTimeSecond tme=lsm.GetPastTime();
		//see if there is any use of the passed time
		if(TurnAlarmCheck->GetValue() && BeforeAfterCombo->GetSelection() == 1 && tme.hour == 0)
		{
			if(usersetmins[TimeCombo->GetSelection()] == tme.minute && tme.second == 0 && !alarmdialog)
			{
				//if shutdown is checked, go for it
				if(AutoShutdownCheck->GetValue())
					ShutDown();
				alarmdialog=true;
				AlarmDialog dialog(this,EnableAudioCheck->GetValue());
				dialog.ShowModal();
				RefreshSettings();
				alarmdialog=false;
				return;
			}			
		}
		timelabel.Printf("%02d:%02d:%02d",btime.hour,btime.minute ,btime.second);
		Message->SetLabel("Time remaining before Blackout ends");
	}
	else
	{
		//see if the minute remaining before blackout is same as the user set value


		if(TurnAlarmCheck->GetValue() && BeforeAfterCombo->GetSelection() == 0 && btime.hour == 0)
		{
			if(usersetmins[TimeCombo->GetSelection()] == btime.minute&& btime.second == 0 && !alarmdialog)
			{
				if(AutoShutdownCheck->GetValue())
					ShutDown();
				alarmdialog=true;
				AlarmDialog dialog(this,EnableAudioCheck->GetValue());
				dialog.ShowModal();
				RefreshSettings();
				alarmdialog=false;
				return;
			}			
		}

		timelabel.Printf("%02d:%02d:%02d",btime.hour,btime.minute , btime.second);
		Message->SetLabel("Time remaining before Blackout");
	}


	TimeRemaining->SetLabel(timelabel);
}

void MyFrame::LoadGlobalSettings()
{
	/*
	global settinggs- alaram on/off, before/after, X minutes, enable audio, enable shutdown, group id
	*/
	//loads the global settings like the alarm settings, etc
	ifstream in("settings.dat" );
	if(! in.good())
	{
		in.close();
		throw 1;
	}
	bool bl;
	int inv;

	in>>bl;
	TurnAlarmCheck->SetValue(bl);//first setting

	in>>inv;
	inv=(inv==0)?0:1;
	BeforeAfterCombo->SetSelection(inv);//either 0 or 1

	//load the x minutes
	in>>inv;
	if(inv<0 || inv>14)
		inv=0;	
	TimeCombo->SetSelection(inv);

	in>>bl;
	EnableAudioCheck->SetValue(bl);

	in>>bl;
	AutoShutdownCheck->SetValue(bl);

	in>>inv;
	if(inv<0 || inv>6)
		inv=0;
	GroupCombo->SetSelection(inv);

	//finally check if no item is disabled
	CheckAlarm();
	in.close();
	//now load the sync time
	return;	

}
void MyFrame::SaveGlobalSettings()
{
	/*
	global settinggs- alaram on/off, before/after, X minutes, enable audio, enable shutdown, group id
	*/
	//loads the global settings like the alarm settings, etc
	ofstream out("settings.dat" );
	if(! out.good())
	{
		out.close();
		throw 1;
	}
	out<<TurnAlarmCheck->GetValue()<<" ";
	out<<BeforeAfterCombo->GetSelection()<<" ";
	out<<TimeCombo->GetSelection()<<" ";
	out<<EnableAudioCheck->GetValue()<<" ";
	out<<AutoShutdownCheck->GetValue()<<" ";
	out<<GroupCombo->GetSelection()<<" ";
	out.close();
	//now load the sync time
	return;	
}
void MyFrame::CheckAlarm()
{

	if(TurnAlarmCheck->IsChecked())
	{
		EnableAudioCheck->Enable();
		AutoShutdownCheck->Enable();
		for(int i=0;i<AlarmStatics.size();i++)
			AlarmStatics[i]->Enable();
		BeforeAfterCombo->Enable();
		TimeCombo->Enable();
	}
	else
	{
		EnableAudioCheck->Disable();
		AutoShutdownCheck->Disable();
		for(int i=0;i<AlarmStatics.size();i++)
			AlarmStatics[i]->Disable();
		BeforeAfterCombo->Disable();
		TimeCombo->Disable();

	}
}
void MyFrame::OnAlarmCheck(wxCommandEvent& event)
{
	CheckAlarm();
}
MyFrame::MyFrame(const wxString& title, 
	const wxPoint& pos, const wxSize& size)
	: wxFrame((wxFrame *)NULL, -1, title, pos, size,wxFRAME_NO_TASKBAR | wxBORDER_NONE ,"blackout"),	isDragging(false)
{
	maximized=false;


	//create all the buttons and other stuff
	wxImage::AddHandler( new wxPNGHandler );
	CloseButton = new wxBitmapButton(this, ID_CLOSEBUTTON, wxBitmap(wxT("close.png"), wxBITMAP_TYPE_PNG), wxPoint(270, 0));
	CloseButton->SetToolTip("Close Blackout Application");
	SettingsButton=new wxBitmapButton(this, ID_SETTINGSBUTTON, wxBitmap(wxT("settings.png"), wxBITMAP_TYPE_PNG), wxPoint(245, 0));
	SettingsButton->SetToolTip("Blackout Settings");

	wxFont font(40, wxDEFAULT, wxNORMAL, wxBOLD);
	TimeRemaining=new wxStaticText(this, ID_TIMEREMAIN, wxT("00:00:00"),wxPoint(40, 20));
	TimeRemaining->SetFont(font);


	wxFont font2(12, wxDEFAULT, wxITALIC, wxNORMAL);
	Message=new wxStaticText(this, ID_WXMSG, wxT(""),wxPoint(25, 85));
	Message->SetFont(font2);

	wxStaticBox *st = new wxStaticBox(this, wxID_ANY, wxT("Timing Options"),wxPoint(5, 135), wxSize(290, 185));


	TurnAlarmCheck=new wxCheckBox(this, ID_TURNALARMONCHECK, wxT("Turn Alarm On"), wxPoint(17, 160));
	Connect(ID_TURNALARMONCHECK, wxEVT_COMMAND_CHECKBOX_CLICKED,wxCommandEventHandler(MyFrame::OnAlarmCheck));



	AlarmStatics.push_back(new wxStaticText(this, wxID_ANY, wxT("Alarm"),wxPoint(17, 180), wxSize(-1, -1), wxALIGN_LEFT));
	wxString options[60];

	//create the options for before/after
	options[0]="Before";
	options[1]="After";
	BeforeAfterCombo = new wxComboBox(this,wxID_ANY, wxT("Before"), wxPoint(57, 175),wxSize(-1,-1),2,options,wxCB_READONLY);


	for(int i=0;i<10;i++)
		options[i].Printf("%d",i+1);
	options[10]="20";
	options[11]="30";
	options[12]="40";
	options[13]="50";
	options[14]="59";
	TimeCombo = new wxComboBox(this,wxID_ANY, wxT("1"), wxPoint(130, 175),wxSize(40,-1),15,options,wxCB_READONLY);

	AlarmStatics.push_back(new wxStaticText(this, wxID_ANY, wxT("Minutes"),wxPoint(175, 180), wxSize(90, -1), wxALIGN_LEFT));
	//show the two check boxes for sound alarm and automatic shut down
	EnableAudioCheck=new wxCheckBox(this, wxID_ANY, wxT("Enable Audio Alarm"), wxPoint(17, 205));
	AutoShutdownCheck=new wxCheckBox(this, wxID_ANY, wxT("Automatic Shutdown (forced)"), wxPoint(17, 230));

	CheckAlarm();

	//now prepare the group
	for(int i=1;i<=7;i++)
		options[i].Printf("%d",i+1);

	GroupCombo = new wxComboBox(this,wxID_ANY, wxT("1"), wxPoint(60, 250),wxSize(40,-1),7,options,wxCB_READONLY);
	wxStaticText*st9=new wxStaticText(this, wxID_ANY, wxT("Group"),wxPoint(17, 255), wxSize(-1, -1), wxALIGN_LEFT);


	//create the save and cancel button
	SaveButton=new wxButton(this, ID_SAVEBUTTON, wxT("Save"),wxPoint(100, 280));
	CancelButton=new wxButton(this, ID_CANCELBUTTON, wxT("Cancel"),wxPoint(200, 280));
	//try to load the settings
	try
	{
		LoadGlobalSettings();//try to load the global settings
	}
	catch(...)
	{
		try
		{
			SaveGlobalSettings();
		}
		catch(...)
		{
			wxMessageBox("Error while loading global settings!", "Error",wxOK | wxICON_EXCLAMATION, this);
		}
	}

	CloseButton->Hide();
	SettingsButton->Hide();
	ButtonsShowing=false;
	Centre();//we start at the centre
	//load the loadshedding schedule
	try
	{
		lsm.LoadSchedule();
		//check if file is up to date

	}
	catch(int Error)
	{
		wxMessageBox("Cannot load the input file. BlackOut will now try to sync the file from the internet!");
		//cant load the file. try to sync from the internet. 
		try
		{
			lsm.SyncSchedule();
			lsm.LoadSchedule();
		}
		catch(int error)
		{
			if(error == 1)
				wxMessageBox("Cannot load from the internet. BlackOut will now exit!");
			else
				wxMessageBox("Cannot load from drive. BlackOut will now exit!");
			Close(true);
		}
	}

	lsm.SetGroup(2);

	//set the timer
	RefreshTimer = new wxTimer(this, REFRESH_TIMER);
	RefreshTimer->Start(1000);
	ResizeTimer=new wxTimer(this, ID_RESIZETIMER);

	frameheight=WINHEIGHT;
	framewidth=WINWIDTH;

	alarmdialog=false;
	framestate=2;///stable frame state initially
	RefreshSettings();




}
