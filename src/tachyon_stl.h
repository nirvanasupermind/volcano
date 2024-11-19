#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <thread>
#include <utility>
#include <sstream>
#include <random>
#include <cmath>
#include <thread>
#include <map>
#include <chrono>
#include <memory>
#include <cstdio>

#define TACHYON_OBJ std::vector<std::pair<std::string, double> >
#define TACHYON_FUNC std::function<double(const std::vector<double>&)>
#define STR_TAG 0x7ffc000000000000
#define VEC_TAG 0x7ffd000000000000
#define FUNC_TAG 0x7ffe000000000000
#define THREAD_TAG 0x7fff000000000000
#define FILE_TAG 0xfffc00000000000
#define OBJ_TAG 0xfffd000000000000
#define NULL_TAG 0xfffe000000000000
#define PTR_MASK 0xfffffffffff
#define TYPECHECK_MASK 0xffff000000000000

// Namespace to encapsulate internals for Tachyon
namespace tachyon_internal {
    std::vector<void*> all_ptrs;
    double null;
    // std::vector<std::fstream*> all_fstreams;
    // double fstream_id;

    inline double make_str(std::string* str) {
        all_ptrs.push_back(str);
        uint64_t u = ((uint64_t)str) | STR_TAG;
        return *(double*)(&u);
    }

    inline double make_vec(std::vector<double>* vec) {
        all_ptrs.push_back(vec);
        uint64_t u = ((uint64_t)vec) | VEC_TAG;
        return *(double*)(&u);
    }

    inline double make_func(TACHYON_FUNC* func) {
        all_ptrs.push_back(func);
        uint64_t u = ((uint64_t)func) | FUNC_TAG;
        return *(double*)(&u);
    }

    inline double make_thread(std::thread* th) {
        all_ptrs.push_back(th);
        uint64_t u = ((uint64_t)th) | THREAD_TAG;
        return *(double*)(&u);
    }

    inline double make_file(FILE* f) {
        // all_ptrs.push_back(f);
        uint64_t u = ((uint64_t)f) | FILE_TAG;
        return *(double*)(&u);
    }

    inline double make_obj(TACHYON_OBJ* obj) {
        all_ptrs.push_back(obj);
        uint64_t u = ((uint64_t)obj) | OBJ_TAG;
        return *(double*)(&u);
    }



    inline double make_void_ptr(void* ptr) {
        uint64_t u = ((uint64_t)ptr);
        return *(double*)(&u);
    }

    inline std::string* decode_str(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (std::string*)(ptr);
    }

    inline std::vector<double>* decode_vec(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (std::vector<double>*)(ptr);
    }

    inline TACHYON_FUNC* decode_func(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (TACHYON_FUNC*)(ptr);
    }

    inline std::thread* decode_thread(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (std::thread*)(ptr);
    }

    inline FILE* decode_file(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (FILE*)(ptr);
    }

    inline TACHYON_OBJ* decode_obj(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (TACHYON_OBJ*)(ptr);
    }

    inline void* decode_void_ptr(double d) {
        uint64_t ptr = *(uint64_t*)(&d);
        return (void*)(ptr);
    }

    inline void free_all() {
        for (void* ptr : all_ptrs) {
            if (ptr) free(ptr);
        }

        all_ptrs.clear();
        // // Close all file streams
        // for (std::fstream* fstream : all_fstreams) {
        //     if (fstream->is_open()) {
        //         fstream->close();
        //     }
        //     delete fstream;
        // }
        // all_fstreams.clear();
    }

    inline double get_member(TACHYON_OBJ* obj, const std::string& key) {

        for (auto& pair : *obj) {
            if (pair.first == key) return pair.second;
        }

        for (auto& pair : *obj) {
            if (pair.first == "proto") return get_member(decode_obj(pair.second), key);
        }


        throw std::runtime_error("key not found: " + key);
    }

    inline bool has_member(TACHYON_OBJ* obj, const std::string& key) {
        for (auto& pair : *obj) {
            if (pair.first == key) return true;
        }
        for (auto& pair : *obj) {
            if (pair.first == "proto") return has_member(decode_obj(pair.second), key);
        }
        return false;
    }

