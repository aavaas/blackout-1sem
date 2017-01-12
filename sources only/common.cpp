#include"common.h"


#ifdef PLATFORM_WINDOWS
//functions for windows
void ShutDown()
{
	system("shutdown /s");
}

void Hibernate()
{
	system("shutdown /h");
}

#endif

#ifdef PLATFORM_LINUX
//functions for linux
void ShutDown()
{
	system("shutdown -h now");
}
void Hibernate()
{
	system("pm-hibernate");
}
#endif

#ifdef PLATFORM_MACOSX
//functions for linux
void ShutDown()
{
	system("shutdown -h now");
}
#endif