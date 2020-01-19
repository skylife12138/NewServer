#include "Log.h"

char LogNameFormat[] = "%s/%04d_%02d_%02d_%02d_%02d_%02d_%s";
CLogFileWriter::CLogFileWriter()
{
	pFile = NULL;
	NowFileSize = 0;
	MaxFileSize = 0;
	OpenFileMod[0] = '\0';
	OpenFileDir[0] = '\0';
	OpenFileName[0] = '\0';
	WriteConfig = ELWC_NoConfig;
}

bool CLogFileWriter::InitConfig(const char* dir,const char* name,const char* wrmod,ELogWriteConfig WriteCon,int MaxFileLimit)
{
	if(!dir || !name || !wrmod)
		return false;
	MaxFileSize = MaxFileLimit;
	WriteConfig = WriteCon;
	snprintf(OpenFileMod,sizeof(OpenFileMod),wrmod);
	snprintf(OpenFileDir,sizeof(OpenFileDir),dir);
	snprintf(OpenFileName,sizeof(OpenFileName),name);
	return OpenLogFile();
}

void CLogFileWriter::WriteAllLog()
{
	while(!RecvLogMsgVec.empty())
	{
		WriteLog();
	}
}

void CLogFileWriter::WriteLog()
{
	if(RecvLogMsgVec.empty())
        return;
    SLogMsgData *LogMsg = RecvLogMsgVec.front();
    assert(LogMsg);
	RecvLogMsgVec.pop();

	if(!LogMsg || LogMsg->BuffLen == 0)
	{
		GObjFact->Destory<SLogMsgData>(LogMsg);
		LogMsg = NULL;
		return;
	}
	if(!pFile)
	{
		GObjFact->Destory<SLogMsgData>(LogMsg);
		LogMsg = NULL;
		return;
	}
	fwrite(LogMsg->Buff,LogMsg->BuffLen,1,pFile);
	NowFileSize += LogMsg->BuffLen;
	if(WriteConfig & ELWC_AutoNewLine)
	{
#ifdef _WIN32
		NowFileSize += fprintf(pFile,"\r\n");
#else
		NowFileSize += fprintf(pFile,"\n");
#endif		
	}
	Fulsh();
	CheckLogFileLimit();

	GObjFact->Destory<SLogMsgData>(LogMsg);
    LogMsg = NULL;
}

void CLogFileWriter::CheckLogFileLimit()
{
	if(!pFile || !MaxFileSize)
		return;
	if(NowFileSize < MaxFileSize)
		return;
	CloseLogFile();
	if(WriteConfig & ELWC_FullBackFile)
	{
		char OldName[512];
		snprintf(OldName,sizeof(OldName),"%s%s",OpenFileDir,OpenFileName);
		struct stat st;
		if(stat(OldName,&st) == 0)//通过文件名获取文件信息，成功返回0
		{
			char NewName[512];
    		snprintf(NewName,sizeof(NewName),LogNameFormat,OpenFileDir,NowSystemTime.Year,NowSystemTime.Month,NowSystemTime.Date,
    		NowSystemTime.Hour,NowSystemTime.Minute,NowSystemTime.Second,OpenFileName);
			rename(OldName,NewName);
			OpenLogFile();
		}
	}
	else if(WriteConfig & ELWC_FullNewFile)
	{
		OpenLogFile();
	}
	else
	{
		assert(false);
		return;
	}
}

bool CLogFileWriter::OpenLogFile()
{
	if(pFile)
		return true;
	if(!strlen(OpenFileMod) || !strlen(OpenFileName))
		return false;
	struct stat st;
	if(strlen(OpenFileDir) && stat(OpenFileDir,&st))
	{
#ifdef _WIN32
		mkdir(OpenFileDir);
#else
		mkdir(OpenFileDir,0755);
#endif
	}
	char FullName[512];
	if(WriteConfig & ELWC_FullNewFile)
	{
		snprintf(FullName,sizeof(FullName),LogNameFormat,OpenFileDir,NowSystemTime.Year,NowSystemTime.Month,NowSystemTime.Date,
    		NowSystemTime.Hour,NowSystemTime.Minute,NowSystemTime.Second,OpenFileName);
	}
	else
	{
		snprintf(FullName,sizeof(FullName),"%s/%s",OpenFileDir,OpenFileName);
	}
	pFile = fopen(FullName,OpenFileMod);
	if(!pFile)
		return false;
	memset(&st,0,sizeof(st));
	stat(FullName,&st);
	NowFileSize = st.st_size;
	return true;
}

void CLogFileWriter::CloseLogFile()
{
	if(pFile)
	{
		fflush(pFile);
		fclose(pFile);
		pFile = NULL;
	}
}

void CLogFileWriter::Fulsh()
{
	if(pFile)
		fflush(pFile);
}

void CLogFileWriter::PushLogMsg(SLogMsgData* LogMsg)
{
	RecvLogMsgVec.push(LogMsg);
}


LogThread::LogThread()
{
#define REGLOGMSG(s) Hander[s] = &LogThread::WriteLog<s>
	REGLOGMSG(ELT_Text);
	REGLOGMSG(ELT_Json);
#undef REGLOGMSG
	JsonLogWriter.InitConfig("logs/jsonlogs","log.json","a",ELWC_DefaultConfig,500000);
	TextLogWriter.InitConfig("logs","log.txt","a",ELWC_DefaultConfig);
	IsLogTreadRun = false;
}

LogThread::~LogThread()
{
	
}

void LogThread::Release()
{
	if (!I_IsStop)
		Stop();
}

const int MinDeltaTickCount = 1000 / 15;
void LogThread::DoThread()
{
	DWORD LogThreadTickCount = 0;
	DWORD LastTickTime = TimeGetTime();
	int Moredelta = 0;
	IsLogTreadRun = true;
	while(!I_IsStop)
	{
		LogThreadTickCount = TimeGetTime();

		DWORD DeltaTickCount = LogThreadTickCount - LastTickTime;
		if((int)DeltaTickCount > MinDeltaTickCount - Moredelta)
		{
			JsonLogWriter.WriteLog();
			TextLogWriter.WriteLog();

			LastTickTime = LogThreadTickCount;
			Moredelta += (DeltaTickCount - MinDeltaTickCount);
			if(Moredelta > 2*MinDeltaTickCount)
				Moredelta = 2 * MinDeltaTickCount;
			else if(Moredelta < 0)
				Moredelta = 0;
		}
		else if((int)DeltaTickCount < MinDeltaTickCount)
		{
			SLEEP(MinDeltaTickCount - DeltaTickCount);
		}
	}

	JsonLogWriter.WriteAllLog();
	TextLogWriter.WriteAllLog();

	JsonLogWriter.CloseLogFile();
	TextLogWriter.CloseLogFile();
	IsLogTreadRun = false;
}

template <>void LogThread::WriteLog<ELT_Text>(SLogMsgData* LogMsg)
{
	if(!LogMsg)
		return;
	TextLogWriter.PushLogMsg(LogMsg);
}

template <>void LogThread::WriteLog<ELT_Json>(SLogMsgData* LogMsg)
{
	if(!LogMsg)
		return;
	JsonLogWriter.PushLogMsg(LogMsg);
}