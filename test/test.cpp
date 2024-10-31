
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
try{
tachyon_internal::get_prop(tachyon_internal::decode_obj(tachyon_internal::make_obj(new TACHYON_OBJ({}))),"b");
}catch(const std::exception& _err) {
double e=tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string(_err.what()))}}));
{
(*tachyon_internal::decode_func(print))({tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string("THE ERROR WAS:"))}}))});
(*tachyon_internal::decode_func(print))({e});
}
}

tachyon_internal::free_all();
return 0;
}