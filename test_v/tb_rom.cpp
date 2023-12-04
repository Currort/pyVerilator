#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "./obj_dir/Vrom.h"
#include "./obj_dir/Vrom___024root.h"
#include "Python.h"
#define MAX_SIM_TIME 20
vluint64_t sim_time = 0;

int main(int argc, char** argv, char** env) {
    Py_Initialize();
    // 调用Python脚本
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyObject* pModule = PyImport_ImportModule("hello");
    if (pModule) {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "hello");
        if (pFunc && PyCallable_Check(pFunc)) {
            // 准备函数参数
            PyObject *pArgs = PyTuple_New(0);
            // 调用函数
            PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
            // 处理函数返回值
            if (pResult) {
                // 假设返回值是整数
                long result = PyLong_AsLong(pResult);
                // 在这里处理返回值
                Py_DECREF(pResult);
            }
            Py_DECREF(pArgs);
        }
        Py_DECREF(pFunc);
        Py_DECREF(pModule);
    }
    // 清理Python解释器
    Py_Finalize();



    Vrom *dut = new Vrom;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");

    while (sim_time < MAX_SIM_TIME) {
        dut->data_addr_i += 1;
        dut->eval();
        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}


