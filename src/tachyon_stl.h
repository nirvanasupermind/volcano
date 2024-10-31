#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <thread>
#include <utility>

#define TACHYON_OBJ std::vector<std::pair<std::string, double> >
#define TACHYON_FUNC std::function<double(const std::vector<double>&)>
#define STR_TAG 0x7ffc000000000000
#define VEC_TAG 0x7ffd000000000000
#define FUNC_TAG 0x7ffe000000000000
#define OBJ_TAG 0x7fff000000000000
#define PTR_MASK 0x1ffffffffffff
#define TYPECHECK_MASK 0xfffe000000000000

// Namespace to encapsulate internals for Tachyon
namespace tachyon_internal {
    std::vector<void*> all_ptrs;

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

    inline double make_obj(TACHYON_OBJ* obj) {
        all_ptrs.push_back(obj);
        uint64_t u = ((uint64_t)obj) | FUNC_TAG;
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

    inline TACHYON_OBJ* decode_obj(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (TACHYON_OBJ*)(ptr);
    }


    inline void free_all() {
        // for (void* ptr : all_ptrs) {
        //     free(ptr);
        // }
        // all_ptrs.clear();
    }


    inline double get_prop(TACHYON_OBJ* obj, const std::string& key) {
        for (auto& pair : *obj) {
            if (pair.first == key) return pair.second;
        }

        for (auto& pair : *obj) {
            if (pair.first == "proto") return get_prop(decode_obj(pair.second), key);
        }


        throw std::runtime_error("key not found: " + key);
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

    inline bool is_str(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == STR_TAG;
    }

    inline bool is_vec(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == VEC_TAG;
    }

    inline bool is_func(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == FUNC_TAG;
    }

    inline bool is_obj(double d) {
        uint64_t u = reinterpret_cast<uint64_t&>(d);
        return (u & TYPECHECK_MASK) == OBJ_TAG;
    }

    inline double get_subscript(double base, double idx) {
        if(is_str(base))  {
            return make_str(new std::string(1, (*decode_str(base)).at(idx)));
        } else if(is_vec(base)) {
            return (*decode_vec(base)).at(idx);
        } else if(is_obj(base)) {
            return get_prop(decode_obj(base), *decode_str(idx));
        } else {
            throw std::runtime_error("illegal subscript");  
        }
    }


    inline double set_subscript(double base, double idx, double val) {
        if(is_str(base))  {
            (*decode_str(base))[idx] = val;
        } else if(is_vec(base)) {
            (*decode_vec(base))[idx] = val;
        } else if(is_obj(base)) {
            set_prop(decode_obj(base), *decode_str(idx), val);
        } else {
            throw std::runtime_error("illegal subscript");
        }   
        return val;
    }
}

double print = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
        double x = _args.at(0);
        if(tachyon_internal::is_obj(x)) {
            TACHYON_OBJ* obj = tachyon_internal::decode_obj(x);
            if(tachyon_internal::has_prop(obj, "toString")) {
                double temp = (*tachyon_internal::decode_func(tachyon_internal::get_prop(obj, "toString")))({x});
                std::cout << *tachyon_internal::decode_str(temp) << '\n';
            } else {
                std::cout << obj << '\n';
            }
        } else if(tachyon_internal::is_str(x)) {
            std::cout << *tachyon_internal::decode_str(x) << '\n';
        } else if(tachyon_internal::is_vec(x)) {
            std::cout << tachyon_internal::decode_vec(x) << '\n';
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
    // tachyon_internal::set_prop(tachyon_internal::decode_obj(String), "toString", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
    //     return _args.at(0);
    // })));
}