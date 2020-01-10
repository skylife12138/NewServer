#include "Reflector.h"

Reflector::~Reflector()
{
	map<std::string, RObjectFactory*>::iterator it = objectFactories.begin();
	for (; it != objectFactories.end();++it)
	{
		delete it->second;
	}
	objectFactories.clear();
}

void Reflector::RegisterFactory(const string& className, RObjectFactory *of)
{
	map<string, RObjectFactory*>::iterator it = objectFactories.find(className);
	if (it != objectFactories.end()) 
	{
		std::cout << "arleady register yet!" << std::endl;
	}
	else 
	{
		objectFactories[className] = of;
	}
}

ReflectObject* Reflector::getNewInstance(const string& className)
{
	map<string, RObjectFactory*>::iterator it = objectFactories.find(className);
	if (it != objectFactories.end()) {
		RObjectFactory *of = it->second;
		return of->newInstance();
	}
	return NULL;
}