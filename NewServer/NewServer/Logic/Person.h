#ifndef PERSON_H_
#define PERSON_H_
#include "Reflector.h"

// »√ Person ºÃ≥–∑¥…‰ª˘¿‡
class Person : public ReflectObject
{
public:
	Person();
	virtual ~Person();
	virtual void show();
};

class Allen : public Person
{
public:
	Allen();
    virtual ~Allen();
	virtual void show();
};

class Luffy : public Person
{
public:
	Luffy();
	virtual ~Luffy();
	virtual void show();
};
#endif