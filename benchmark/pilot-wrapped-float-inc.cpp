#include <iostream>
#include "pilot-stl.h"

int main() {
    tachyon_stl_setup();
    auto start = std::chrono::system_clock::now();

    double Wrapper = tachyon_internal::make_obj(new TACHYON_OBJ({}));
    double x = tachyon_internal::make_obj(new TACHYON_OBJ({{"proto",Wrapper},{"val",0.0}}));
    for (double i = 0.0; i < 1000000.0; i++) {
        tachyon_internal::set_prop(tachyon_internal::decode_obj(x), "val", tachyon_internal::get_prop(tachyon_internal::decode_obj(x), "val") + 1.0);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    tachyon_internal::free_all();
}