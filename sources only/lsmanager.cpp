#include"lsmanager.h"

//returns what time is past since last load shedding
CTimeSecond CLoadSheddingSchedule::GetPastTime()
{
	//get the current day
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t ), *lsdtime=localtime( & t );
	int weekday=now->tm_wday-1;
	//now see the schedule for the given weekday for the given group
	int hour=now->tm_hour,minute=now->tm_min;
	//see if the hour and minute fall under loadshedding hour

	int wd=now->tm_wday;//the week day

	for(int i=0;i<2;i++)//loop 2 times to cover both loadshedding times
	{
		if(blackouttimes[curgroup][wd][i][0].hour == 0 && blackouttimes[curgroup][wd][i][0].minute == 0 && blackouttimes[curgroup][wd][i][1].hour == 0 && blackouttimes[curgroup][wd][i][1].minute == 0)
		{
			//this time is just a place holder
		}
		else
		{
			//there is chance of load shedding
			wxString lt;
			//we dont need to check other things here. just know that load shedding is already effective
				//the loadshedding has already started. get the ending time
				lsdtime->tm_hour=blackouttimes[curgroup][wd][i][1].hour;
				lsdtime->tm_min=blackouttimes[curgroup][wd][i][1].minute;//see of ot exceeds the schedule
				lsdtime->tm_sec=0;
				//find the difference in in time
				time_t diff1=mktime(lsdtime);
				if(diff1>t)
				{
					lsdtime->tm_hour=blackouttimes[curgroup][wd][i][0].hour;//try to get diff from first time
					lsdtime->tm_min=blackouttimes[curgroup][wd][i][0].minute;
					lsdtime->tm_sec=0;
					//find the difference in in time
					time_t diff1=mktime(lsdtime);
					diff1=t-diff1;
					int seconds=(diff1%60);
					int hours=diff1/3600;
					int minutes=(diff1%3600)/60;
					return CTimeSecond(hours,minutes,seconds);
				}
		}
	}
	return CTimeSecond(0,0,0);
}
CTimeSecond CLoadSheddingSchedule::IsBlackout(bool& isloadshedding)
{
	//get the current day
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t ), *lsdtime=localtime( & t );
	int weekday=now->tm_wday-1;
	//now see the schedule for the given weekday for the given group
	int hour=now->tm_hour,minute=now->tm_min;
	//see if the hour and minute fall under loadshedding hour

	int wd=now->tm_wday;//the week day

	for(int i=0;i<2;i++)//loop 2 times to cover both loadshedding times
	{
		if(blackouttimes[curgroup][wd][i][0].hour == 0 && blackouttimes[curgroup][wd][i][0].minute == 0 && blackouttimes[curgroup][wd][i][1].hour == 0 && blackouttimes[curgroup][wd][i][1].minute == 0)
		{
			//this time is just a place holder
		}
		else
		{
			//there is chance of load shedding
			wxString lt;
			lsdtime->tm_hour=blackouttimes[curgroup][wd][i][0].hour;
			lsdtime->tm_min=blackouttimes[curgroup][wd][i][0].minute;
			lsdtime->tm_sec=0;
			//find the difference in in time
			time_t diff1=mktime(lsdtime);
			//get the difference
			if(diff1>t)
			{
				diff1=diff1-t;
				int seconds=(diff1%60);
				int hours=diff1/3600;
				int minutes=(diff1%3600)/60;
				isloadshedding=false;
				return CTimeSecond(hours,minutes,seconds);
			}
			else
			{
				//the loadshedding has already started. get the ending time
				//there is chance of load shedding
				lsdtime->tm_hour=blackouttimes[curgroup][wd][i][1].hour;
				lsdtime->tm_min=blackouttimes[curgroup][wd][i][1].minute;
				lsdtime->tm_sec=0;
				//find the difference in in time
				time_t diff1=mktime(lsdtime);
				if(diff1>t)
				{
					isloadshedding=true;
					diff1=diff1-t;
					int seconds=(diff1%60);
					int hours=diff1/3600;
					int minutes=(diff1%3600)/60;
					return CTimeSecond(hours,minutes,seconds);
				}
			}
		}
	}
	isloadshedding=false;
	return CTimeSecond(0,0,0);
}

void CLoadSheddingSchedule::LoadSchedule()
{
	//gets the load shedding file from the file
	ifstream in("schedule.dat",ios::binary );// instead of loading from the web server, we load a local file and try to read from it
	if(! in.good())
	{
		in.close();
		throw 1;
	}

	char* FileContents;
	//laod the file contents. In actual program it would have been downloaded from the internet

	in.seekg(0,ios::end);
	int length = in.tellg();
	in.seekg(ios::beg);

	FileContents=new char[length];

	//now read the contents of the file
	in.read(FileContents,length);
	ReadSchedule(FileContents,length);
	in.close();

	delete []FileContents;
	return;	
}

void CLoadSheddingSchedule::SaveSchedule()
{
	//gets the load shedding file from the file
	ofstream out("schedule.dat",ios::binary | ios::out );// instead of loading from the web server, we load a local file and try to read from it
	if(! out.good())
	{
		out.close();
		throw 2;
	}

	out.write(reinterpret_cast<char*>(&SyncDateTime),sizeof(SyncDateTime));
	out.write(reinterpret_cast<char*>(&blackouttimes[0][0][0][0]),sizeof(blackouttimes));
	out.close();
	return;	
}


void CLoadSheddingSchedule::SyncSchedule()
{
	//this function loads the loadshedding schedule from the internet.
	//it throws error if connection fails.
	//it is better to start this function in seperate thread
	CInternet Internet;
	ofstream out("schedule.dat",ios::binary | ios::out );
	if(! out.good())
	{
		out.close();
		throw 3;
	}
	try
	{
		Internet.GetData(out);
	}
	catch(int error)
	{
		out.close();//at least close the file in case of error
		throw error;//return it to parent function
	}
	out.close();
	LoadSchedule();
	return;	
}

void CLoadSheddingSchedule::ReadSchedule(const char* RawData,int length)
{
	//the first 4 bytes are of the date and time of sync, and other chars represent the hour and minute of each
	// loadshedding time
	if(length != 397)//5+49*4*4=392,  5 bytes for the date, and 49*4 bytes for each of the loadshedding time.
		throw 4;
	memcpy(&SyncDateTime,RawData,5);//read the 5 bytes data
	RawData+=5;
	//now load the whole load shedding table
	memcpy(&blackouttimes[0][0][0][0],RawData,sizeof(blackouttimes));
	//all loading done.
	return;
}
void CLoadSheddingSchedule::SetGroup(int group)
{
	curgroup=group;
}