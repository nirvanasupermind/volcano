#include <iostream>
#include "pilot-stl.h"

int main() {
    tachyon_stl_setup();
    auto start = std::chrono::system_clock::now();

    double x = 0.0;
    for (double i = 0.0; i < 1000000.0; i++) {
        x++;
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    tachyon_internal::free_all();
}