
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double start=(*tachyon_internal::decode_func(getTimeMillis))({});
double Wrapper=tachyon_internal::make_obj(new TACHYON_OBJ({}));
double x=tachyon_internal::make_obj(new TACHYON_OBJ({{"val",0},{"proto",Wrapper}}));
for(double i=0;((double)((i)<(1e+06)));i++) {{
tachyon_internal::set_prop(tachyon_internal::decode_obj(x),"val",(tachyon_internal::get_prop(tachyon_internal::decode_obj(x),"val")+1.0));
}}
double end=(*tachyon_internal::decode_func(getTimeMillis))({});
(*tachyon_internal::decode_func(println))({(end)-(start)});

tachyon_internal::free_all();
return 0;
}