    inline double set_member(TACHYON_OBJ* obj, const std::string& key, double val) {
        for (size_t i = 0; i < obj->size(); ++i) {
            if ((*obj)[i].first == key) {
                (*obj)[i].second = val;
                return val;
            }
        }
        obj->emplace_back(key, val);
        return val;
    }

    inline double is_eq(double a, double b) {
        uint64_t a_u = *(uint64_t*)(&a);
        uint64_t b_u = *(uint64_t*)(&b);

        return a_u == b_u;
    }

    inline double is_ne(double a, double b) {
        uint64_t a_u = *(uint64_t*)(&a);
        uint64_t b_u = *(uint64_t*)(&b);

        return a_u != b_u;
    }


    inline bool is_str(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == STR_TAG;
    }

    inline bool is_vec(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == VEC_TAG;
    }

    inline bool is_func(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == FUNC_TAG;
    }

    inline bool is_thread(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == THREAD_TAG;
    }

    inline bool is_file(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == FILE_TAG;
    }

    inline bool is_obj(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == OBJ_TAG;
    }


    inline double get_subscript(double base, double idx) {
        if (is_str(base)) {
            return make_str(new std::string(1, decode_str(base)->at(idx)));
        }
        else if (is_vec(base)) {
            return decode_vec(base)->at(idx);
        }
        else if (is_obj(base)) {
            return get_member(decode_obj(base), *decode_str(idx));
        }
        else {
            throw std::runtime_error("illegal subscript");
        }
    }

    inline double set_subscript(double base, double idx, double val) {
        if (is_str(base)) {
            (*decode_str(base))[idx] = val;
        }
        else if (is_vec(base)) {
            (*decode_vec(base))[idx] = val;
        }
        else if (is_obj(base)) {
            set_member(decode_obj(base), *decode_str(idx), val);
        }
        else {
            throw std::runtime_error("illegal subscript");
        }
        return val;
    }
}

double Math = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double StringUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double VectorUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double ThisThread = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double ThreadUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double FileUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));
// double FStream = tachyon_internal::make_obj(new TACHYON_OBJ({}));


double typeOf = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    double x = _args[0];
    if (tachyon_internal::is_str(x)) {
        return 1.0;
    }
    else if (tachyon_internal::is_vec(x)) {
        return 2.0;
    }
    else if (tachyon_internal::is_func(x)) {
        return 3.0;
    }
    else if (tachyon_internal::is_thread(x)) {
        return 4.0;
    }
    else if (tachyon_internal::is_file(x)) {
        return 5.0;
    }
    else if (tachyon_internal::is_obj(x)) {
        return 6.0;
    }
    else if (tachyon_internal::is_eq(x, tachyon_internal::null)) {
        return 7.0;
    }
    else {
        return 0.0;
    }
    }));


double getTimeMillis = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return ms.count();
    }));

double input = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    std::string result;
    std::cin >> result;
    return tachyon_internal::make_str(new std::string(result));
    }));

double print = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    double val = _args[0];
    if (tachyon_internal::is_obj(val)) {
        TACHYON_OBJ* obj = tachyon_internal::decode_obj(val);
        if (tachyon_internal::has_member(obj, "toString")) {
            double temp = (*tachyon_internal::decode_func(tachyon_internal::get_member(obj, "toString")))({ val });
            std::cout << "object at " << *tachyon_internal::decode_str(temp);
        }
        else {
            std::cout << obj;
        }
    }
    else if (tachyon_internal::is_str(val)) {
        std::cout << *tachyon_internal::decode_str(val);
    }
    else if (tachyon_internal::is_vec(val)) {
        std::vector<double> vec = *tachyon_internal::decode_vec(val);
        std::cout << '[';
        for (int i = 0; i < vec.size(); i++) {
            (*tachyon_internal::decode_func(print))({ vec[i] });
            if (i != vec.size() - 1) {
                std::cout << ',';
            }
        }
        std::cout << ']';
    }
    else if (tachyon_internal::is_func(val)) {
        std::cout << "function at " << tachyon_internal::decode_func(val);
    }
    else if (tachyon_internal::is_thread(val)) {
        std::cout << "thread at " << tachyon_internal::decode_thread(val);
    }
    else if (tachyon_internal::is_file(val)) {
        std::cout << "file at " << tachyon_internal::decode_file(val);
    }
    else if (val == tachyon_internal::null) {
        std::cout << "null";
    }
    else {
        std::cout << val;
    }
    return tachyon_internal::null;
    }));

