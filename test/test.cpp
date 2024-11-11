
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double f=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FileUtils),"open")))({FileUtils,tachyon_internal::make_str(new std::string("test/test.txt")),tachyon_internal::make_str(new std::string("rw"))});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FileUtils),"puts")))({FileUtils,f,tachyon_internal::make_str(new std::string("hi"))});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FileUtils),"gets")))({FileUtils,f,3})});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FileUtils),"close")))({FileUtils,f});

tachyon_internal::free_all();
return 0;
}