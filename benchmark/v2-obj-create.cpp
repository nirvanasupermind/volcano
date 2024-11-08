
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double start=(*tachyon_internal::decode_func(getTimeMillis))({});
for(double i=0;((double)((i)<(1e+06)));i++) {{
tachyon_internal::make_obj(new TACHYON_OBJ({}));
}}
double end=(*tachyon_internal::decode_func(getTimeMillis))({});
(*tachyon_internal::decode_func(println))({(end)-(start)});

tachyon_internal::free_all();
return 0;
}