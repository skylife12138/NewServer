#ifndef REFLECTOR_H_
#define REFLECTOR_H_

#include"../Prec.h"
#include"SingleTon.h"
using namespace std;

class ReflectObject
{
public:
  virtual ~ReflectObject(){}
};

class ObjectFactory
{
public:
  ObjectFactory() { cout << "ObjectFactory()" << endl; }
  virtual ~ObjectFactory() { cout << "~ObjectFactory" << endl; }
  virtual ReflectObject* newInstance() = 0;
};

class Reflector:public SingleTon<Reflector>
{
public:
  Reflector() {};
  ~Reflector();
  void RegisterFactory(const string &classname, ObjectFactory *of);
  ReflectObject *getNewInstance(const string& classname);
private:
  map<string, ObjectFactory*> objectFactories;
};


#define GReflectMgr Reflector::Instance()


#define REFLECT(name)\
class ObjectFactory_##name : public ObjectFactory{\
public:\
    ObjectFactory_##name(){ std::cout << "ObjectFactory_" << #name << "()" << std::endl; }\
    virtual ~ObjectFactory_##name(){ std::cout << "~ObjectFactory_" << #name << "()" << std::endl; }\
    ReflectObject* newInstance() {\
        return new name(); \
    }\
}; \
class Register_##name{\
public:\
    Register_##name(){\
        GReflectMgr->RegisterFactory(#name, new ObjectFactory_##name()); \
    }\
};\
Register_##name register_##name;


template<typename T>
T* getNewInstance(const std::string& className) {
	return dynamic_cast<T*>(GReflectMgr->getNewInstance(className));
}

#endif