#ifndef COMMON_H
#define COMMON_H

#define PLATFORM_WINDOWS
#define WXUSINGDLL 
#define wxUSE_ANIMATIONCTRL 1
#include "wx/wx.h" 
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<ctime>
#include<vector>
using namespace std;


//this structure will store time (hours and minute)
//char is used instead of int to reduce data so that it can sync quickly in slow connection
struct CTime
{
	char hour;
	char minute;
	CTime(char h=0,char m=0):hour(h),minute(m){}
	CTime(const CTime& c):hour(c.hour),minute(c.minute){};

	~CTime(){}
};

struct CTimeSecond
{
	char hour;
	char minute;
	char second;
	CTimeSecond(char h=0,char m=0,char s=0):hour(h),minute(m),second(s){}
	CTimeSecond(const CTimeSecond& c):hour(c.hour),minute(c.minute),second(c.second){};
};

//this one will store date
struct CDateTime
{
	CTime time;
	char year;//we store only the last 2 digits
	char month;
	char day;
	CDateTime(CTime t=CTime(),char y=0,char m=0,char d=0):time(t),year(y),month(m),day(d){}
};



#ifdef PLATFORM_WINDOWS
//functions for windows
void ShutDown();
void Hibernate();
#endif

#ifdef PLATFORM_LINUX
//functions for linux
void ShutDown();
void Hibernate();
#endif

#ifdef PLATFORM_MACOSX
void ShutDown();
#endif

#endif
