#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
using  std::string;
using  std::vector;

typedef std::string vpi_name;
typedef std::string vpi_rename;
typedef std::string vpi_prefix;
typedef std::map<vpi_name, vpi_rename> vpi_rename_map ;
typedef std::map<vpi_prefix, vpi_rename_map> monitor_vpi_map;

struct mpi {
    const std::string path_prefix;
    const std::string path_signal;
    const std::string rename;
    std::vector<long long> value;
    mpi(const std::string& path_prefix,
                const std::string& path_signal,
                const std::string& rename,
                size_t depth) :
                path_prefix(path_prefix),
                path_signal(path_signal),
                rename(rename),
                value(depth){}
};
std::ostream& operator<<(std::ostream& os, const mpi& monitor) {
    os << "path_prefix: " << monitor.path_prefix << std::endl;
    os << "path_signal: " << monitor.path_signal << std::endl;
    os << "rename: " << monitor.rename << std::endl;
    os << "value: [";
    for (const auto& v : monitor.value) {
        os << v << " ";
    }
    os << "]" << std::endl;

    return os;
}
    vpi_rename_map if_name = {{"pc","pc"}, {"addr","asd"}, {"asda","asdad"} };
    vpi_rename_map wb_name = {{"pc0","pc"}, {"addr4","asd"}, {"asda1","asdad"} };
    vpi_rename_map reg_name = {{"pc1","pc"}, {"add3","asd"}, {"asda4","asdad"} };
    monitor_vpi_map pre  = {{"top.pc.",if_name}, {"top.wb.",wb_name}, {"top.reg.",reg_name}};

    std::vector<mpi> m_vpi;


int main(){

    int a = 10;
    int b = 20;

    std::pair<int*, int*> myPair(&a, &b);

    std::cout << "First: " << *(myPair.first) << std::endl;
    std::cout << "Second: " << *(myPair.second) << std::endl;

    return 0;
    // for(const auto & preifx_map : pre){
    //         for(const auto & name_map : preifx_map.second){
    //             m_vpi.push_back(mpi(preifx_map.first, name_map.first, name_map.second, 7));
    //         }
    //     }
    

    // for (auto & m_vpii : m_vpi)
    //     std::cout << m_vpii << std::endl;
    return 0;
};  