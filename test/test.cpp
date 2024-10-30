
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
(*tachyon_internal::decode_func(print))({tachyon_internal::make_obj(new TACHYON_OBJ({{"toString",tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
{
return tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string("hi"))}}));
}}))}}))});

tachyon_internal::free_all();
return 0;
}