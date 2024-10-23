
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
uint64_t Complex = (*(func_ptr*)(unpack_object(unpack_object(Object)->get("clone"))->hidden_data))({Object});
unpack_object(Complex)->set("fromComponents",create_object({}, new func_ptr([=] (const std::vector<uint64_t>& _args) -> uint64_t {
uint64_t self= _args.at(0);
uint64_t real= _args.at(1);
uint64_t imag= _args.at(2);
{
uint64_t result = (*(func_ptr*)(unpack_object(unpack_object(Complex)->get("clone"))->hidden_data))({Complex});
unpack_object(result)->set("real",real);
unpack_object(result)->set("imag",imag);
return result;
}
return 1ULL;
})));
unpack_object(Complex)->set("add",create_object({}, new func_ptr([=] (const std::vector<uint64_t>& _args) -> uint64_t {
uint64_t self= _args.at(0);
uint64_t other= _args.at(1);
{
return (*(func_ptr*)(unpack_object(unpack_object(Complex)->get("fromComponents"))->hidden_data))({Complex,pack_number(unpack_number(unpack_object(self)->get("real"))+unpack_number(unpack_object(other)->get("real"))),pack_number(unpack_number(unpack_object(self)->get("imag"))+unpack_number(unpack_object(other)->get("imag")))});
}
return 1ULL;
})));
uint64_t a = (*(func_ptr*)(unpack_object(unpack_object(Complex)->get("fromComponents"))->hidden_data))({Complex,13676716840271740929ULL,126689358966161409ULL});
uint64_t b = (*(func_ptr*)(unpack_object(unpack_object(Complex)->get("fromComponents"))->hidden_data))({Complex,13676716840305295361ULL,126689359016493057ULL});
uint64_t c = (*(func_ptr*)(unpack_object(unpack_object(a)->get("add"))->hidden_data))({a,b});
(*(func_ptr*)(unpack_object(print)->hidden_data))({unpack_object(c)->get("real")});
(*(func_ptr*)(unpack_object(print)->hidden_data))({unpack_object(c)->get("imag")});

free_all_objects();
return 0;
}