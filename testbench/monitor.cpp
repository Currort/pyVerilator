#include "monitor.h"

mon::mpi_name_paths mon::mpi_unrename_map_init(const vpi_prefixs& prefixs, const vpi_namess& namess){
    assert(!prefixs.empty());
    assert(!namess.empty());
    size_t psize = prefixs.size();
    size_t nsize = namess.size();
    assert(psize == nsize);
    mpi_name_paths main = mpi_unrename_paths_generate(prefixs[0], namess[0]); 
    for(int i = 1; i < nsize; i++){
        mpi_name_paths_insert(main, mpi_unrename_paths_generate(prefixs[i], namess[i]));
    }
    return main;
};

bool mon::monitor::check_clk(){
    bool trigger = 0;
    switch (this->clk_trigger){
        case 0:    trigger = 1; break;
        case 1:    if(this->dut->clk_sys_i == 1)  trigger = 1;  break;
        case 2:    if(this->dut->clk_sys_i == 0)  trigger = 1;  break;
    }
    return trigger;
};

bool mon::monitor::check_rst(){
    bool trigger = 0;
    switch (this->rst_trigger){
        case 0:    if(this->dut->rst_sys_i == 0)  trigger = 1;  break;
        case 1:    if(this->dut->rst_sys_i == 1)  trigger = 1;  break;
    }
    return trigger;
};

void mon::monitor::printf_monitor(){
    std::cout<< "------------------------MONITOR------------------------\n" << std::endl;
};
    

void mon::monitor::read(){
    if(!check_clk()) return; 
    if(check_rst()) return; 
    this->clk_cnt++;
    this->instr_cnt++;
    for(auto & mpi : this->mpis){
        for(int i = 0; i < this->fifo_deepth-1; i++){
            mpi.value[i] = mpi.value[i+1];
        }
        mpi.value[fifo_deepth-1] = vpi_read_longint(mpi.path);
    }

};

void mon::monitor::printf_clk_cnt(int init_sub, bool hex){
    if(hex)std::cout << std::hex;
    std::cout <<"    clk_cnt : " << this->clk_cnt-this->current_index-3-init_sub << std::endl;
    std::cout << std::dec;
}

void mon::monitor::printf_instr_cnt(int init_sub, bool hex){
    if(hex)std::cout << std::hex;
    std::cout <<"    instr_cnt : " << this->instr_cnt-this->current_index-3-init_sub << std::endl;
    std::cout << std::dec;
};

void mon::monitor::printf(const vpi_names& names, const value_index& fifo_deep, const bool& hex){
    assert(fifo_deep <= this->fifo_deepth);
    if(check_clk()); else return; 
    for(auto & mpi : this->mpis){
        if (names.empty()){
            if(fifo_deep == 0) {
                this->printf_index(mpi, 0, 0, hex);
            } else for(int i =0; i<fifo_deep; i++){
                this->printf_index(mpi, i, 1, hex);
            } 
        }
        else {
            for(const auto & name : names){
                if(mpi.name == name){
                    if(fifo_deep == 0) {
                        this->printf_index(mpi, 0, 0, hex);
                    }else for(int i =0; i<fifo_deep; i++){
                        this->printf_index(mpi, i, 1, hex);
                    }  
                }
            }   
        }
    }
}

void mon::monitor::printf(const vpi_names& names, const std::vector<value_index>& fifo_index, const bool& hex){ 
    for(auto & mpi : this->mpis){
        if (names.empty()){
            for(auto & index : fifo_index){
                this->printf_index(mpi, index, 0, hex);
            }
        }
        else {
            for(const auto & name : names){
                if(mpi.name == name){
                    for(auto & index : fifo_index){
                        this->printf_index(mpi, index, 1, hex);
                    } 
                }
            }   
        }
    }
};

void mon::monitor::printf(const mpi_name_indexs& name_indexs, const bool& show_hide, const bool& hex){
    for(auto & mpi : this->mpis){
        for(const auto & name_index : name_indexs){
            if(mpi.name == name_index.first){
                this->printf_index(mpi, name_index.second, show_hide, hex);
            }
        }   
    }
};



