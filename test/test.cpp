
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(Math),"hypot")))({Math,3.0,4.0})});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(Math),"gamma")))({Math,5.0})});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(StringUtils),"back")))({StringUtils,tachyon_internal::make_str(new std::string("abc"))})});

tachyon_internal::free_all();
return 0;
}