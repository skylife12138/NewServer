#ifndef REFLECTOR_H_
#define REFLECTOR_H_

#include"Prec.h"
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

//获取反射器实例，全局唯一
#define GReflectMgr Reflector::Instance()

//需要被反射的类，需要在其对应的 cpp 文件中进行反射声明
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

//根据类名获取对象实例
template<typename T>
T* getNewInstance(const std::string& className) {
	return dynamic_cast<T*>(GReflectMgr->getNewInstance(className));
}

#endif