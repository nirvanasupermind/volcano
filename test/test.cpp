
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
uint64_t a = (*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromString"))->other_data))({Double,pack_object(new TachyonObject(new std::unordered_map<std::string, uint64_t>({{"prototype",String}}),new std::string("1e+105")))});
(*(func_ptr*)(unpack_object(print)->other_data))({a});

return 0;
}