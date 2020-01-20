#include "../python3.7/include/Python.h"
#include "../Logic/SingleTon.h"

class GPython:public SingleTon<GPython>
{
public:
    GPython();
    ~GPython();

    void Tick();
private:
    bool Call_Python_Fun(const char* MoudleName,const char* FunctionName,PyObject* Args=NULL);
    void Insert_Path(int Pos,const char* Path);
    bool Exec(const char* path);
};