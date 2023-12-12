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
#include <fstream>
#include <sstream>
#include <bitset>
#define double_trigger 0
#define rise_trigger  1
#define fall_trigger 2

#define negative 0
#define positive 1

#define hide    0
#define show 1

#define equal 0
#define unequal 1

#define hex_disabled 0
#define hex_enabled 1

namespace mon{

typedef std::string vpi_name;
typedef std::string vpi_prefix;
typedef std::string vpi_path;

typedef std::string mpi_name;
typedef long long   mpi_value;
typedef int     value_index;

typedef std::vector<vpi_name> vpi_names;
typedef std::vector<vpi_names> vpi_namess;
typedef std::vector<vpi_prefix> vpi_prefixs;
typedef std::vector<vpi_path> vpi_paths;



typedef std::vector<std::pair<mpi_name, value_index>>  mpi_name_indexs;
typedef std::vector<std::pair<mpi_name, vpi_path>> mpi_name_paths ;

typedef std::vector<mpi_value>       mpi_values;
// typedef bool unequal;
// typedef std::map<value_index, mpi_value> mpi_index_value;
// typedef std::vector<mpi_index_value> mpi_index_values;

enum eval_type {
    error,  
    jump, 
    pause,
    ecall   
};

typedef std::pair<const mpi_value*,const mpi_value*> eq_pair;

// todo eq  待评估等式信息
    class eq{
        private:
        public:
            mpi_name         name; 
            value_index      index; 
            mpi_value        value;
            const mpi_value*       eq_value_ptr;
            const mpi_value*       mpi_value_ptr;
            bool     inv;
            mon::eval_type   eval_type;
            eq();
        // todo 直接定义指针 
        // todo 温馨提示 指针记得初始化
            eq(const mpi_name& name, const value_index& index, const mpi_value* eq_value_ptr, 
                const bool& inv, const mon::eval_type& eval_type){
                    this->name = name;
                    this->index = index;
                    this->eq_value_ptr = eq_value_ptr;
                    this->inv = inv;
                    this->eval_type = eval_type;
                    this->mpi_value_ptr = nullptr;
                };
        // todo 常量转指针
            eq(const mpi_name& name, const value_index& index, const mpi_value value, 
            const bool& inv, const mon::eval_type& eval_type){
                this->name = name;
                this->index = index;
                this->value = value;
                this->inv = inv;
                this->eval_type = eval_type;
                this->eq_value_ptr = &this->value;
                this->mpi_value_ptr = nullptr;
            };
    };

typedef std::vector<eq> eqs;

//todo mpi 结构体 ，为 monitor 基本成员
    struct mpi {
            const mpi_name    name;          //* mpi名字
            const vpi_path    path;          //* vpi路径
            mpi_values        value;         //* mpi值

            mpi(const mpi_name& name, const vpi_path& path,  size_t depth) :
                        name(name),
                        path(path),
                        value(depth){}
    };

// todo 读取csv并解析
   class csv_reader{
        private:
            std::ifstream csv_file;
            std::string line;
            std::string word;
        public:
            std::vector<std::pair<std::string, long long>> header_value;
            csv_reader(){};
            csv_reader(std::string file_path){
                this->csv_file = std::move(std::ifstream(file_path, std::ios::in));
                if (!csv_file.is_open())
                {
                    std::cout << "Error: opening file fail" << std::endl;
                    std::exit(1);
                }
            };
            void get_header_all();
            // void get_header_except(std::vector<std::string>);
            void getvalue();
            void printf(const bool& hex);
    };



