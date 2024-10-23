
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
    auto start = std::chrono::system_clock::now();



for(uint64_t i = 13517397601145585665ULL;((unpack_number(i)<unpack_number(13517398430818916865ULL) ? 10ULL : 2ULL)) != 2ULL;i = pack_number(unpack_number(i) + 1.0f)){
uint64_t x = (*(func_ptr*)(unpack_object(unpack_object((*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromFloat"))->hidden_data))({Double,13517343179614978049ULL}))->get("add"))->hidden_data))({(*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromFloat"))->hidden_data))({Double,13517355274242883585ULL}),(*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromFloat"))->hidden_data))({Double,13517355274242883585ULL})});
}

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

free_all_objects();
return 0;
}