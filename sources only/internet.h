#ifndef INTERNET_H
#define INTERNET_H

#include"common.h"
#include<wx/url.h>
#include<wx/sstream.h>
#include<wx/log.h>

#pragma comment(lib,"wxbase29u_net_dll")

class CInternet
{
public:
	void GetData(ofstream& output);
};


#endif