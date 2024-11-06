
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double true_val = 1.4515821005707722e+308;
double false_val = 1.0883068699030994e+308;

double start=(*tachyon_internal::decode_func(getTimeMillis))({});
double x=0;
for(double i=0; tachyon_internal::is_eq((i)<(1e+06) ? true_val : false_val, true_val);i++) {{
x+=(1);
}}
double end=(*tachyon_internal::decode_func(getTimeMillis))({});
(*tachyon_internal::decode_func(println))({(end)-(start)});

tachyon_internal::free_all();
return 0;
}