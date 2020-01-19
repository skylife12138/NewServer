#ifndef _CJSONLOG_H_
#define _CJSONLOG_H_
#include "../json/include/json.h"
#include "../Common/Portable.h"
#include "../Common/Log.h"
#include <string>

enum EJsonLogType
{
    EJLT_First = 0,

    EJLT_Max,
};

class CJson : public Json::Value
{
#define DefaultEqual(t) *((Json::Value *)this) = (t); return *this;
#define DefaultBrack(t) return *((CJson *)&((*((Json::Value *)this))[(t)]));
public:
    CJson& operator[](const std::string& key){DefaultBrack(key)}
    CJson& operator[](const char* key){DefaultBrack(key?key:"")}
    CJson& operator[](unsigned int index){DefaultBrack((Json::UInt)index)}
    CJson& operator[](int index){DefaultBrack(index)}
    CJson& operator=(int other){DefaultEqual((Json::Int)other)}
    CJson& operator=(WORD other){DefaultEqual((Json::UInt)other)}
    CJson& operator=(unsigned int other){DefaultEqual((Json::UInt)other)}
    CJson& operator=(unsigned long other){DefaultEqual((Json::UInt64)other)}
    CJson& operator=(std::string& other){DefaultEqual(other)}
    CJson& operator=(const std::string& other){DefaultEqual(other)}
    CJson& operator=(double other){DefaultEqual(other)}
    CJson& operator=(bool other){DefaultEqual(other)}
    CJson& operator=(BYTE other){DefaultEqual(other)}
    CJson& operator=(CJson::Value& other){DefaultEqual(other)}
    CJson& operator=(const char* other){DefaultEqual(other?other:"")}
    CJson& operator=(QWORD other);
    CJson& operator=(INT64 other);
    CJson& operator=(CJson &other);

    void append(CJson &other);
    void PushDataFormat(const char* key,const char* data,...);
#undef DefaultEqual
#undef DefaultBrack
};

class CJsonLog : public CJson
{
public:
    void WriteJsonLog(EJsonLogType LogType,ELogLevel LogLevel = ELLE_INFO);
};
#endif