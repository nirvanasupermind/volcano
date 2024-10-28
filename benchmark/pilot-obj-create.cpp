#include <iostream>
#include "pilot-stl.h"

int main() {
    auto start = std::chrono::system_clock::now();

    for (tachyon_internal::Val i = tachyon_internal::Val(0.0f);
        tachyon_internal::Val(i.num < tachyon_internal::Val(1000000.0f).num).num;
        i.num++) {
        tachyon_internal::make_obj_val();
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}