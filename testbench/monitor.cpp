#include "monitor.h"
void MON::monitor::print_monitor(){
    std::cout << "monitor :" << std::endl;
};
    

void MON::monitor::read(){
    if(check_clk()); else return; 
    this->clk_cnt++;
    if(this->key_IF.empty());
    else {
        for(auto & str : this->key_IF){
            vpi_read_str_fifo("TOP.SOC.IF.", str, vpiHexStrVal);
        }
    }
    if(this->key_RegFile.empty());
    else {
        for(auto & str : this->key_RegFile){
            vpi_read_str_fifo("TOP.SOC.RegFile.", str, vpiHexStrVal);
        }
    }
};

void MON::monitor::print_clk(){
    std::cout <<"    clk cycle : " << this->clk_cnt << std::endl;
}

void MON::monitor::printf(std::vector<std::string> name = {}, uint8_t fifo_deep = 0){
    assert(fifo_deep <= this->fifo_deepth);
    if(check_clk()); else return; 
    for(auto & str : this->dict){
        if (name.empty()){
            if(fifo_deep == 0) {
                this->printf_index(str.first, str.second, 0, 0);
            } else for(int i =0; i<fifo_deep; i++){
                this->printf_index(str.first, str.second, i, 1);
            } 
        }
        else {
            for(const auto & name_ptr : name){
                if(str.first == name_ptr){
                    if(fifo_deep == 0) {
                        this->printf_index(str.first, str.second, 0, 0);
                    }else for(int i =0; i<fifo_deep; i++){
                        this->printf_index(str.first, str.second, i, 1);
                    }  
                }
            }   
        }
    }
}

void MON::monitor::printf(std::vector<std::string> name, std::vector<uint8_t> fifo_index){ 
    for(auto & str : this->dict){
        if (name.empty()){
            for(auto & index : fifo_index){
                this->printf_index(str.first, str.second, index, 0);
            }
        }
        else {
            for(const auto & name_ptr : name){
                if(str.first == name_ptr){
                    for(auto & index : fifo_index){
                        this->printf_index(str.first, str.second, index, 1);
                    } 
                }
            }   
        }
    }
};

void MON::monitor::printf(std::map<std::string, uint8_t> name_index, uint8_t display_index){
    for(auto & str : this->dict){
        for(const auto & ptr : name_index){
            if(str.first == ptr.first){
                this->printf_index(str.first, str.second, ptr.second, display_index);
            }
        }   
    }
};

uint8_t MON::monitor::check_clk(){
    uint8_t trigger = 0;
    switch (this->clk_trigger){
        case 0:    trigger = 1; break;
        case 1:    if(this->dut->clk_sys_i == 1)  trigger = 1;  break;
        case 2:    if(this->dut->clk_sys_i == 0)  trigger = 1;  break;
    }
    return trigger;
};

std::string MON::vpi_read_str(const std::string verilator_vpi_name, int value_formats){
    vpiHandle vpiHandle;
    s_vpi_value s_vpi_value;
    vpiHandle = vpi_handle_by_name((PLI_BYTE8*)verilator_vpi_name.c_str(), NULL);
    if (!vpiHandle) vl_fatal(__FILE__, __LINE__, "sim_main", "No handle found");
    s_vpi_value.format  = value_formats;
    vpi_get_value(vpiHandle, &s_vpi_value);
    return s_vpi_value.value.str;
};

void MON::monitor::vpi_read_str_fifo(const std::string name_prefix, const std::string verilator_vpi_name, int value_formats){
    std::string str_temp = name_prefix + verilator_vpi_name;
    for(int i = 0; i < this->fifo_deepth-1; i++){
        this->dict[verilator_vpi_name][i] = this->dict[verilator_vpi_name][i+1];
    }
    this->dict[verilator_vpi_name][fifo_deepth-1] = vpi_read_str(str_temp.c_str(), vpiHexStrVal);
};

void MON::monitor::printf_index(std::string name, std::vector<std::string> value, uint8_t index, uint8_t display_index){
    assert(index <= this->fifo_deepth);
    std::cout <<"    "<<
    std::left << std::setw(9)  << name;
    if(display_index) std::cout << "[" << (int)index << "]" ;
    std::cout << " : " << std::left << std::setw(20) << value[index] << std::endl;
}


