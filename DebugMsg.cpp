#include <iostream>
#include <stdlib.h>
#include <stdio.h>  
#include <stdarg.h> /* va_list, va_start, va_arg, va_end */
#include <string.h>
#include <ctime>
#include <fstream>
using namespace std;
void LogMsg(const char *format, ...) {

	FILE *LogFile;
	errno_t err = fopen_s(&LogFile, "..//OutPut//Log.txt", "a+");

	if (LogFile == NULL)
	{
		cout << "LogFile is Not Opened" << endl;
		system("PAUSE");
	}
	va_list ap;
	va_start(ap, format);
	vfprintf(LogFile, format, ap);
	va_end(ap);
	fclose(LogFile);
	//Log = NULL;
}


void GetNowTime(ofstream FileName)
{
	struct tm newtime;
	char am_pm[] = "AM";
	__time64_t long_time;
	char timebuf[26];
	errno_t err;
	// Get time as 64-bit integer.
	_time64(&long_time);
	// Convert to local time.
	err = _localtime64_s(&newtime, &long_time);
	if (err)
	{
		printf("Invalid argument to _localtime64_s.");
		exit(1);
	}
	err = asctime_s(timebuf, 26, &newtime);
	if (err)
	{
		printf("Invalid argument to asctime_s.");
		exit(1);
	}
	FileName << timebuf << endl;
}

