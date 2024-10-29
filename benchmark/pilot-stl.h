#include <list>
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

// Namespace to encapsulate internals for Tachyon
namespace tachyon_internal {
    std::vector<TACHYON_OBJ*> all_objs;
    std::vector<TACHYON_FUNC*> all_funcs;
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

    inline TACHYON_OBJ* decode_obj(double d) {
        uint64_t ptr = reinterpret_cast<uint64_t&>(d) & PTR_MASK;
        return reinterpret_cast<TACHYON_OBJ*>(ptr);
    }

    inline TACHYON_FUNC* decode_func(double d) {
        uint64_t ptr = reinterpret_cast<uint64_t&>(d) & PTR_MASK;
        return reinterpret_cast<TACHYON_FUNC*>(ptr);
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

    inline void set_prop(TACHYON_OBJ* obj, const std::string& key, double val) {
        for (size_t i = 0; i < obj->size(); ++i) {
            if ((*obj)[i].first == key) {
                (*obj)[i].second = val;
                return;
            }
        }
        obj->emplace_back(key, val);
    }
}

// Tachyon standard library setup function
void tachyon_stl_setup() {
    // tachyon_internal::all_objs.reserve(1000000);
    // Initialize any required standard library components here
}