#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <utility>

#define TACHYON_OBJ std::vector<std::pair<std::string, double> >
#define TACHYON_FUNC std::function<double(const std::vector<double>&)>

// Using constexpr for tag values to reduce potential branching
constexpr uint64_t OBJ_TAG = 0x7ffc000000000000;
constexpr uint64_t FUNC_TAG = 0x7ffe000000000000;
constexpr uint64_t PTR_MASK = 0x1ffffffffffff;
constexpr uint64_t TYPECHECK_MASK = 0xfffe000000000000;

// Namespace to encapsulate internals for Tachyon
namespace tachyon_internal {
    std::vector<TACHYON_OBJ*> all_objs;
    std::vector<TACHYON_FUNC*> all_funcs;
    std::vector<void*> all_void_ptrs;

    int idx = 0;

    inline double make_obj(TACHYON_OBJ* obj) {
        all_objs.push_back(obj);
        uint64_t u = reinterpret_cast<uint64_t>(obj) | OBJ_TAG;
        return *reinterpret_cast<double*>(&u);
    }

    inline double make_func(TACHYON_FUNC* func) {
        all_funcs.push_back(func);
        uint64_t u = reinterpret_cast<uint64_t>(func) | FUNC_TAG;
        return *reinterpret_cast<double*>(&u);
    }

    double make_void_ptr(void* v) {
        all_void_ptrs.push_back(v);
        return *reinterpret_cast<double*>(&v);
    }

    inline TACHYON_OBJ* decode_obj(double d) {
        uint64_t ptr = reinterpret_cast<uint64_t&>(d) & PTR_MASK;
        return (TACHYON_OBJ*)(ptr);
    }

    inline TACHYON_FUNC* decode_func(double d) {
        uint64_t ptr = reinterpret_cast<uint64_t&>(d) & PTR_MASK;
        return reinterpret_cast<TACHYON_FUNC*>(ptr);
    }

    inline void* decode_void_ptr(double d) {
        return *reinterpret_cast<void**>(&d);
    }

    inline void free_all() {
        for (auto* obj : all_objs) {
            delete obj;
        }
        all_objs.clear();
        for (auto* func : all_funcs) {
            delete func;
        }
        all_funcs.clear();
        for (auto* v : all_void_ptrs) {
            free(v);
        }
        all_void_ptrs.clear();
    }


    inline double get_prop(TACHYON_OBJ* obj, const std::string& key) {
        for (auto& pair : *obj) {
            if (pair.first == key) return pair.second;
        }
        for (auto& pair : *obj) {
            if (pair.first == "proto") return get_prop(decode_obj(pair.second), key);
        }
        throw std::runtime_error("Key not found: " + key);
    }

    inline bool has_prop(TACHYON_OBJ* obj, const std::string& key) {
        for (auto& pair : *obj) {
            if (pair.first == key) return true;
        }
        for (auto& pair : *obj) {
            if (pair.first == "proto") return has_prop(decode_obj(pair.second), key);
        }
        return false;
    }

    inline double set_prop(TACHYON_OBJ* obj, const std::string& key, double val) {
        for (size_t i = 0; i < obj->size(); ++i) {
            if ((*obj)[i].first == key) {
                (*obj)[i].second = val;
                return val;
            }
        }
        obj->emplace_back(key, val);
        return val;
    }

    bool is_obj(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == OBJ_TAG;
    }

    bool is_func(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == FUNC_TAG;
    }
}

double String = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double Vector = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double Error = tachyon_internal::make_obj(new TACHYON_OBJ({}));

double print = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
        double x = _args.at(0);
        if(tachyon_internal::is_obj(x)) {
            TACHYON_OBJ* obj = tachyon_internal::decode_obj(x);
            if(tachyon_internal::has_prop(obj, "toString")) {
                double temp = (*tachyon_internal::decode_func(tachyon_internal::get_prop(obj, "toString")))({x});
                std::cout << *(std::string*)tachyon_internal::decode_void_ptr(tachyon_internal::get_prop(tachyon_internal::decode_obj(temp), "_voidPtr")) << '\n';
            } else {
                std::cout << obj << '\n';
            }
        } else if(tachyon_internal::is_func(x)) {
            std::cout << tachyon_internal::decode_func(x) << '\n';
        } else {
            std::cout << x << '\n';
        }
    }));
    
// Tachyon standard library setup function
void tachyon_stl_setup() {
    // tachyon_internal::all_objs.reserve(1000000);
    // Initialize any required standard library components here
    // Initialize any required standard library components here
    tachyon_internal::set_prop(tachyon_internal::decode_obj(String), "toString", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return _args.at(0);
    })));
}