    class monitor{
        private:
            VSOC* dut;
            int clk_trigger;         // todo 设置时钟触发 0:双边沿  1:上升沿  2:下降沿 
            int rst_trigger;
            value_index fifo_deepth; // todo 读取的fifo深度
            value_index current_index; // todo 当前（WB模块）执行的指令
            csv_reader csv_r;   
            // todo 初始化时需传入的eqs，必须包含需要读取的csv中待评估的 mpi_name 和 value_index   
            eqs eqs_origin;     
            // todo 初始化后分类的eqs     
            eqs eqs_jump;
            eqs eqs_pause;
            eqs eqs_error;
            eqs eqs_ecall;
        public:
            std::vector<mpi> mpis;
            unsigned long long clk_cnt;     // todo 时钟周期计数
            unsigned long long instr_cnt;     // todo 指令周期计数
            bool finish;
            bool m_exit;
        monitor(VSOC* dut,const int& clk_trigger, const int& rst_trigger ,const value_index& fifo_deepth ,
                mpi_name_paths&  mpi_name_paths, eqs& eqs_origin){
            if(clk_trigger >2 ){
                std::cout << "Error: 不存在的 clk_trigger" << std::endl;
            }
            this->dut = dut;
            this->clk_trigger = clk_trigger;
            this->rst_trigger = rst_trigger;
            this->fifo_deepth = fifo_deepth;
            this->current_index = fifo_deepth/2;
            for(const auto & name_path : mpi_name_paths){
                this->mpis.push_back(mpi(name_path.first, name_path.second, fifo_deepth));
            }
            this->clk_cnt = 0;
            this->instr_cnt = 0;
            this->eqs_origin = eqs_origin;
            this->finish = false;
            this->m_exit = false;
        };


    // todo 检查时钟
        bool check_clk();
    // todo 检查rst 
        bool check_rst();
    // todo 打印 monitor :
        void printf_monitor();

    // todo 读取 dut信息
        void read(); //

    // todo 打印时钟周期
        void printf_clk_cnt(int init_sub, bool hex); 
    // todo 打印指令周期
        void printf_instr_cnt(int init_sub, bool hex);
    
    // todo 从0打印到指定深度
        void printf(const vpi_names& names, const value_index& fifo_deep, const bool& hex);

    // todo 直接打印指定深度
        void printf(const vpi_names& names, const std::vector<value_index>& fifo_index, const bool& hex);

    // todo 用一个vector指定打印 可以选择是否输出 [i]
        void printf(const mpi_name_indexs& name_indexs, const bool& show_hide, const bool& hex);

    // todo 打印子函数实现
        void printf_index(const mpi& mpi, const value_index& index, const bool& show_hide, const bool& hex); 

    // todo csv_reader 初始化
        void csv_reader_init(const std::string& file_path);
    // todo 从eqs_oringe中绑定csv_reader指针 
        void csv_eq_value_ptr_init();
        void csv_getvalue();
        void csv_printf(const bool& hex);
    // todo 压入一个 eq 到 eqs   mpi.name ==name mpi.value[index] = value
        void eqs_push(const mpi_name& name,   const value_index& index, 
                      const mpi_value* value_ptr, const bool& inv, mon::eval_type eval_type);
        void eqs_push(const eq& eq);
        void eqs_push(const mon::eqs& eqs);
        
        
    //todo 根据 Error_type 分类eqs
        void eqs_arrange();
    // todo 判断所有 eqs  并处理
        void eval(const bool& hex);

        bool eval_eq(eq& eq,const bool& hex);

        void instr_translator(const value_index& index);
    };
    


//todo vpi str读取子函数
    std::string vpi_read_str(const vpi_path& vpi_path, const int& value_formats_str);

//todo vpi int读取子函数
    int vpi_read_int(const vpi_path& vpi_path);

//todo vpi longlong读取子函数
    long long vpi_read_longint(const vpi_path& vpi_path);

//todo 给 vpi_names 加上前缀 返回 vpi_paths
    vpi_paths vpi_paths_generate(const vpi_prefix& prefix, const vpi_names& names);

//todo 不需要重命名，mpi_name与 vpi_name相同
    mpi_name_paths mpi_unrename_paths_generate(const vpi_prefix& prefix, const vpi_names& vpi_names); 

// todo 递归生成无需重命名 mpi_name_paths
    mpi_name_paths mpi_unrename_map_init(const vpi_prefixs& prefixs, const vpi_namess& namess);
    
//todo mpi_name_paths合成
    void mpi_name_paths_insert(mpi_name_paths& main, const mpi_name_paths& branchs);

void prinrf_eq(const eq& eq,const bool& hex);
void debug();
void printBinary(int num);

std::string rst_translator(const mpi_value& value);
std::string opcode_translator(const mpi_value& value);
}
 


#endif