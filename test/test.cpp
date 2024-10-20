
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
(*(func_ptr*)(unpack_object(print)->other_data))({(*(func_ptr*)(unpack_object(unpack_object(Map)->get("createEmpty"))->other_data))({Map})});

free_all_objects();
return 0;
}