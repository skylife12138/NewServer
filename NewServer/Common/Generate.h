#include "./Common/Portable.h"
#ifdef WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

int GenUuid()
{
#ifdef WIN32
	GUID Uuid;
	CoCreateGuid(&Uuid);//获取的是一个字符串
#else
	uuid_t uu;
	uuid_generate(uu);
#endif
	return 0;
}