
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_member(tachyon_internal::decode_obj(StringUtils),"repr")))({StringUtils,tachyon_internal::make_vec(new std::vector<double>({1.00000000000000000,2.00000000000000000,3.00000000000000000}))})});

tachyon_internal::free_all();
return 0;
}