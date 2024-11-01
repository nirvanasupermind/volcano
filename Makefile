default:
	clang++ src/*.cpp -o /usr/local/bin/tachyonc -O3 -std=c++11
	cp src/tachyon_stl.h /usr/local/include/tachyon_stl.h

# ifeq ($(OS),Windows_NT)
#     SOURCE := src\*.cpp
#     TARGET := C:\Program Files\tachyonc
# else
#     SOURCE := src/*.cpp
#     TARGET := /usr/local/bin/tachyonc
# endif

# default:
# 	clang++ $(SOURCE) -o $(TARGET) -Wno-return-type -std=c++11