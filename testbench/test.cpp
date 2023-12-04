#include <stdlib.h>
#include <iostream>
#include <map>
#include "monitor.h"
#include "monitor.cpp"
// class VPI{
//     public:
//     int   handle;
//     int   value;
//     // VPI(int handle ,int value){
//     //     this->handle = handle;
//     //     this->value = value;
//     // };
// };

// class monitor{
//     public:
//         std::map<std::string, VPI> VPI_dict;
//         const std::string key[36] = {"pc","pause_i","jump_i","instr_o",
//                 "zero","ra","sp","gp",
//                 "tp","t0","t1","t2","t3","t4","t5","t6",
//                 "s0","s1","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11",
//                 "a0","a1","a2","a3","a4","a5","a6","a7"};
                
//     monitor(){
        
//         // this->VPI_dict["pc"].handle = 4;
//         // this->VPI_dict["pc"].value = 2;
//     };
//     void init(){
//         for (const std::string& str : this->key) {
//             this->VPI_dict[str].handle = 2; 
//         }
//     };
// };

int main(){
    MON::monitor monitor;
    monitor.init();
    for (const std::string& str : monitor.key) {
        printf("%d\n",monitor.VPI_dict[str]);
    }
    return 0;
};  