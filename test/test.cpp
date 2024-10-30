
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(print))({tachyon_internal::get_prop(tachyon_internal::decode_obj(tachyon_internal::make_obj(new TACHYON_OBJ({{"a",2}}))),"a")});

tachyon_internal::free_all();
return 0;
}