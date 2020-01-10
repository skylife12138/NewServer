#ifndef _OBJECTFACTORY_H_
#define _OBJECTFACTORY_H_

struct GenerateObjectPoolAggregation;
class ObjectFactory
{
public:
    ObjectFactory();
    ~ObjectFactory();

    //对象池
    template <class T>T *Create();
    template <class T,class P>T *Create(P p);
    template <class T,class P1,class P2>T *Create(P1 p1,P2 p2);
    template <class T> void Destory(T* obj);

    //多态对象池
    template <class T>T *New();
    template <class T,class P>T *New(P p);
    template <class T,class P1,class P2>T *New(P1 p1,P2 p2);
    template <class T> void Delete(T* obj);

    void ShowState();
    template <class T>bool NotEnough();
private:
    GenerateObjectPoolAggregation *_impl;
};
#endif