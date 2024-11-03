
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(tachyon_internal::get_prop(tachyon_internal::decode_obj(Math),"PI"))+(1)});

tachyon_internal::free_all();
return 0;
}