
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
if(1){
(*tachyon_internal::decode_func(print))({tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string("Hello world!"))}}))});
}else if(0){
(*tachyon_internal::decode_func(print))({tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string("maybe hello"))}}))});
}else{
(*tachyon_internal::decode_func(print))({tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",String},{"_voidPtr",tachyon_internal::make_void_ptr(new std::string("Bye bye world!"))}}))});
}

tachyon_internal::free_all();
return 0;
}