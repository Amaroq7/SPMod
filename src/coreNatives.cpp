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

#include <pymod.hpp>

static PyObject *core_printToConsole(PyObject *self, PyObject *args)
{
    std::stringstream msg;
    msg << PyUnicode_AsUTF8(args) << '\n';
    SERVER_PRINT(msg.str().c_str());

    Py_RETURN_NONE;
}

static PyMethodDef coreNatives[] = {
    { "printToConsole", core_printToConsole, METH_O, "Prints to console." },
    { nullptr, nullptr, 0, nullptr }
};

static struct PyModuleDef coreModuleDef = {
    PyModuleDef_HEAD_INIT,
    "core",
    nullptr,
    -1,
    coreNatives
};

PyMODINIT_FUNC PyInit_core()
{
    return PyModule_Create(&coreModuleDef);
}
