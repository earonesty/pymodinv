#define PY_SSIZE_T_CLEAN

#include <Python.h>

static PyNumberMethods *nb = PyLong_Type.tp_as_number;

typedef struct {PyObject *g; PyObject *y; PyObject *x;} struct_gyx;

static PyObject * Py0 = PyLong_FromLong(0);
static PyObject * Py1 = PyLong_FromLong(1);
static struct_gyx egcd(PyObject *a, PyObject *b) {
    int ovr = 1;

    struct_gyx rval;

    /// if a == 0
    if (!PyLong_Type.tp_compare(Py0, a)) {
        Py_INCREF(b);
        rval.g = b;
        rval.y = PyLong_FromLong(0);
        rval.x = PyLong_FromLong(1);
        return rval;
    }

    PyObject *f1, *f2, *f3;

    struct_gyx ret = egcd(f3=nb->nb_remainder(b, a), a);

    Py_DECREF(f3);

    rval.g = ret.g;
    rval.y = nb->nb_subtract(ret.x, f1=nb->nb_multiply(f2=nb->nb_floor_divide(b, a), ret.y));
    rval.x = ret.y;

    Py_DECREF(ret.x);
    Py_DECREF(f1);
    Py_DECREF(f2);

    return rval;
}

static PyObject* pymodinv_modinv(PyObject *self, PyObject *args) {
    PyObject *a;
    PyObject *m;

    if(!PyArg_ParseTuple(args, "OO", &a, &m))
        return NULL;
    #if PY_MAJOR_VERSION <= 2
        if (PyInt_Check(a)) {
            long ia = PyInt_AS_LONG(a);
            Py_DECREF(a);
            a=PyLong_FromLong(ia);
        }
        if (PyInt_Check(m)) {
            long im = PyInt_AS_LONG(m);
            Py_DECREF(m);
            m=PyLong_FromLong(im);
        }
    #endif

    if (!PyLong_Check(a)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be an integer");
        return NULL;
    }

    struct_gyx ret = egcd(a, m);

    if (PyLong_Type.tp_compare(Py1, ret.g))
        PyErr_SetString(PyExc_ValueError, "modular invese does not exist");

    PyObject *res = nb->nb_remainder(ret.y, m);

    Py_DECREF(ret.g);
    Py_DECREF(ret.x);
    Py_DECREF(ret.y);

    return res;
}

static PyMethodDef pymodinvMethods[] = {
    {"modinv", pymodinv_modinv, METH_VARARGS, PyDoc_STR("modular inverse for large integers")},
    {NULL, NULL, 0, NULL},
};

#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef pymodinvDef = {
        PyModuleDef_HEAD_INIT,
        "pymodinv",
        NULL,
        -1,
        pymodinvMethods,
    };
	PyMODINIT_FUNC PyInit_pymodinv(void) {
		return PyModule_Create(&pymodinvDef);
	}
#else
	PyMODINIT_FUNC initpymodinv(void)
	{
		(void) Py_InitModule("pymodinv", pymodinvMethods);
	}
#endif
