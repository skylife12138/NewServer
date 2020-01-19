
#ifndef _LOG_H_
#define _LOG_H_

#include <queue>
#include "Thread.h"
#include "Common.h"

//日志记录类型
enum ELogType
{
	ELT_Text = 0,
	ELT_Json = 1,

	ELT_Max,
};

//日志等级
enum ELogLevel
{
    ELLE_INFO = 0,
    ELLE_WARING = 1,
    ELLE_ERROR = 2,
	ELLE_DEBUG = 3,

    ELLE_MAX,
};

enum ELogWriteConfig
{
	ELWC_NoConfig = 0,
	ELWC_AutoNewLine = 1,
	ELWC_FullBackFile = 2,
	ELWC_FullNewFile = 4,
	ELWC_DefaultConfig = ELWC_AutoNewLine|ELWC_FullNewFile,

	ELWC_Max,
};

class LogThread;
struct SLogMsgData
{
	explicit SLogMsgData():BuffLen(Max_LOGMAG_LEN){};
	DWORD BuffLen;
	char Buff[Max_LOGMAG_LEN];
};

class CLogFileWriter
{
public:
	CLogFileWriter();
	~CLogFileWriter() { CloseLogFile(); }
	bool InitConfig(const char* dir,const char* name,const char* wrmod,ELogWriteConfig WriteCon,int MaxFileLimit=0);
	void PushLogMsg(SLogMsgData* LogMsg);
	void WriteLog();
	void WriteAllLog();

	void CloseLogFile();
private:
	void CheckLogFileLimit();
	bool OpenLogFile();
	void Fulsh();

private:
	FILE *pFile;
	int NowFileSize;
	int MaxFileSize;
	char OpenFileMod[16];
	char OpenFileDir[256];
	char OpenFileName[256];
	ELogWriteConfig WriteConfig;

	std::queue<SLogMsgData *> RecvLogMsgVec;//后续用无锁队列,因为接收和解析在不同线程，所以使用队列
};

typedef void (LogThread::*LOGCALLBACK)(SLogMsgData *p);
class LogThread : public BaseThread,public SingleTon<LogThread>
{
public:
	LogThread();
    virtual ~LogThread();
	template <ELogType m>void WriteLog(SLogMsgData* LogMsg);

	virtual void DoThread();
	void Release();

	bool IsLogTreadRuning() { return IsLogTreadRun; }
private:
	bool IsLogTreadRun;
	LOGCALLBACK Hander[ELT_Max];
	CLogFileWriter JsonLogWriter;
	CLogFileWriter TextLogWriter;
};

template <>void LogThread::WriteLog<ELT_Text>(SLogMsgData *LogMsg);
template <>void LogThread::WriteLog<ELT_Json>(SLogMsgData *LogMsg);

#endif