#include "Common.h"
#include "Log.h"

DWORD RealWorldTime = 0;
DWORD NowTickCount = 0;
bool IsServerInitOver = false;
ST    NowSystemTime;
class ObjectFactory* GObjFact;

LockMeta* CreateThreadLock()
{
	return new CriticalLock;
}

#ifdef _WIN32
static inline void SetColor(WORD ForColor,WORD BackColor)
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCon,ForColor|BackColor);
}
#endif


char MessagePrefix[ELLE_MAX][10] = {"[INFO] ", "[WARING] ", "[ERROR] ", "[DEBUG] "};
char TextTimeFormat[] = "%d-%02d-%02dT%02d:%02d:%02d.000%5s";
void SendTextLogImpl_(SLogMsgData *MsgData,ELogLevel type,const char* msg,va_list va)
{
	if(!MsgData || !msg)
        	return;
	memset(MsgData->Buff,0,sizeof(MsgData->Buff));
	char TextTime[64];
	char Msg[Free_LOGMSG_LEN];
	snprintf(TextTime,sizeof(TextTime),TextTimeFormat,NowSystemTime.Year,NowSystemTime.Month,NowSystemTime.Date,
    NowSystemTime.Hour,NowSystemTime.Minute,NowSystemTime.Second,"UTC-8  ");
	vsnprintf(Msg, sizeof(Msg), msg, va);
	strcat(MsgData->Buff,MessagePrefix[type]);
	strcat(MsgData->Buff,TextTime);
	strcat(MsgData->Buff,Msg);

#ifdef _WIN32
	if(type == ELLE_WARING)
		SetColor(40,30);
	else if(type == ELLE_ERROR)
		SetColor(FOREGROUND_RED,0);
#endif
	fprintf(stdout,MsgData->Buff);
	fprintf(stdout,"\r\n");
	fflush(stdout);
#ifdef _WIN32
	if(type != ELLE_INFO)
		SetColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE,0);
#endif
}

void SendTextLog(ELogLevel type,const char* msg,va_list va)
{
	assert(type < ELLE_MAX);
	SLogMsgData *MsgData = NULL;
	if(!LogThread::Instance()->IsLogTreadRuning())
	{
		cout << msg << endl;
	}
	else
	{
		MsgData = GObjFact->Create<SLogMsgData>();
		MsgData->BuffLen = strlen(msg)+76;
		SendTextLogImpl_(MsgData,type,msg,va);
		LogThread::Instance()->WriteLog<ELT_Text>(MsgData);
	}
}

void Show(const char* msg,...)
{
	va_list va;
	va_start(va,msg);
	SendTextLog(ELLE_INFO,msg,va);
	va_end(va);
}

void Warn(const char* msg,...)
{
	va_list va;
	va_start(va,msg);
	SendTextLog(ELLE_WARING,msg,va);
	va_end(va);
}

void Error(const char* msg,...)
{
	va_list va;
	va_start(va,msg);
	SendTextLog(ELLE_ERROR,msg,va);
	va_end(va);
}