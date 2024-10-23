
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

uint64_t x = 13008784687735242753ULL;
for(uint64_t i = 16249494450672238593ULL;((unpack_number(i)<unpack_number(13008773147792543745ULL) ? 10ULL : 2ULL)) != 2ULL;i = pack_number(unpack_number(i) + 1.0f)){
x = pack_number(unpack_number(x) + 1.0f);
}


    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";


free_all_objects();
return 0;
}