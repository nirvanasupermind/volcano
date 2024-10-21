
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
uint64_t x = (*(func_ptr*)(unpack_object(unpack_object(UnorderedMap)->get("createEmpty"))->hidden_data))({UnorderedMap});
(*(func_ptr*)(unpack_object(unpack_object(x)->get("set"))->hidden_data))({x,create_object(new std::unordered_map<std::string, uint64_t>({{"prototype",String}}),new std::string("a")),4294967297ULL});
(*(func_ptr*)(unpack_object(print)->hidden_data))({(*(func_ptr*)(unpack_object(unpack_object(x)->get("has"))->hidden_data))({x,create_object(new std::unordered_map<std::string, uint64_t>({{"prototype",String}}),new std::string("a"))})});

free_all_objects();
return 0;
}