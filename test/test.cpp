
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=1;
while(true){
x*=(3);
if((double)((x)>=(10000))){
break;
}
}
(*tachyon_internal::decode_func(println))({x});

tachyon_internal::free_all();
return 0;
}