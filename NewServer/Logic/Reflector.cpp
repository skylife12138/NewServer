#include "../Prec.h"

Reflector::~Reflector()
{
	map<std::string, ObjectFactory*>::iterator it = objectFactories.begin();
	for (; it != objectFactories.end();++it)
	{
		delete it->second;
	}
	objectFactories.clear();
}

void Reflector::RegisterFactory(const string& className, ObjectFactory *of)
{
	map<string, ObjectFactory*>::iterator it = objectFactories.find(className);
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
	map<string, ObjectFactory*>::iterator it = objectFactories.find(className);
	if (it != objectFactories.end()) {
		ObjectFactory *of = it->second;
		return of->newInstance();
	}
	return NULL;
}