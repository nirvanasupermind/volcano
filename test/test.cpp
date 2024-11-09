
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double f=(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FStreamUtils),"makeFStream")))({FStreamUtils,tachyon_internal::make_str(new std::string("a/b.txt"))});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FStreamUtils),"open")))({FStreamUtils,f,tachyon_internal::make_str(new std::string("a/b.txt"))});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FStreamUtils),"write2")))({FStreamUtils,f,tachyon_internal::make_str(new std::string("Hello world!"))});
(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(FStreamUtils),"close")))({FStreamUtils,f});

tachyon_internal::free_all();
return 0;
}