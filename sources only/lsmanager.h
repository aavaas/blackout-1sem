#ifndef LSMANAGER_H
#define LSMANAGER_H
//load shedding schedule manager
#include"common.h"
#include"internet.h"

class CLoadSheddingSchedule
{
private :
	CTime blackouttimes[7][7][2][2];//this will store the load shedding times
	CDateTime SyncDateTime;
	int curgroup;

	void ResetSyncDate();
	void ReadSchedule(const char* RawData,int length);
public:
	CLoadSheddingSchedule(){};
	~CLoadSheddingSchedule(){};
	void SetGroup(int group);
	void LoadSchedule();
	void SaveSchedule();
	void SyncSchedule();//loads the schedule from the internet if necessary
	CDateTime GetSyncDate();
	CTimeSecond IsBlackout(bool& isloadshedding);
	CTimeSecond GetPastTime();

};



#endif