void mon::monitor::printf_index(const mpi& mpi, const value_index& index, const bool& show_hide, const bool& hex){
    assert(index <= this->fifo_deepth);
    std::cout <<"    "<<
    std::left << std::setw(9)  << mpi.name;
    if(show_hide)std::cout << "[" << (int)index << "]" ;
    std::cout << " : ";
    if(hex)      std::cout << std::hex << std::uppercase ;
    std::cout << std::left << std::setw(20) << mpi.value[index] << std::endl;
    std::cout << std::dec;
}



std::string mon::vpi_read_str(const vpi_path& vpi_path, const int& value_formats_str){
    vpiHandle vpiHandle;
    s_vpi_value s_vpi_value;
    vpiHandle = vpi_handle_by_name((PLI_BYTE8*)vpi_path.c_str(), NULL);
    if (!vpiHandle) vl_fatal(__FILE__, __LINE__, "sim_main", "No handle found");
    s_vpi_value.format  = value_formats_str;
    vpi_get_value(vpiHandle, &s_vpi_value);   
    return s_vpi_value.value.str;
};

int mon::vpi_read_int(const vpi_path& vpi_path){
    vpiHandle vpiHandle;
    s_vpi_value s_vpi_value;
    vpiHandle = vpi_handle_by_name((PLI_BYTE8*)vpi_path.c_str(), NULL);
    if (!vpiHandle) vl_fatal(__FILE__, __LINE__, "sim_main", "No handle found");
    s_vpi_value.format  = vpiIntVal;
    vpi_get_value(vpiHandle, &s_vpi_value);   
    return s_vpi_value.value.integer;
};

long long mon::vpi_read_longint(const vpi_path& vpi_path){
    vpiHandle vpiHandle;
    s_vpi_value s_vpi_value;
    vpiHandle = vpi_handle_by_name((PLI_BYTE8*)vpi_path.c_str(), NULL);
    if (!vpiHandle) vl_fatal(__FILE__, __LINE__, "sim_main", "No handle found");
    s_vpi_value.format  = vpiLongIntVal;
    vpi_get_value(vpiHandle, &s_vpi_value);   
    return s_vpi_value.value.longint;
};

mon::vpi_paths mon::vpi_paths_generate(const vpi_prefix& prefix, const vpi_names& names){
    vpi_paths vpi_paths;
    for(const auto& name : names){
        vpi_paths.push_back(prefix+name);
    }
    return vpi_paths;
};

mon::mpi_name_paths mon::mpi_unrename_paths_generate(const vpi_prefix& prefix, const vpi_names& names){
    mpi_name_paths name_paths;
    for(const auto & name :names){
        name_paths.push_back(make_pair(name, prefix + name));
    }
    return name_paths;
};

void mon::mpi_name_paths_insert(mpi_name_paths& main, const mpi_name_paths& branchs){
    for(auto & branch : branchs)
    main.push_back(std::move(branch));
};

void mon::csv_reader::get_header_all(){
    std::getline(this->csv_file, this->line);
    std::istringstream iss(line);
    while (std::getline(iss, this->word, ',')){
        this->header_value.push_back({word.c_str(), 0});
    }
    this->header_value.pop_back();
    this->header_value.push_back({word.substr(0,word.length() - 1), 0});
};

void mon::csv_reader::getvalue(){
    std::getline(this->csv_file, this->line);
    std::istringstream iss(line);
    unsigned int i = 0;
    while (std::getline(iss, this->word, ',')){
        this->header_value[i++].second = (mpi_value)std::stoull(word, nullptr, 16);
        // std::cout << this->header_value[i++].second << std::endl;    
    }
};
void mon::csv_reader::printf(const bool& hex){
    size_t size = this->header_value.size();
    if(hex)std::cout << std::hex << std::uppercase ;
    for (size_t i = 0; i < size; i++)
    {
        std::cout << std::left << std::setw(15)  << this->header_value[i].first << ":" ;
        std::cout << std::right << std::setw(20) << this->header_value[i].second << std::endl;
    }
    std::cout << std::dec;
};

