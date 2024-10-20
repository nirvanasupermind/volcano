
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
uint64_t f = create_object(new std::unordered_map<std::string, uint64_t>({}), new func_ptr([=] (const std::vector<uint64_t>& _args) {
uint64_t x= _args.at(0);
{
return (*(func_ptr*)(unpack_object(unpack_object(x)->get("mul"))->hidden_data))({x,x});
}
return 1ULL;
}));
(*(func_ptr*)(unpack_object(print)->hidden_data))({(*(func_ptr*)(unpack_object(f)->hidden_data))({(*(func_ptr*)(unpack_object(unpack_object(Double)->get("fromFloat"))->hidden_data))({Double,4383047681ULL})})});

free_all_objects();
return 0;
}