#include <stdlib.h>
#include <iostream>
#include <verilated.h>
#include <verilated_vpi.h>
#include <verilated_vcd_c.h>
#include "VSOC.h"
#include "VSOC___024root.h"
#include "VSOC__Syms.h"
#include "monitor.h"
#define MAX_SIM_TIME 800
#define MAX_RST_TIME 20
vluint64_t sim_time = 0;

mon::eqs eqs_oringe_init(mon::value_index fifo_deept);

int main(int argc, char** argv, char** env) {    
    VSOC *dut = new VSOC;
    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");
    int flag  = 0;
    dut->clk_sys_i = 1;
    dut->rst_sys_i     = 1;
    // Verilated::scopesDump();
    //? vpi信号前缀
    const mon::vpi_prefixs prefixs ={"TOP.SOC.IF.", "TOP.SOC.RegFile.", "TOP.SOC.WB.", "TOP.SOC.ALU.", "TOP.SOC.ID."};
    //? vpi信号名字
    const mon::vpi_namess namess ={ {"pc","pause","jump","instr"},  //? IF

                                    {"zero","ra","sp","gp",         //? RegFile
                                    "tp","t0","t1","t2","t3","t4","t5","t6",
                                    "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
                                    "a0","a1","a2","a3","a4","a5","a6","a7"}, 

                                    {"ecall"},                      //? WB
                                    {"for_ena", "result", "rd"},           //? ALU
                                    {"instr_ID", "opcode_ID", "funct3_ID","funct7_ID",
                                     "funct_e_ID", "rd_ID", "imm_ID", "rst1_ID", "rst2_ID","rst1_read_ID","rst2_read_ID"}                          //? ID
                                    };                     

    //? mpi监视重命名
    mon::mpi_name_paths mpi_name_paths;
    mpi_name_paths = mon::mpi_unrename_map_init(prefixs, namess);

    //? mpi fifo深度
    mon::value_index fifo_deepth = 20;
    mon::value_index c_index = fifo_deepth /2;
    int RST_CLK_CYCLE = MAX_RST_TIME/2 + fifo_deepth-3;

    mon::eqs eqs_oringe = eqs_oringe_init(c_index);
    //? monitor 初始化
    mon::monitor monitor(dut, rise_trigger, positive, fifo_deepth, mpi_name_paths, eqs_oringe);

    //? monitor print名单
        //? 名字
        std::vector<mon::mpi_name> read_name = {};
        //? 索引
        std::vector<mon::value_index> read_index = {0,1,2,3,4,5,6,7,8};
        //? 一组名单 {名字， 索引}
        mon::mpi_name_indexs name_index = { {"pc",   c_index-3}, {"ecall", c_index-1},
                                            //  {"gp",   c_index },   {"sp",  c_index},
                                            // {"jump", c_index -2}, {"jump", c_index -1}, {"jump",c_index},
                                            {"ra",   c_index},{"a3",   c_index},{"a4",   c_index},{"sp", c_index},
                                            {"instr_ID", c_index -4},{"instr", c_index -4}
        };
                                            
    bool finish = 0;
    //? 读取csv
    std::string file = "/home/zero/master_share/pySpike/spike_log.csv";
    // const std::string file = "/home/zero/master_share/pySpike/spike_log.csv";
    std::cout << file << std::endl;
    std::cout << argv[1] << std::endl;
    file = argv[1];
    std::ifstream file_test(file);

    if (!file_test) {
        std::cerr << "File not found: " << file << std::endl;
        return 1;
    } else {
        file_test.close();
    }

    monitor.csv_reader_init(file);
    monitor.csv_eq_value_ptr_init();
    // csv_reader.printf(hex_disabled);
    // csv_reader.printf();
    // mon::mpi_value i = 10;
    // const mon::mpi_value* ptr = &i;

    //? monitor 待评估等式
    mon::eq jump1("jump", c_index-2, 1, equal, mon::eval_type::jump);
    mon::eq jump2("jump", c_index-3, 1, equal, mon::eval_type::jump);
    mon::eq jump3("jump", c_index-4, 1, equal, mon::eval_type::jump);
    mon::eq ecall("ecall", c_index-1, 1, equal, mon::eval_type::ecall);

    //? 等式压入
    monitor.eqs_push(jump1);
    monitor.eqs_push(jump2);
    monitor.eqs_push(jump3);
    monitor.eqs_push(ecall);

    //? 等式重新分类
    monitor.eqs_arrange();

    //? 进入循环
    while ((sim_time < MAX_SIM_TIME) && (monitor.finish == false)) {
        dut->clk_sys_i ^= 1;
        if(sim_time == MAX_RST_TIME) dut->rst_sys_i =0;
        dut->eval();

        //? 每个时钟边沿，先检测clk_trigger,再通过vpi读取信号存储在mpi中
        monitor.read();

        //? rst拉低并且check_clk通过
        if((monitor.instr_cnt>=c_index+4)&&(monitor.check_clk())){
            //? 打印monitor :
            //?         clk_cycle : xxx
                monitor.printf_monitor();
                monitor.printf_clk_cnt(0, hex_enabled);
                monitor.printf_instr_cnt(0, hex_enabled);
            //? 打印监视名单上信号
            monitor.printf(name_index, show, hex_enabled);

            //? 评估所有等式并处理
            monitor.eval(hex_enabled);

            // //? 打印名单上所有深度信号
            // monitor.printf(read_name, 10, hex_enabled);
        }  
        m_trace->dump(sim_time);
        sim_time++;
    }
    m_trace->close();
    delete dut;
    exit(monitor.m_exit);
}




mon::eqs eqs_oringe_init(mon::value_index current_index){
    mon::eqs eqs{
        mon::eq("pc",            current_index-3, nullptr, unequal, mon::eval_type::error),
        // mon::eq("current_iaddr", 3, nullptr, unequal, mon::eval_type::error),
        mon::eq("zero",          current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("ra",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("sp",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("gp",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("tp",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t0",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t1",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t2",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t3",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t4",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t5",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("t6",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a0",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a1",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a2",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a3",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a4",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a5",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a6",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("a7",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s0",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s1",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s2",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s3",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s4",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s5",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s6",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s7",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s8",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s9",            current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s10",           current_index, nullptr, unequal, mon::eval_type::error),
        mon::eq("s11",           current_index, nullptr, unequal, mon::eval_type::error),
    };
    return eqs;
}       
