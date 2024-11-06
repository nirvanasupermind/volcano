
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(getTimeMillis))({})});

tachyon_internal::free_all();
return 0;
}