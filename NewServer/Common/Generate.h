#ifndef _GENERATE_H_
#define _GENERATE_H_

#include "./Common/Portable.h"
#ifdef _WIN32
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

inline int GenUuid()
{
#ifdef _WIN32
	GUID Uuid;
	CoCreateGuid(&Uuid);
#else
	uuid_t uu;
	uuid_generate(uu);
#endif
	return 0;
}

#endif