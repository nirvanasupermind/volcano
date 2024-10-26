#include <iostream>
#include "pilot-stl.h"

int main() {
    auto start = std::chrono::system_clock::now();
 
    for(TachyonVal i = TachyonVal(0.0); i.num < TachyonVal(1000000.0).num; i.num++) {
        TachyonObject({}, Object);
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}