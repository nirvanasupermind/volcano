
#include "/usr/local/include/tachyon_stl.h"
    
int main(){
tachyon_stl_setup();
double x=5;
x++;

tachyon_internal::free_all();
return 0;
}