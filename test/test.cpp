
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=tachyon_internal::make_obj(new TACHYON_OBJ({{"a",2}}));
tachyon_internal::set_subscript(x,tachyon_internal::make_str(new std::string("a")),(tachyon_internal::get_subscript(x,tachyon_internal::make_str(new std::string("a")))+1.0));
(*tachyon_internal::decode_func(println))({tachyon_internal::get_subscript(x,tachyon_internal::make_str(new std::string("a")))});

tachyon_internal::free_all();
return 0;
}