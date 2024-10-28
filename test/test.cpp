
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
tachyon_internal::Val Complex=(*(Object.obj->get("clone").func))({Object});
(Complex).obj->set("fromComponents",tachyon_internal::make_func([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
tachyon_internal::Val self= _args.at(0);
tachyon_internal::Val real= _args.at(1);
tachyon_internal::Val imag= _args.at(2);
{
tachyon_internal::Val result=(*(Complex.obj->get("clone").func))({Complex});
(result).obj->set("real",real);
(result).obj->set("imag",imag);
return result;
}}));
(Complex).obj->set("add",tachyon_internal::make_func([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
tachyon_internal::Val self= _args.at(0);
tachyon_internal::Val other= _args.at(1);
{
return (*(Complex.obj->get("fromComponents").func))({Complex,tachyon_internal::Val((self.obj->get("real")).num+(other.obj->get("real")).num),tachyon_internal::Val((self.obj->get("imag")).num+(other.obj->get("imag")).num)});
}}));
tachyon_internal::Val a=(*(Complex.obj->get("fromComponents").func))({Complex,tachyon_internal::Val(1),tachyon_internal::Val(1)});
tachyon_internal::Val b=(*(Complex.obj->get("fromComponents").func))({Complex,tachyon_internal::Val(2),tachyon_internal::Val(3)});
tachyon_internal::Val c=(*(a.obj->get("add").func))({a,b});
(*(print.func))({c.obj->get("real")});
(*(print.func))({c.obj->get("imag")});

tachyon_internal::free_all();
return 0;
}