double println = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    double val = _args[0];
    (*tachyon_internal::decode_func(print))({ val });
    std::cout << '\n';
    return tachyon_internal::null;
    }));

double exitProgram = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    tachyon_internal::free_all();
    std::exit(_args[0]);
    return tachyon_internal::null;
    }));

// Tachyon standard library setup function
void tachyon_stl_setup() {
    uint64_t u = NULL_TAG;
    tachyon_internal::null = *(double*)(&u);

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "abs", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::abs(_args[1]);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "exp", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::exp(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "exp2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::exp2(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "expm1", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::expm1(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "log", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "log10", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log10(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "log2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log2(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "log1p", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log1p(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "pow", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::pow(_args[1], _args[2]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "sqrt", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sqrt(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "cbrt", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cbrt(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "hypot", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::hypot(_args[1], _args[2]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "cos", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cos(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "sin", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sin(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "tan", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tan(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "acos", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::acos(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "asin", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::asin(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "atan", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atan(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "atan2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atan2(_args[1], _args[2]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "cosh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cosh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "sinh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sinh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "tanh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tanh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "acosh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::acosh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "asinh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::asinh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "atanh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atanh(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "erf", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::erf(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "erfc", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::erfc(_args[1]);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "gamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tgamma(_args[1]);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "lgamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::lgamma(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "ceil", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::ceil(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "floor", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::floor(_args[1]);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "trunc", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::trunc(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "round", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::round(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "isFinite", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isfinite(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "isInf", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isinf(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "isNaN", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isnan(_args[1]);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "random", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        static thread_local std::mt19937 generator(std::random_device{}());
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        return distribution(generator);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "INFINITY", INFINITY);
    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "NAN", NAN);
    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "E", 2.71828182845904523536028747135266250);
    tachyon_internal::set_member(tachyon_internal::decode_obj(Math), "PI", 3.14159265358979323846264338327950288);

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "front", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        return tachyon_internal::make_str(new std::string(1, str->front()));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "back", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        return tachyon_internal::make_str(new std::string(1, str->back()));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "empty", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        return str->empty();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "size", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        return str->size();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "clear", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        str->clear();
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "insert", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        double idx = _args[2];
        std::string* str2 = tachyon_internal::decode_str(_args[3]);
        str->insert(idx, *str2);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "erase", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        double idx = _args[1];
        double count = _args[2];
        str->erase(idx, count);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "popBack", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        str->pop_back();
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "append", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);
        str->append(*str2);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "replace", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        double pos = _args[2];
        double count = _args[3];
        std::string* str2 = tachyon_internal::decode_str(_args[4]);
        str->replace(pos, count, *str2);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "find", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);
        return str->find(*str2);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "rfind", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);
        return str->rfind(*str2);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "substr", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        return tachyon_internal::make_str(new std::string(str->substr(_args[2], _args[3])));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "compare", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);

        return str->compare(*str2);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "startsWith", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* pre = tachyon_internal::decode_str(_args[2]);

        return str->compare(0, pre->size(), *pre) == 0;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "endsWith", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* suffix = tachyon_internal::decode_str(_args[2]);

        return str->compare(str->size() - suffix->size(), suffix->size(), *suffix) == 0;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "contains", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);

        return str->find(*str2) != std::string::npos;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "substr", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        double pos = _args[2];
        double count = _args.size() >= 4 ? _args[3] : std::string::npos;

        return tachyon_internal::make_str(new std::string(str->substr(pos, count)));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "split", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* sep = tachyon_internal::decode_str(_args[2]);
        std::vector<double> result;

        auto start = 0U;
        auto end = str->find(*sep);
        while (end != std::string::npos) {
            result.push_back(tachyon_internal::make_str(new std::string(str->substr(start, end - start))));
            start = end + sep->size();
            end = str->find(*sep, start);
        }

        result.push_back(tachyon_internal::make_str(new std::string(str->substr(start, end))));
        return tachyon_internal::make_vec(new std::vector<double>(result));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "eq", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);
        return (*str) == (*str2);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "concat", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args[1]);
        std::string* str2 = tachyon_internal::decode_str(_args[2]);
        return tachyon_internal::make_str(new std::string(*str + *str2));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "repeat", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string str = *tachyon_internal::decode_str(_args[1]);
        double n = _args[2];

        if (n == 0) {
            str.clear();
            str.shrink_to_fit();
            return tachyon_internal::make_str(new std::string(str));
        }
        else if (n == 1 || str.empty()) {
            return tachyon_internal::make_str(new std::string(str));
        }
        const auto period = str.size();
        if (period == 1) {
            str.append(n - 1, str.front());
            return tachyon_internal::make_str(new std::string(str));
        }
        str.reserve(period * n);
        std::size_t m{ 2 };
        for (; m < n; m *= 2) str += str;
        str.append(str.c_str(), (n - (m / 2)) * period);
        return tachyon_internal::make_str(new std::string(str));
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "repr", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        double x = _args[0];
        if (tachyon_internal::is_obj(x)) {
            TACHYON_OBJ* obj = tachyon_internal::decode_obj(x);
            if (tachyon_internal::has_member(obj, "toString")) {
                return (*tachyon_internal::decode_func(tachyon_internal::get_member(obj, "toString")))({ x });
            }
            else {
                std::ostringstream oss;
                oss << "object at " << obj;
                return tachyon_internal::make_str(new std::string(oss.str()));
            }
        }
        else if (tachyon_internal::is_str(x)) {
            return x;
        }
        else if (tachyon_internal::is_vec(x)) {
            std::vector<double> vec = *tachyon_internal::decode_vec(x);
            std::ostringstream oss;
            oss << '[';
            for (int i = 0; i < vec.size(); i++) {
                (*tachyon_internal::decode_func(print))({ vec[i] });
                if (i != vec.size() - 1) {
                    oss << ',';
                }
            }
            oss << ']';
            return  tachyon_internal::make_str(new std::string(oss.str()));
        }
        else if (tachyon_internal::is_func(x)) {
            std::ostringstream oss;
            oss << "function at " << tachyon_internal::decode_func(x);
            return tachyon_internal::make_str(new std::string(oss.str()));
        }
        else if (tachyon_internal::is_thread(x)) {
            std::ostringstream oss;
            oss << "thread at " << tachyon_internal::decode_thread(x);
            return tachyon_internal::make_str(new std::string(oss.str()));
        }
        else if (tachyon_internal::is_file(x)) {
            std::ostringstream oss;
            oss << "file at " << tachyon_internal::decode_file(x);
            return tachyon_internal::make_str(new std::string(oss.str()));
        }
        else if (x == tachyon_internal::null) {
            return tachyon_internal::make_str(new std::string("null"));
        }
        else {
            std::ostringstream oss;
            oss << tachyon_internal::decode_file(x);
            return tachyon_internal::make_str(new std::string(oss.str()));
        }
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "toNumber", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::stod(*tachyon_internal::decode_str(_args[1]));
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "copy", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return tachyon_internal::make_str(new std::string(*tachyon_internal::decode_str(_args[1])));
        })));

        tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "fromCodePoint", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return tachyon_internal::make_str(new std::string(1, (char)_args[1]));
        })));

        tachyon_internal::set_member(tachyon_internal::decode_obj(StringUtils), "toCodePoint", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return (double)(tachyon_internal::decode_str(_args[1])->front());
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "size", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        return vec->size();
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "front", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        return vec->front();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "back", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        return vec->back();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "pushBack", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        vec->push_back(_args[2]);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "popBack", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        vec->pop_back();
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "insert", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        vec->insert(vec->begin() + (int)_args[2], _args[3]);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "clear", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        vec->clear();
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "subvec", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        return tachyon_internal::make_vec(new std::vector<double>(vec->begin() + _args[2], vec->begin() + _args[2] + _args[3]));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "join", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        std::string* sep = tachyon_internal::decode_str(_args[2]);
        std::ostringstream result;
        for (int i = 0; i < vec->size(); i++) {
            result << *tachyon_internal::decode_str(vec->at(i));
            if (i != vec->size() - 1) {
                result << *sep;
            }
        }

        return tachyon_internal::make_str(new std::string(result.str()));
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "transform", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        TACHYON_FUNC* func = tachyon_internal::decode_func(_args[2]);
        std::vector<double>* result = new std::vector<double>(*(tachyon_internal::decode_vec(_args[1])));
        std::transform(result->begin(), result->end(), result->begin(), [func](double arg) -> double {
            return (*func)({ arg });
            });
        return tachyon_internal::make_vec(result);
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "accumulate", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        TACHYON_FUNC* func = tachyon_internal::decode_func(_args[3]);

        return std::accumulate(vec->begin(), vec->end(), _args[2], [func](double a, double b) -> double {
            return (*func)({ a,b });
            });
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "eq", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        std::vector<double>* vec2 = tachyon_internal::decode_vec(_args[2]);
        return (*vec) == (*vec2);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "concat", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec2 = tachyon_internal::decode_vec(_args[2]);
        std::vector<double>* result = new std::vector<double>(*tachyon_internal::decode_vec(_args[1]));
        result->insert(result->end(), vec2->begin(), vec2->end());
        return tachyon_internal::make_vec(result);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "repeat", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);
        double n = _args[2];

        // Handle edge cases
        if (vec->empty() || n <= 0) {
            return tachyon_internal::make_vec(new std::vector<double>({}));
        }

        // Preallocate output vector with the exact required size
        std::vector<double>* result = new std::vector<double>({});
        result->reserve(vec->size() * n);

        // Append input vector `n` times
        for (int i = 0; i < n; ++i) {
            result->insert(result->end(), vec->begin(), vec->end());
        }

        return tachyon_internal::make_vec(result);
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "copy", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return tachyon_internal::make_vec(new std::vector<double>(*tachyon_internal::decode_vec(_args[1])));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(VectorUtils), "reserve", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::vector<double>* vec = tachyon_internal::decode_vec(_args[1]);  
        vec->reserve(_args[2]);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThisThread), "yield", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::this_thread::yield();
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThisThread), "getID", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        uint64_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return tachyon_internal::make_str(new std::string(std::to_string(id)));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThisThread), "sleepFor", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)_args[1]));
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(ThisThread), "sleepUntil", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        double milliseconds_since_epoch = _args[1];


        auto time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>(
            std::chrono::milliseconds((long long)(milliseconds_since_epoch))
        );

        std::this_thread::sleep_until(time_point);

        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "makeThread", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        TACHYON_FUNC* func = tachyon_internal::decode_func(_args[1]);

        // Create a lambda to wrap the function call, providing it with the expected arguments
        auto thread_func = [func]() {
            // Assuming the thread function can be called with an empty vector as the argument
            return (*func)(std::vector<double>{});
            };

        // Pass the lambda to std::thread and wrap it with make_thread
        return tachyon_internal::make_thread(new std::thread(thread_func));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "joinable", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::thread* thread = tachyon_internal::decode_thread(_args[1]);
        return thread->joinable();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "getID", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::thread* thread = tachyon_internal::decode_thread(_args[1]);
        uint64_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return tachyon_internal::make_str(new std::string(std::to_string(id)));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "hardwareConcurrency", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::thread::hardware_concurrency();
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "join", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::thread* thread = tachyon_internal::decode_thread(_args[1]);
        if (thread && thread->joinable()) {
            thread->join();
        }
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(ThreadUtils), "detach", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::thread* thread = tachyon_internal::decode_thread(_args[1]);
        thread->detach();
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(ThisThread), "getID", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        uint64_t id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return tachyon_internal::make_str(new std::string(std::to_string(id)));
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "open", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return tachyon_internal::make_file(fopen(tachyon_internal::decode_str(_args[1])->c_str(), tachyon_internal::decode_str(_args[2])->c_str()));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "reopen", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        freopen(tachyon_internal::decode_str(_args[1])->c_str(), tachyon_internal::decode_str(_args[2])->c_str(), file);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "close", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        fclose(file);
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "flush", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        fflush(file);
        return tachyon_internal::null;
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "getc", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        return tachyon_internal::make_str(new std::string(1, fgetc(file)));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "gets", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        char* str;
        return tachyon_internal::make_str(new std::string(fgets(str, _args[2], file)));
        })));

    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "puts", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        fputs(tachyon_internal::decode_str(_args[2])->c_str(), file);
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "tell", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        return ftell(file);
        })));



    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "seek", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        fseek(file, _args[2], _args[3]);
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "rewind", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        rewind(file);
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "clearErr", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        clearerr(file);
        return tachyon_internal::null;
        })));


    tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "eof", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        return feof(file);
        })));

        tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "error", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        return ferror(file);
        })));
        

        tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "remove", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        remove(tachyon_internal::decode_str(_args[1])->c_str());
        return tachyon_internal::null;
        })));


        tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "rename", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        FILE* file = tachyon_internal::decode_file(_args[1]);
        rename(tachyon_internal::decode_str(_args[1])->c_str(),tachyon_internal::decode_str(_args[2])->c_str() );
        return tachyon_internal::null;
        })));

        tachyon_internal::set_member(tachyon_internal::decode_obj(FileUtils), "tmpFile", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return tachyon_internal::make_file(tmpfile());
        })));
        
    // tachyon_internal::set_member(tachyon_internal::decode_obj(FStreamUtils), "makeFStream", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
    //     std::fstream f(*(tachyon_internal::decode_str(_args[1]))); // Open a file named "sample.txt" for writing

    //     if (!f.is_open()) { // Check if the file opened successfully
    //         std::cerr << "Unable to open file"; // Handle the error if the file couldn't be opened
    //     }

    //     return tachyon_internal::make_fstream(new std::fstream(f));
    // })));

    //     tachyon_internal::set_member(tachyon_internal::decode_obj(FStreamUtils), "write", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
    //         std::fstream* f = tachyon_internal::decode_fstream(_args[1]);
    //         // std::cout << "814 " << f << '\n';
    //         std::string* s = tachyon_internal::decode_str(_args[2]);
    //         f->write(s->c_str(), s->size());

    //         return tachyon_internal::null;
    //         })));

    //     tachyon_internal::set_member(tachyon_internal::decode_obj(FStreamUtils), "close", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
    //         std::fstream* f = tachyon_internal::decode_fstream(_args[1]);
    //         f->close();

    //         return tachyon_internal::null;
    //         })));



        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "fromPath", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     std::fstream* f = new std::fstream(*tachyon_internal::decode_str(_args[0]), std::ios::in | std::ios::out);
        //     tachyon_internal::all_fstreams.push_back(f);
        //     return tachyon_internal::make_obj(new TACHYON_OBJ({{"proto", FStream}, {"id", tachyon_internal::all_fstreams.size() - 1}}));
        // })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "open", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     f->open(*tachyon_internal::decode_str(_args[1]));
        //     return tachyon_internal::null;
        //     })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "gcount", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     return f->gcount();
        //     })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "get", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     return tachyon_internal::make_str(new std::string(1, f->get()));
        //     })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "getLine", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     char* c;
        //     f->getline(c, _args[2]);
        //     return tachyon_internal::make_str(new std::string(c));
        //     })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "peek", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     return tachyon_internal::make_str(new std::string(1, f->peek()));
        //     })));


        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "read", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     char* c;
        //     f->read(c, _args[1]);

        //     return tachyon_internal::make_str(new std::string(1, f->get()));
        //     })));

        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "write", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     std::string* s = tachyon_internal::decode_str(_args[1]);
        //     f->write(s->c_str(), s->size());

        //     return tachyon_internal::null;
        //     })));


        // tachyon_internal::set_member(tachyon_internal::decode_obj(FStream), "close", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     double id = tachyon_internal::get_member(tachyon_internal::decode_obj(_args[0]), "id");
        //     std::fstream* f = tachyon_internal::all_fstreams[id];
        //     f->close();

        //     return tachyon_internal::null;
        //     })));

        // tachyon_internal::all_objs.reserve(1000000);
        // Initialize any required standard library components here
        // Initialize any required standard library components here
        // tachyon_internal::set_member(tachyon_internal::decode_obj(String), "toString", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        //     return _args[0];
        // })));
}