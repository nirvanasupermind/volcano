
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
uint64_t x = (*(func_ptr*)(unpack_object(unpack_object(Object)->get("clone"))->hidden_data))({Object});

free_all_objects();
return 0;
}