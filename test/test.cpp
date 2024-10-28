
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
tachyon_internal::Val Complex=(*(tachyon_internal::func_type*)((Object.obj->get("clone")).obj->hidden_data))({Object});
(Complex).obj->set("fromComponents",tachyon_internal::make_obj({},Function.obj,new tachyon_internal::func_type([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
tachyon_internal::Val self= _args.at(0);
tachyon_internal::Val real= _args.at(1);
tachyon_internal::Val imag= _args.at(2);
{
tachyon_internal::Val result=(*(tachyon_internal::func_type*)((Complex.obj->get("clone")).obj->hidden_data))({Complex});
(result).obj->set("real",real);
(result).obj->set("imag",imag);
return result;
}})));
(Complex).obj->set("add",tachyon_internal::make_obj({},Function.obj,new tachyon_internal::func_type([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
tachyon_internal::Val self= _args.at(0);
tachyon_internal::Val other= _args.at(1);
{
return (*(tachyon_internal::func_type*)((Complex.obj->get("fromComponents")).obj->hidden_data))({Complex,tachyon_internal::Val((self.obj->get("real")).num+(other.obj->get("real")).num),tachyon_internal::Val((self.obj->get("imag")).num+(other.obj->get("imag")).num)});
}})));
tachyon_internal::Val a=(*(tachyon_internal::func_type*)((Complex.obj->get("fromComponents")).obj->hidden_data))({Complex,tachyon_internal::Val(1),tachyon_internal::Val(1)});
tachyon_internal::Val b=(*(tachyon_internal::func_type*)((Complex.obj->get("fromComponents")).obj->hidden_data))({Complex,tachyon_internal::Val(2),tachyon_internal::Val(3)});
tachyon_internal::Val c=(*(tachyon_internal::func_type*)((a.obj->get("add")).obj->hidden_data))({a,b});
(*(tachyon_internal::func_type*)((print).obj->hidden_data))({c.obj->get("real")});
(*(tachyon_internal::func_type*)((print).obj->hidden_data))({c.obj->get("imag")});

tachyon_internal::free_all_objs();
return 0;
}