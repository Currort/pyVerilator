#include "driver.h"

driver::driver(VSOC *dut)
{
    this->dut = dut;
}

void driver::rst(unsigned long int posedge_cnt, unsigned long int rst_cnt_max,uint8_t rst_polarity){
    
};
