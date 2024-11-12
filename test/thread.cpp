
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double t1=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(ThreadUtils),"makeThread")))({ThreadUtils,tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
{
for(double i=0.0;((double)((i)<(10.0000000000000000)));i++) {{
(*tachyon_internal::decode_func(println))({i});
}}
}
return tachyon_internal::null;
}))});
double t2=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(ThreadUtils),"makeThread")))({ThreadUtils,tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
{
for(double i=0.0;((double)((i)<(10.0000000000000000)));i++) {{
(*tachyon_internal::decode_func(println))({i});
}}
}
return tachyon_internal::null;
}))});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(ThreadUtils),"join")))({ThreadUtils,t1});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(ThreadUtils),"join")))({ThreadUtils,t2});

tachyon_internal::free_all();
return 0;
}