void mon::monitor::csv_reader_init(const std::string& file_path){
    this->csv_r = csv_reader(file_path);
    this->csv_r.get_header_all();
    this->csv_getvalue();
};

void mon::monitor::eqs_push(const mon::mpi_name& name,   const mon::value_index& index, 
                            const mon::mpi_value* value_ptr, const bool& inv, 
                            mon::eval_type eval_type){
    this->eqs_origin.push_back(mon::eq(name, index, value_ptr, inv, eval_type));
};
void mon::monitor::eqs_push(const mon::eq& eq){
    this->eqs_origin.push_back(eq);
};
void mon::monitor::eqs_push(const mon::eqs& eqs){
    for( auto & eq : eqs)
        this->eqs_origin.push_back(std::move(eq));
};

void mon::monitor::csv_eq_value_ptr_init(){
    for( auto & eq : this->eqs_origin){
        for(const auto & hv : this->csv_r.header_value){
            if(hv.first == eq.name){
                eq.eq_value_ptr = &hv.second; 
                break;
            }
        }
    }
};

void mon::monitor::csv_getvalue(){
    this->csv_r.getvalue();
}

void mon::monitor::csv_printf(const bool& hex){
    this->csv_r.printf(hex);
};

void mon::monitor::eqs_arrange(){
    for( auto & eq_origin : this->eqs_origin){
        switch (eq_origin.eval_type)
        {
            case jump:  this->eqs_jump.push_back(std::move(eq_origin)); break;
            case pause: this->eqs_pause.push_back(std::move(eq_origin));break;
            case error: this->eqs_error.push_back(std::move(eq_origin));break; 
            case ecall: this->eqs_ecall.push_back(std::move(eq_origin));break;   
        }
    }
};

void mon::monitor::eval(const bool& hex){
    for( auto & eq :this->eqs_jump){
        if(this->eval_eq(eq, hex)){
            std::cout << "    jump!\n" << std::endl;
            this->instr_cnt--;
            return ;
        }    
    }
    for( auto & eq :this->eqs_pause){
        if(this->eval_eq(eq, hex)){
            std::cout << "    pause!\n" << std::endl;
            this->instr_cnt--;
            return ;
        }    
    }
    for( auto & eq :this->eqs_ecall){
        if(this->eval_eq(eq, hex)){
            std::cout << "\n---------------恭喜，你成功进入了ECALL！----------------\n" << std::endl;
            this->instr_cnt--;
            std::cout << "    时钟周期 ： " << this->clk_cnt << std::endl;
            std::cout << "    指令周期 ： " << this->instr_cnt << std::endl;
            
            exit(EXIT_SUCCESS);
            return ;
        }    
    }
    bool error = false;
    for( auto & eq :this->eqs_error){
        if(this->eval_eq(eq, hex)){
            if (!error) std::cout<< "\n------------------------ERROR------------------------n" << std::endl;
            error = true;
            this->printf_clk_cnt(0, hex_enabled);
            this->printf_instr_cnt(0, hex_enabled);
            std::cout <<std::dec;
            prinrf_eq(eq, hex);
            this->finish = true;
            this->m_exit = EXIT_FAILURE;
        }    
    }
    if(error){

        // instr_translator(this->current_index-7);
        // instr_translator(this->current_index-6);
        // instr_translator(this->current_index-5);
        instr_translator(this->current_index-4);

    } 
    this->csv_getvalue();
};

bool mon::monitor::eval_eq(mon::eq& eq,const bool& hex){
    if(eq.eq_value_ptr==nullptr){
        std::cout<< "\n------------------------ERROR------------------------n" << std::endl;
        std::cout << "Error! eq.eq_value_ptr is nullptr!" <<std::endl;
        prinrf_eq(eq, hex);
        exit(1);
        return false;
    }
    if(eq.mpi_value_ptr == nullptr){
        bool found = false;    
        for( auto &mpi : this->mpis){
            if(mpi.name == eq.name){
                found = true;
                eq.mpi_value_ptr = &mpi.value[eq.index];
                if(!eq.inv)     return (*eq.mpi_value_ptr == *eq.eq_value_ptr);
                else if(eq.inv) return (*eq.mpi_value_ptr != *eq.eq_value_ptr);
                break;
            } 
        }
        if(!found){
            std::cout<< "\n------------------------ERROR------------------------n" << std::endl;
            std::cout << "ERROR ! there is not eq.name : "<< eq.name <<" in mpis" << std::endl;
            exit(1);
        }
    }
    else {
        if(!eq.inv)     return (*eq.mpi_value_ptr == *eq.eq_value_ptr);
        else if(eq.inv) return (*eq.mpi_value_ptr != *eq.eq_value_ptr);
    };
    return false;
};



