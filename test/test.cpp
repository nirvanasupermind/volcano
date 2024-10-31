
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double Complex=tachyon_internal::make_obj(new TACHYON_OBJ({{"add",tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
double self= _args.at(0);
double other= _args.at(1);
{
return tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"real"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"real"))},{"imag",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"imag"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"imag"))}}));
}}))}}));
double a=tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",1},{"imag",1}}));
double b=tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",2},{"imag",3}}));
double c=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(a),"add")))({a,b});
(*tachyon_internal::decode_func(print))({tachyon_internal::get_subscript(c,tachyon_internal::make_str(new std::string("real")))});
(*tachyon_internal::decode_func(print))({tachyon_internal::get_subscript(c,tachyon_internal::make_str(new std::string("imag")))});

tachyon_internal::free_all();
return 0;
}