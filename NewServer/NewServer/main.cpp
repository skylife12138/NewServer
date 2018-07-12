#include "Prec.h"

int main(int argc,char** argv)
{
    if(argc==2)
    {
        if(!strcmp(argv[1],"-v"))
        {
            //查版本号
            cout << "Project Version: %d\n"<< 12 << endl;
            return 1;
        }
    }
    GProMgr = new GProjectMgr;
    bool Ret = GProMgr->Init();
	if (!Ret)
	{
		cout << "Project Init Error!" << endl;
		return 1;
	}
    while(!GProMgr->IsExit())
    {
      GProMgr->MainLoop();
    }
    system("pause");
    return 0;
}