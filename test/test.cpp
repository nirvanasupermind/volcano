
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=tachyon_internal::make_vec(new std::vector<double>({5,5,4}));
(*tachyon_internal::decode_func(println))({x});

tachyon_internal::free_all();
return 0;
}