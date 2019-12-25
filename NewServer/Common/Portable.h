#ifndef _PORT_H
#define _PORT_H

#ifdef _MSC_VER
#define LONGLONG(x) x##I64
#define ULONGLONG(x) x##UI64
#define LL_FORMAT "I64"
#else
#define LONGLONG(x) x##LL
#define ULONGLONG(x) x##ULL
#define LL_FORMAT "11"
#endif
#include <limits.h>

typedef unsigned int CodeType;

#ifdef _WIN32
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#ifndef _WINSOCK2API_
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#endif
#endif
#include <windows.h>
#include <fcntl.h>

#define socklen_t int
#define atoll _atoi64
typedef __int64 int64;
typedef __int64 BIGint;
typedef unsigned __int64 QWORD;
typedef __int64 INT64;
#define SLEEP(a) Sleep(a)
const int SOMAXCONECT = 2048;
int snprintf(char *buf, size_t bufsize, const char *format, ...);
int inet_aton(register const char *cp, struct in_addr *addr);
int inet_pton(int af,register const char *cp,struct in_addr *addr);

#else //_WIN32

#include <thread>
#include <sys/types.h>
#include <socketapi.h>
#include <InetReg.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <tcpioctl.h>
#include <time.h>
typedef unsigned long long int QWORD;
typedef long long int INT64;
typedef char TCHAR;
typedef char CHAR;
typedef short SHORT;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef int INT;
typedef long LONG;
typedef void* LPVOID;
typedef unsigned int DWORD_PTR;
typedef float FLOAT:
typedef double DOUBLE;
typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef unsigned long ULONG_PTR, *PULONG_PTR;
typedef ULONG_PTR SIZE_T;
#define MAX_PATH 260
typedef int SOCKET;
#define  closesocket close
const int INVALID_SOCKET = -1;
const char MINCHAR = Ox80;
const char MAXCHAE = 0x7f;
const short MINSHORT = 0x8000;
const short MAXSHORT = 0x7fff;
const int MININT = 0x80000000;
const int MAXINT = 0x7fffffff;
const BYTE MAXBYTE = 0xff;
const WORD MAXWORD = 0xffff;
const DWORD MAXDWORD = 0xffffffff;
#define TEXT(s) (s)
#define SLEEP(a) usleep((a)*1000)
#define stricmp strcasecmp
const int SOMAXCONNECT = 2048;
DWORD timeGetTime();
#endif //_WIN32

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define MAKEQWORD(low,high)  ((QWORD)(low) | ((QWORD)(high)<<32))
#define LODWORD(l)           ((DWORD)(l))
#define HIDWORD(l)           ((DWORD)(((QWORD)(l) >> 32) & 0xffffffff))
void InitTimeGetTime();

const QWORD MAXQWORD = ULONGLONG(0xffffffffffffffff);

const unsigned ByteSize = sizeof(BYTE);
const unsigned WordSize = sizeof(WORD);
const unsigned DwordSize = sizeof(DWORD);
const unsigned QwordSize = sizeof(QWORD);
const unsigned PtrSize = sizeof(void*);

#define SAFE_DELETE(p)  {if((p) != NULL) {delete (p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if((p) != NULL) {delete[] (p); (p)=NULL;}}

#define resetbits(x,m) ((x) &= ~(m))


typedef char int8;
typedef short int16;
typedef int int32;
//typedef unsigned char int8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char byte;

#ifdef timeout
#undef timeout
#endif
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#endif // !_PORT_H

