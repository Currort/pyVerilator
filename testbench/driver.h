#ifndef DRIVER_H_
#define DRIVER_H_
#include "VSOC.h"
class driver
{
private:
    VSOC *dut;
public:
    driver::driver(VSOC *dut);
    void rst();
    void start();
};


#endif