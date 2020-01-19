#include "CJsonLog.h"
#include "../Common/Common.h"

#define JsonValue (*((Json::Value*)this))

CJson& CJson::operator=(QWORD other)
{
    char TempChar[128];
    snprintf(TempChar,sizeof(TempChar),"%" LL_FORMAT"U",other);
    JsonValue = TempChar;
    return *this;
}

CJson& CJson::operator=(INT64 other)
{
    char TempChar[128];
    snprintf(TempChar,sizeof(TempChar),"%" LL_FORMAT"d",other);
    JsonValue = TempChar;
    return *this;
}

CJson& CJson::operator=(CJson &other)
{
    JsonValue = *((Json::Value*)(&other));
    return *this;
}

void CJson::append(CJson &other)
{
    Json::Value::append(other);
}

void CJson::PushDataFormat(const char* key,const char* data,...)
{
    if(!data)
        return;
    char TempChar[2048];
    va_list va;
    va_start(va, data);
    vsnprintf(TempChar,sizeof(TempChar),data,va);
    if(key)
        JsonValue[key] = TempChar;
    else
        JsonValue[""] = TempChar;
    va_end(va);
}
#undef JsonValue

char JsonTimeFormat[] = "%d-%02d-%02dT%02d:%02d:%02d.000%5s";
void CJsonLog::WriteJsonLog(EJsonLogType LogType,ELogLevel LogLevel)
{
    CJsonLog &mJson = *this;
    if(mJson.empty() || LogLevel >= ELLE_MAX)
        return;
    char JsonTime[64];
    snprintf(JsonTime,sizeof(JsonTime),JsonTimeFormat,NowSystemTime.Year,NowSystemTime.Month,NowSystemTime.Date,
    NowSystemTime.Hour,NowSystemTime.Minute,NowSystemTime.Second,"UTC-8");
    mJson["logtime"] = JsonTime;
    mJson["type"] = LogType;
    mJson["loglevel"] = LogLevel;

    std::string out = mJson.toStyledString();
    int Pos = 0;
    while(Pos < out.length())
    {
        SLogMsgData* MsgData = GObjFact->Create<SLogMsgData>();
        if(!MsgData)
            return;
        int unreadsize = out.length() - Pos;
        MsgData->BuffLen = std::min<int>(unreadsize,sizeof(MsgData->Buff));
        memcpy(MsgData->Buff,out.c_str()+Pos,MsgData->BuffLen);
        LogThread::Instance()->WriteLog<ELT_Json>(MsgData);
        Pos += MsgData->BuffLen;
    }
}