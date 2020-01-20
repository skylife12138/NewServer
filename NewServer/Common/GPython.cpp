#include "GPython.h"
#include "Common.h"
#include "Log.h"

#define PYTHON_MODULE_DIR	"./pythonscript/"

#define PYTHON_MIAN_SCRIPT		"main_script"
#define PYTHON_INIT_FUNCTION	"init"
#define PYTHON_TICK_FUNCTION	"tick"
#define PYTHON_FINAL_FUNCTION	"final"

static PyObject* py_log(PyObject* self, PyObject* args)
{
	uint16_t log_type = ELLE_INFO;
	char* log_message = NULL;
	if (!PyArg_ParseTuple(args, "Hs",&log_type, &log_message)) {
		return NULL;
	}
	switch(log_type)
	{
		case ELLE_INFO:
			Show(log_message);
			break;
		case ELLE_WARING:
			Warn(log_message);
			break;
		case ELLE_ERROR:
			Error(log_message);
			break;
		default:
			break;
	}
    Py_RETURN_NONE;
}

//Register function for python
static PyMethodDef PyServerEngine_Methods[] = {
    {"py_log",py_log,METH_VARARGS,"show logs"},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef PyServerEngine_ModuleDef = {
	PyModuleDef_HEAD_INIT,
	"ServerEngine",
	NULL,
	-1,
	PyServerEngine_Methods,
};

PyMODINIT_FUNC
PyInit_ServerEngine(void) {
	return PyModule_Create(&PyServerEngine_ModuleDef);
}

GPython::GPython()
{
#ifdef _WIN32
	Py_SetPythonHome(L"C:/Users/duoyi/AppData/Local/Programs/Python/Python37");
#else

#endif
    PyImport_AppendInittab("ServerEngine", &PyInit_ServerEngine);

	Py_Initialize();
	PyEval_InitThreads();

    //将Python工作路径切换到待调用模块所在目录
	Exec("import sys;sys.path = []");
	Insert_Path(0, PYTHON_MODULE_DIR);

	Call_Python_Fun(PYTHON_MIAN_SCRIPT, PYTHON_INIT_FUNCTION);
}

GPython::~GPython()
{
	Call_Python_Fun(PYTHON_MIAN_SCRIPT, PYTHON_FINAL_FUNCTION);
	Py_FinalizeEx();
}

void GPython::Insert_Path(int Pos,const char* Path)
{
    assert(Py_IsInitialized());
	const int sz = 256;
	char tPath[sz];

	int n = _snprintf_s(tPath, sz, "import sys\nif '%s' not in sys.path: sys.path.insert(%u, '%s')", Path, Pos, Path);
	if (n < 0 || n >= sz) {
		Show("Failed to insert python path, (%d) bytes is written while only support (%d) max", n, sz);
		return;
	}
	Exec(tPath);
}

bool GPython::Exec(const char* path)
{
    PyRun_SimpleString(path);
    return true;
}

bool GPython::Call_Python_Fun(const char* MoudleName,const char* FunctionName,PyObject* Args)
{
    PyObject* pyModule = PyImport_ImportModule(MoudleName);
	if (pyModule == NULL) {
		Show("Failed to import module(%s)", MoudleName);
		return false;
	}

	PyObject* pyFunction = PyObject_GetAttrString(pyModule, FunctionName);
	if (pyFunction == NULL) {
		Show("Failed to find function(%s) in module(%s)", FunctionName, MoudleName);
		Py_DECREF(pyModule);
		return false;
	}

	PyObject* pyResult = PyObject_CallObject(pyFunction, Args);
	if (pyResult == NULL) {
		Show("Failed to call function(%s) in module(%s)", FunctionName, MoudleName);
		Py_DECREF(pyModule);
		Py_DECREF(pyFunction);
		return false;
	}

	Py_DECREF(pyModule);
	Py_DECREF(pyFunction);
	Py_DECREF(pyResult);
	return true;
}

void GPython::Tick()
{
	Call_Python_Fun(PYTHON_MIAN_SCRIPT, PYTHON_TICK_FUNCTION);
}