void mon::prinrf_eq(const mon::eq& eq,const bool& hex){
    std::cout << "    eq.name : " << eq.name  <<std::endl;
    std::cout << "    eq.index : " << eq.index <<std::endl;
    std::cout << "    eq.eval_type : ";
    switch (eq.eval_type)
    {
    case mon::eval_type::error:
        std::cout << "error" << std::endl;
        break;
    case mon::eval_type::jump:
        std::cout << "jump" << std::endl;
        break;
    case mon::eval_type::pause:
        std::cout << "pause" << std::endl;
        break;
    case mon::eval_type::ecall:
        std::cout << "ecall" << std::endl;
        break;
    default:
        break;
    }
    if(hex)      std::cout << std::hex << std::uppercase ;
    std::cout << "    *eq.mpi_value_ptr : "<< *eq.mpi_value_ptr << std::endl;
    std::cout << "    *eq.eq_value_ptr : "<< *eq.eq_value_ptr << std::endl;
    std::cout << std::dec << std::endl;
}

void mon::debug(){
    std::cout << "成功访问 文件：" <<__FILE__ <<std::endl;
    std::cout << "         行 : " << __LINE__<<std::endl;
}


void mon::monitor::instr_translator(const mon::value_index& index){
    std::cout << std::hex ;
    std::bitset<32> binary;
    std::cout<< "\n------------------------INSTR_TRANSLATOR------------------------\n" << std::endl;
    std::cout<< "---------------------- PREVIOUS INSTR : " << this->current_index-index -4 ;
    std::cout<< " ----------------------" << std::endl;
    // std::cout<< "\n------------------------------BIN-------------------------------\n" << std::endl;
    // for(std::string name : {"instr_ID", "rd_ID", "funct3_ID", "rst1_ID", "opcode_ID",
    //                        "rst2_ID", "funct7_ID", "funct_e_ID", "imm_ID","rst1_read_ID","rst2_read_ID"}){
    //     for(const auto & mpi : this->mpis){
    //         if (mpi.name == name){
    //             std::cout << "    " << std::left << std::setw(18) << mpi.name << " : ";
    //             std::cout << std::right << std::setw(32);
    //             printBinary(mpi.value[index]);
    //             std::cout << std::endl;
    //         }
    //     }
    // }
    // std::cout<< "\n--------------------------TRANSLATOR---------------------------\n" << std::endl;
    for(const auto & mpi : this->mpis){
        if(mpi.name == "instr"){
            std::cout << "    " << std::left << std::setw(18) << mpi.name << " : ";
            std::cout << std::right << std::setw(32) << mpi.value[index] << std::endl;
            std::cout << "    " << std::left << std::setw(18) << mpi.name+"_bin" << " : ";
            std::cout << std::right << std::setw(32);
            printBinary(mpi.value[index]);
            std::cout << std::endl;

        } else if (mpi.name == "pc"){
            std::cout << "    " << std::left << std::setw(18) << "instr_cnt" << " : ";
            std::cout << std::right << std::setw(32) << this->instr_cnt-this->current_index-3 - this->current_index+index +4 << std::endl;
            std::cout << "    " << std::left << std::setw(18) << mpi.name << " : ";
            std::cout << std::right << std::setw(32) << mpi.value[index+1] << std::endl;
            std::cout << "    " << std::left << std::setw(18) << "current_pc" << " : ";
            std::cout << std::right << std::setw(32) << mpi.value[index] << std::endl;
            std::cout << std::endl;

        }else if(mpi.name == "instr_ID"){
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) + "_unfold" << " : ";
            std::cout << std::right << std::setw(32) << mpi.value[index] << std::endl;
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) + "_unfold"+"_bin" << " : ";
            std::cout << std::right << std::setw(32);
            printBinary(mpi.value[index]);
            std::cout << std::endl;
            std::cout << std::endl;

        } else if( mpi.name == "opcode_ID"){
            std::cout << "    " << std::left << std::setw(11) << mpi.name.substr(0, mpi.name.length() - 3) 
            << std::right << std::setw(7);
            printBinary(mpi.value[index]);
            std::cout << " : " << std::right << std::setw(32) << opcode_translator(mpi.value[index]) << std::endl;

        } else if(( mpi.name == "funct3_ID") ||
                  ( mpi.name == "funct7_ID") ||
                  ( mpi.name == "funct_e_ID")){
                std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) << " : ";
                std::cout << std::right << std::setw(32);
                printBinary(mpi.value[index]);
                std::cout << std::endl;    
        } else if(mpi.name == "rd_ID"){
            std::cout << "    " << std::left << std::setw(14) << mpi.name.substr(0, mpi.name.length() - 3) 
            << std::right << std::setw(4) << rst_translator(mpi.value[index]) << " : "
            << std::right << std::setw(32) << mpi.value[index] << std::endl;
        }
    }
    std::cout<< "---------------------------BIN---------------------------" << std::endl;
    for(const auto & mpi : this->mpis){
        std::cout << std::hex;
        if( mpi.name == "imm_ID"){
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) << " : "
            << std::right << std::setw(32) ;
            printBinary(mpi.value[index+1]);
            std::cout << std::endl;
        } else if(  ((mpi.name == "rst1_ID")) ||
                    ((mpi.name == "rst2_ID"))){
            int value_temp ;
            for(const auto & mpii : this->mpis){
                if(mpi.name.substr(0, mpi.name.length() - 3)== mpii.name.substr(0, mpii.name.length() - 8))
                   value_temp =  mpii.value[index] ;
            }
            std::cout << "    " << std::left << std::setw(14) << mpi.name.substr(0, mpi.name.length() - 3) 
            << std::right << std::setw(4) << rst_translator(mpi.value[index]) << " : "
            << std::right << std::setw(32) ;
            printBinary(value_temp);
            std::cout << std::endl;
        }
    }
    std::cout<< "---------------------------HEX---------------------------" << std::endl;
    for(const auto & mpi : this->mpis){
        std::cout << std::hex;
        if( mpi.name == "imm_ID"){
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) << " : "
            << std::right << std::setw(32) << mpi.value[index+1] << std::endl;
        } else if(  ((mpi.name == "rst1_ID")) ||
                    ((mpi.name == "rst2_ID"))){
            int value_temp ;
            for(const auto & mpii : this->mpis){
                if(mpi.name.substr(0, mpi.name.length() - 3)== mpii.name.substr(0, mpii.name.length() - 8))
                   value_temp =  mpii.value[index] ;
            }
            std::cout << "    " << std::left << std::setw(14) << mpi.name.substr(0, mpi.name.length() - 3) 
            << std::right << std::setw(4) << rst_translator(mpi.value[index]) << " : "
            << std::right << std::setw(32) << value_temp << std::endl;
        }
    }
    std::cout<< "--------------------------SIGNED-------------------------" << std::endl;
    for(const auto & mpi : this->mpis){
        std::cout << std::dec;
        if( mpi.name == "imm_ID"){
            std::cout << std::dec;
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) << " : ";
            std::cout << std::right << std::setw(32) << mpi.value[index+1] << std::endl;
            
        } else if(  ((mpi.name == "rst1_ID")) ||
                    ((mpi.name == "rst2_ID"))){
            int value_temp ;
            for(const auto & mpii : this->mpis){
                if(mpi.name.substr(0, mpi.name.length() - 3)== mpii.name.substr(0, mpii.name.length() - 8))
                   value_temp =  mpii.value[index] ;
            }
            std::cout << "    " << std::left << std::setw(14) << mpi.name.substr(0, mpi.name.length() - 3) 
            << std::right << std::setw(4) << rst_translator(mpi.value[index]) << " : "
            << std::right << std::setw(32) << value_temp << std::endl;
        }
    }
    std::cout<< "-------------------------UNSIGNED------------------------" << std::endl;
    for(const auto & mpi : this->mpis){
        std::cout << std::dec;
        if( mpi.name == "imm_ID"){
            std::cout << "    " << std::left << std::setw(18) << mpi.name.substr(0, mpi.name.length() - 3) << " : ";
            std::cout << std::right << std::setw(32) << (unsigned long long)mpi.value[index+1] << std::endl;
        } else if(  ((mpi.name == "rst1_ID")) ||
                    ((mpi.name == "rst2_ID"))){
            int value_temp ;
            for(const auto & mpii : this->mpis){
                if(mpi.name.substr(0, mpi.name.length() - 3)== mpii.name.substr(0, mpii.name.length() - 8))
                   value_temp =  mpii.value[index] ;
            }
            std::cout << "    " << std::left << std::setw(14) << mpi.name.substr(0, mpi.name.length() - 3)
            << std::right << std::setw(4) << rst_translator(mpi.value[index]) << " : "
            << std::right << std::setw(32) << (unsigned long long)value_temp << std::endl;
        }
    }
            
    std::cout << std::dec ;
}



