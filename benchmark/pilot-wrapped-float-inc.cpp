#include <iostream>
#include "pilot-stl.h"

int main() {
    tachyon_stl_setup();
    auto start = std::chrono::system_clock::now();

    double Wrapper = tachyon_internal::make_obj({}, tachyon_internal::decode_obj(Object));
    double x =  tachyon_internal::make_obj({}, tachyon_internal::decode_obj(Wrapper));
    tachyon_internal::decode_obj(x)->set("val", 0.0);
    for (double i = 0.0; i < 1000000.0; i++) {
        tachyon_internal::decode_obj(x)->set("val", tachyon_internal::decode_obj(x)->get("val") + 1.0);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    tachyon_internal::free_all();
}