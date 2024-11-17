
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({5.00000000000000000});
(*tachyon_internal::decode_func(println))({18446744073709551616.000000});

tachyon_internal::free_all();
return 0;
}