void mon::printBinary(int num) {
    const int numBits = sizeof(num) * 8; // 获取整数的位数
    std::string bits = "";
    bool leadingZeros = true; // 标志位，用于跳过前导零

    for (int i = numBits - 1; i >= 0; i--) {
        int bit = (num >> i) & 1; // 获取第 i 位的值
        if (bit == 1) leadingZeros = false; // 遇到第一个非零位，设置标志位为 false
        if (!leadingZeros) {
            bits += std::to_string(bit);
        }
    }
    if (bits.empty()) {
        bits = "0"; // 处理特殊情况：整数为 0
    }
    std::cout << bits;
}


std::string mon::rst_translator(const mon::mpi_value& value){
    switch (value)
    {
    case  0: return "zero";
    case  1: return "ra" ;
    case  2: return "sp" ;
    case  3: return "gp" ;
    case  4: return "tp" ;
    case  5: return "t0" ;
    case  6: return "t1" ;
    case  7: return "t2" ;
    case  8: return "s0" ;
    case  9: return "s1" ;
    case 10: return "a0" ;
    case 11: return "a1" ;
    case 12: return "a2" ;
    case 13: return "a3" ;
    case 14: return "a4" ;
    case 15: return "a5" ;
    case 16: return "a6" ;
    case 17: return "a7" ;
    case 18: return "s2" ;
    case 19: return "s3" ;
    case 20: return "s4" ;
    case 21: return "s5" ;
    case 22: return "s6" ;
    case 23: return "s7" ;
    case 24: return "s8" ;
    case 25: return "s9" ;
    case 26: return "s10" ;
    case 27: return "s11" ;
    case 28: return "t3" ;
    case 29: return "t4" ;
    case 30: return "t5" ;
    case 31: return "t6" ;
    default: return "";
    }
};

std::string mon::opcode_translator(const mon::mpi_value& value){
    switch (value)
    {
        case (0b0110011): return "ARITHMETIC_R" ;       
        case (0b0101111): return "ATOMICAL_R"   ;      
        case (0b0010011): return "ARITHMETIC_I" ;       
        case (0b0000011): return "ACCESS_I"     ;   
        case (0b1110011): return "EXCEPTION"    ;   
        case (0b0001111): return "FENCE"        ;
        case (0b0100011): return "ACCESS_S"     ;   
        case (0b1100011): return "BRANCH_B"     ;   
        case (0b1101111): return "JAL"          ;
        case (0b1100111): return "JALR"         ;
        case (0b0110111): return "LUI"          ;
        case (0b0010111): return "AUIPC"        ;
        case (0b0111011): return "R_64_ONLY"    ;   
        case (0b0011011): return "I_64_ONLY"    ;
    default: return "UNKNOWN";break;
    }
}
