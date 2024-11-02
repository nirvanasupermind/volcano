
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(Math),"pow")))({Math,2.0,3.0})});

tachyon_internal::free_all();
return 0;
}