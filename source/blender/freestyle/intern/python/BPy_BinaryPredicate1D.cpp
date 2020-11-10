/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/** \file
 * \ingroup freestyle
 */

#include "BPy_BinaryPredicate1D.h"

#include "BPy_Convert.h"
#include "BPy_Interface1D.h"

#include "BinaryPredicate1D/BPy_FalseBP1D.h"
#include "BinaryPredicate1D/BPy_Length2DBP1D.h"
#include "BinaryPredicate1D/BPy_SameShapeIdBP1D.h"
#include "BinaryPredicate1D/BPy_TrueBP1D.h"
#include "BinaryPredicate1D/BPy_ViewMapGradientNormBP1D.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////

//-------------------MODULE INITIALIZATION--------------------------------
int BinaryPredicate1D_Init(PyObject *module)
{
  if (module == nullptr) {
    return -1;
  }

  if (PyType_Ready(&BinaryPredicate1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&BinaryPredicate1D_Type);
  PyModule_AddObject(module, "BinaryPredicate1D", (PyObject *)&BinaryPredicate1D_Type);

  if (PyType_Ready(&FalseBP1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&FalseBP1D_Type);
  PyModule_AddObject(module, "FalseBP1D", (PyObject *)&FalseBP1D_Type);

  if (PyType_Ready(&Length2DBP1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&Length2DBP1D_Type);
  PyModule_AddObject(module, "Length2DBP1D", (PyObject *)&Length2DBP1D_Type);

  if (PyType_Ready(&SameShapeIdBP1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&SameShapeIdBP1D_Type);
  PyModule_AddObject(module, "SameShapeIdBP1D", (PyObject *)&SameShapeIdBP1D_Type);

  if (PyType_Ready(&TrueBP1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&TrueBP1D_Type);
  PyModule_AddObject(module, "TrueBP1D", (PyObject *)&TrueBP1D_Type);

  if (PyType_Ready(&ViewMapGradientNormBP1D_Type) < 0) {
    return -1;
  }
  Py_INCREF(&ViewMapGradientNormBP1D_Type);
  PyModule_AddObject(module, "ViewMapGradientNormBP1D", (PyObject *)&ViewMapGradientNormBP1D_Type);

  return 0;
}

//------------------------INSTANCE METHODS ----------------------------------

static char BinaryPredicate1D___doc__[] =
    "Base class for binary predicates working on :class:`Interface1D`\n"
    "objects.  A BinaryPredicate1D is typically an ordering relation\n"
    "between two Interface1D objects.  The predicate evaluates a relation\n"
    "between the two Interface1D instances and returns a boolean value (true\n"
    "or false).  It is used by invoking the __call__() method.\n"
    "\n"
    ".. method:: __init__()\n"
    "\n"
    "   Default constructor.\n"
    "\n"
    ".. method:: __call__(inter1, inter2)\n"
    "\n"
    "   Must be overload by inherited classes. It evaluates a relation\n"
    "   between two Interface1D objects.\n"
    "\n"
    "   :arg inter1: The first Interface1D object.\n"
    "   :type inter1: :class:`Interface1D`\n"
    "   :arg inter2: The second Interface1D object.\n"
    "   :type inter2: :class:`Interface1D`\n"
    "   :return: True or false.\n"
    "   :rtype: bool\n";

static int BinaryPredicate1D___init__(BPy_BinaryPredicate1D *self, PyObject *args, PyObject *kwds)
{
  static const char *kwlist[] = {nullptr};

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "", (char **)kwlist)) {
    return -1;
  }
  self->bp1D = new BinaryPredicate1D();
  self->bp1D->py_bp1D = (PyObject *)self;
  return 0;
}

static void BinaryPredicate1D___dealloc__(BPy_BinaryPredicate1D *self)
{
  delete self->bp1D;
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *BinaryPredicate1D___repr__(BPy_BinaryPredicate1D *self)
{
  return PyUnicode_FromFormat("type: %s - address: %p", Py_TYPE(self)->tp_name, self->bp1D);
}

static PyObject *BinaryPredicate1D___call__(BPy_BinaryPredicate1D *self,
                                            PyObject *args,
                                            PyObject *kwds)
{
  static const char *kwlist[] = {"inter1", "inter2", nullptr};
  BPy_Interface1D *obj1, *obj2;

  if (!PyArg_ParseTupleAndKeywords(args,
                                   kwds,
                                   "O!O!",
                                   (char **)kwlist,
                                   &Interface1D_Type,
                                   &obj1,
                                   &Interface1D_Type,
                                   &obj2)) {
    return nullptr;
  }
  if (typeid(*(self->bp1D)) == typeid(BinaryPredicate1D)) {
    PyErr_SetString(PyExc_TypeError, "__call__ method not properly overridden");
    return nullptr;
  }
  if (self->bp1D->operator()(*(obj1->if1D), *(obj2->if1D)) < 0) {
    if (!PyErr_Occurred()) {
      string class_name(Py_TYPE(self)->tp_name);
      PyErr_SetString(PyExc_RuntimeError, (class_name + " __call__ method failed").c_str());
    }
    return nullptr;
  }
  return PyBool_from_bool(self->bp1D->result);
}

/*----------------------BinaryPredicate0D get/setters ----------------------------*/

PyDoc_STRVAR(BinaryPredicate1D_name_doc,
             "The name of the binary 1D predicate.\n"
             "\n"
             ":type: str");

static PyObject *BinaryPredicate1D_name_get(BPy_BinaryPredicate1D *self, void *UNUSED(closure))
{
  return PyUnicode_FromString(Py_TYPE(self)->tp_name);
}

static PyGetSetDef BPy_BinaryPredicate1D_getseters[] = {
    {"name",
     (getter)BinaryPredicate1D_name_get,
     (setter) nullptr,
     BinaryPredicate1D_name_doc,
     nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr} /* Sentinel */
};

/*-----------------------BPy_BinaryPredicate1D type definition ------------------------------*/
PyTypeObject BinaryPredicate1D_Type = {
    PyVarObject_HEAD_INIT(nullptr, 0) "BinaryPredicate1D", /* tp_name */
    sizeof(BPy_BinaryPredicate1D),                         /* tp_basicsize */
    0,                                                     /* tp_itemsize */
    (destructor)BinaryPredicate1D___dealloc__,             /* tp_dealloc */
    /* Incompatible with Python3.8+ (deprecated function).
     * NOLINTNEXTLINE: modernize-use-nullptr. */
    0,                                        /* tp_print */
    nullptr,                                  /* tp_getattr */
    nullptr,                                  /* tp_setattr */
    nullptr,                                  /* tp_reserved */
    (reprfunc)BinaryPredicate1D___repr__,     /* tp_repr */
    nullptr,                                  /* tp_as_number */
    nullptr,                                  /* tp_as_sequence */
    nullptr,                                  /* tp_as_mapping */
    nullptr,                                  /* tp_hash  */
    (ternaryfunc)BinaryPredicate1D___call__,  /* tp_call */
    nullptr,                                  /* tp_str */
    nullptr,                                  /* tp_getattro */
    nullptr,                                  /* tp_setattro */
    nullptr,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    BinaryPredicate1D___doc__,                /* tp_doc */
    nullptr,                                  /* tp_traverse */
    nullptr,                                  /* tp_clear */
    nullptr,                                  /* tp_richcompare */
    0,                                        /* tp_weaklistoffset */
    nullptr,                                  /* tp_iter */
    nullptr,                                  /* tp_iternext */
    nullptr,                                  /* tp_methods */
    nullptr,                                  /* tp_members */
    BPy_BinaryPredicate1D_getseters,          /* tp_getset */
    nullptr,                                  /* tp_base */
    nullptr,                                  /* tp_dict */
    nullptr,                                  /* tp_descr_get */
    nullptr,                                  /* tp_descr_set */
    0,                                        /* tp_dictoffset */
    (initproc)BinaryPredicate1D___init__,     /* tp_init */
    nullptr,                                  /* tp_alloc */
    PyType_GenericNew,                        /* tp_new */
};

///////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif
