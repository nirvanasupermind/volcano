
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double a=tachyon_internal::make_vec(new std::vector<double>({1.00000000000000000,2.00000000000000000,3.00000000000000000}));
double b=tachyon_internal::make_vec(new std::vector<double>({4.00000000000000000,5.00000000000000000,6.00000000000000000}));
(*tachyon_internal::decode_func(print))({a});
(*tachyon_internal::decode_func(print))({tachyon_internal::make_str(new std::string(" "))});
(*tachyon_internal::decode_func(print))({b});
(*tachyon_internal::decode_func(print))({tachyon_internal::make_str(new std::string(" "))});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(VectorUtils),"concat")))({VectorUtils,a,b})});
double c=tachyon_internal::make_str(new std::string("abc"));
double d=tachyon_internal::make_str(new std::string("def"));
(*tachyon_internal::decode_func(print))({c});
(*tachyon_internal::decode_func(print))({tachyon_internal::make_str(new std::string(" "))});
(*tachyon_internal::decode_func(print))({d});
(*tachyon_internal::decode_func(print))({tachyon_internal::make_str(new std::string(" "))});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(StringUtils),"concat")))({StringUtils,c,d})});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(StringUtils),"repeat")))({StringUtils,tachyon_internal::make_str(new std::string("abc")),3.00000000000000000})});
(*tachyon_internal::decode_func(println))({(*tachyon_internal::decode_func(tachyon_internal::get_prop(tachyon_internal::decode_obj(VectorUtils),"repeat")))({VectorUtils,tachyon_internal::make_vec(new std::vector<double>({1.00000000000000000,2.00000000000000000,3.00000000000000000})),3.00000000000000000})});

tachyon_internal::free_all();
return 0;
}