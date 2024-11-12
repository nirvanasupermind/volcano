
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double Complex=tachyon_internal::make_obj(new TACHYON_OBJ({{"add",tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
double self= _args[0];
double other= _args[1];
{
return tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"real"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"real"))},{"imag",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"imag"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"imag"))}}));
}
return tachyon_internal::null;
}))}}));
double a=tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",1.00000000000000000},{"imag",1.00000000000000000}}));
double b=tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",2.00000000000000000},{"imag",3.00000000000000000}}));
double c=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(a),"add")))({a,b});
(*tachyon_internal::decode_func(println))({tachyon_internal::get_prop(tachyon_internal::decode_obj(c),"real")});
(*tachyon_internal::decode_func(println))({tachyon_internal::get_prop(tachyon_internal::decode_obj(c),"imag")});

tachyon_internal::free_all();
return 0;
}