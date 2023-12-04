#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vpi.h>
#include <verilated_vcd_c.h>
#include "VSOC.h"
#include "VSOC___024root.h"
#include "VSOC__Syms.h"
#include "monitor.h"
#define MAX_SIM_TIME 60
#define MAX_RST_TIME 20
vluint64_t sim_time = 0;


int main(int argc, char** argv, char** env) {    
    VSOC *dut = new VSOC;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");
    int flag  = 0;
    dut->clk_sys_i = 1;
    dut->rst_i     = 1;
    // Verilated::scopesDump();

    const std::vector<std::string> key_IF = {"pc","pause","jump","instr"};
    const std::vector<std::string> key_RegFile  ={"zero","ra","sp","gp",
                "tp","t0","t1","t2","t3","t4","t5","t6",
                "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
                "a0","a1","a2","a3","a4","a5","a6","a7"};
    const uint8_t fifo_deepth = 8;
    MON::monitor monitor(dut, rise_trigger, fifo_deepth,  key_IF, key_RegFile);

    std::vector<std::string> read_name = {"pc","instr","gp","jump",};
    std::vector<uint8_t> read_index = {0,1,2,3,4};
    std::map<std::string, uint8_t> name_index = {{"pc", 3}, {"instr",3}, {"gp",7}, {"jump",4}};

    while (sim_time < MAX_SIM_TIME) {
        dut->clk_sys_i ^= 1;
        if(sim_time == MAX_RST_TIME) dut->rst_i =0;
        dut->eval();
        monitor.read();
        if((monitor.clk_cnt>= MAX_RST_TIME/2 +5 )&&(monitor.check_clk())){
            monitor.print_monitor();
            monitor.print_clk();
            // monitor.printf(read_name, fifo_deepth);
            if((monitor.dict["jump"][4] == "1")|(monitor.dict["jump"][3] == "1")|(monitor.dict["jump"][2] == "1")){
                printf("    jump ! \n");
            }else{
                monitor.printf(name_index, hide);
            }
            
        }  
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(EXIT_SUCCESS);
}


