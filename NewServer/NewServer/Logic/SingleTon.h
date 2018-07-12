#ifndef _SINGLE_TON_H
#define _SINGLE_TON_H
#include <assert.h>

template<typename T>
class SingleTon
{
public:
  inline static void StaticInit()
  {
      pInstance = new T;
      assert(pInstance);
  }
  inline static void StaticDestory()
  {
      delete pInstance;
  }
  inline static T *Instance() { 
      assert(pInstance);
      return pInstance;
  }
protected:
  static T* pInstance;
  SingleTon(void){}
  virtual ~SingleTon(void){}
};
template <typename T>
T *SingleTon<T>::pInstance = NULL;
#endif