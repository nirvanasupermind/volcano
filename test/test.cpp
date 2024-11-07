
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=tachyon_internal::make_obj(new TACHYON_OBJ({{"val",10}}));
tachyon_internal::set_prop(tachyon_internal::decode_obj(x),"val",(int32_t)(tachyon_internal::get_prop(tachyon_internal::decode_obj(x),"val"))&(int32_t)((13)));
tachyon_internal::set_prop(tachyon_internal::decode_obj(x),"val",std::fmod(tachyon_internal::get_prop(tachyon_internal::decode_obj(x),"val"),(5)));
tachyon_internal::set_prop(tachyon_internal::decode_obj(x),"val",tachyon_internal::get_prop(tachyon_internal::decode_obj(x),"val")/(13));
tachyon_internal::set_subscript(x,tachyon_internal::make_str(new std::string("val")),(int32_t)(tachyon_internal::get_subscript(x,tachyon_internal::make_str(new std::string("val"))))&(int32_t)(29));
tachyon_internal::set_subscript(x,tachyon_internal::make_str(new std::string("val")),std::fmod((tachyon_internal::get_subscript(x,tachyon_internal::make_str(new std::string("val")))),(11)));
tachyon_internal::set_subscript(x,tachyon_internal::make_str(new std::string("val")),(tachyon_internal::get_subscript(x,tachyon_internal::make_str(new std::string("val"))))-(209));
(*tachyon_internal::decode_func(println))({tachyon_internal::get_prop(tachyon_internal::decode_obj(x),"val")});

tachyon_internal::free_all();
return 0;
}