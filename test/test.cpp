
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_internal::Object({},Map,new std::unordered_map<std::string, tachyon_internal::Val>({{*(std::string*)(tachyon_internal::to_object(tachyon_internal::Object({},String,new std::string("abc")))).hidden_data,tachyon_internal::Val(123)}}));

return 0;
}