/*  PyMod - Python Scripting Engine for Half-Life
 *  Copyright (C) 2018  PyMod Development Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Python.h>
#include <IPyGlobal.hpp>

PyMod::IPyGlobal *gPyModInst;

PYMOD_API int PyMod_Query(PyMod::IPyGlobal *pymodInstance, unsigned long apiversion)
{
    gPyModInst = pymodInstance;

    return 1;
}

static PyObject *print(PyObject *self, PyObject *args)
{
    Py_RETURN_NONE;
}

static PyMethodDef natives[] = {
    { "printAsModule", print, METH_O, "Prints to console." },
    { nullptr, nullptr, 0, nullptr }
};

static struct PyModuleDef exampleModuleDef = {
    PyModuleDef_HEAD_INIT,
    "example",
    nullptr,
    -1,
    natives
};

PyMODINIT_FUNC PyInit_example()
{
    return PyModule_Create(&exampleModuleDef);
}
