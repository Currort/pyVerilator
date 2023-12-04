#ifndef MONITOR_H_
#define MONITOR_H_
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <map>
#include <iomanip>
#include "VSOC.h"
#include <verilated_vpi.h>
#include <cassert>
#include <algorithm>
#include <vector>

#define double_trigger 0
#define rise_trigger 1
#define fall_trigger 2


#define hide    0
#define display 1

namespace MON{


class monitor{
    private:
        VSOC* dut;
        uint8_t clk_trigger; //? 设置时钟触发 0:双边沿  1:上升沿  2:下降沿 
        uint8_t fifo_deepth; //? 读取的fifo深度
        std::vector<std::string> key_IF;     //? IF的vpi信号
        std::vector<std::string> key_RegFile;//? RegFile的vpi信号
    public:
        unsigned long long clk_cnt; //? 时钟周期计数
        std::map<std::string, std::vector<std::string>> dict; //? 存储的信号名字和值

    monitor(VSOC* dut, uint8_t clk_trigger = 0, uint8_t fifo_deepth = 0,
            std::vector<std::string>  key_IF = {} , std::vector<std::string> key_RegFile = {} ){
        assert(clk_trigger <= 2);
        this->dut = dut;
        this->clk_trigger = clk_trigger;
        this->fifo_deepth = fifo_deepth;
        for(const auto & key : key_IF){
            this->key_IF.push_back(key);
            std::string str = "TOP.SOC.IF." + key;
            this->dict[key].assign(fifo_deepth,"0");
        }
        for(const auto & key : key_RegFile){
            this->key_RegFile.push_back(key);
            std::string str = "TOP.SOC.RegFile." + key;
            this->dict[key].assign(fifo_deepth,"0");
        }
        this->clk_cnt = 0;
    };
    uint8_t check_clk();//? 检查时钟
    void read(); //
    void print_monitor();
    void print_clk(); //? 打印时钟周期
    void printf(std::vector<std::string> name, uint8_t fifo_deep);//? 从0打印到指定深度
    void printf(std::vector<std::string> name, std::vector<uint8_t> fifo_index);//? 直接打印指定深度
    void printf(std::map<std::string, uint8_t> name_index, uint8_t display_index);//? 用一个map指定打印 可以选择是否输出 [i]


    void printf_index(std::string name, std::vector<std::string> value, uint8_t index, uint8_t display_index); //? 打印子函数实现
    void vpi_read_str_fifo(const std::string name_prefix, const std::string verilator_vpi_name, int value_formats);//? vpi fifo读取子函数
};

std::string vpi_read_str(const std::string verilator_vpi_name, int value_formats);//? vpi 读取子函数

}
#endif