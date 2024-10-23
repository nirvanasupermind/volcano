
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

uint64_t x = (*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromFloat"))->hidden_data))({Double,12360824618422894593ULL});
for(uint64_t i = 1ULL;((unpack_number(i)<unpack_number(12360817751404613633ULL) ? 10ULL : 2ULL)) != 2ULL;i = pack_number(unpack_number(i) + 1.0f)){
(*(func_ptr*)(unpack_object(unpack_object(x)->get("inc"))->hidden_data))({x});
}


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

free_all_objects();
return 0;
}