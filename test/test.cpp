
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double Complex=tachyon_internal::make_obj(new TACHYON_OBJ({{"add",tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
double self= _args.at(0);
double other= _args.at(1);
{
return tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Complex},{"real",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"real"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"real"))},{"imag",(tachyon_internal::get_prop(tachyon_internal::decode_obj(self),"imag"))+(tachyon_internal::get_prop(tachyon_internal::decode_obj(other),"imag"))}}));
}}))}}));
tachyon_internal::get_prop(tachyon_internal::decode_obj(Complex),"add");

tachyon_internal::free_all();
return 0;
}