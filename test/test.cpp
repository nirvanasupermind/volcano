
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(println))({(1.00000000000000000)&&(0.0)});
(*tachyon_internal::decode_func(println))({(1.00000000000000000)||(1.00000000000000000)});
(*tachyon_internal::decode_func(println))({(9.00000000000000000)||(7.00000000000000000)});
(*tachyon_internal::decode_func(println))({!(12.0000000000000000)});

tachyon_internal::free_all();
return 0;
}