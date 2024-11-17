
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=1.00000000000000000;
x&&=(0.0);
(*tachyon_internal::decode_func(println))({x});

tachyon_internal::free_all();
return 0;
}