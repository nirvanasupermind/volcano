
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({tachyon_internal::make_str(new std::string("Hello world!"))});

tachyon_internal::free_all();
return 0;
}