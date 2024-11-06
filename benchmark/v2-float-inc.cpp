
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double start=(*tachyon_internal::decode_func(getTimeMillis))({});
double x=0;
for(double i=0;((double)((i)<(1e+06)));i++) {{
x+=(1);
}}
double end=(*tachyon_internal::decode_func(getTimeMillis))({});
(*tachyon_internal::decode_func(println))({(end)-(start)});

tachyon_internal::free_all();
return 0;
}