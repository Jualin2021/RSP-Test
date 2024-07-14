//#define __DEBUG__  
//#define DefaultLinkInput

void LogMsg(const char *format, ...);
#define TRACE(fmt,...) LogMsg("%s(%d)(Func : %s): \n" ##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
