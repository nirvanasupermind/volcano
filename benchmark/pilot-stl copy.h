#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cstdint>
#define TACHYON_OBJ std::unordered_map<std::string, double>
#define TACHYON_FUNC std::function<double(const std::vector<double>&)>
namespace tachyon_internal {
    std::vector<TACHYON_OBJ*> all_objs;
    std::vector<TACHYON_FUNC*> all_funcs;

    double make_obj(TACHYON_OBJ* obj) {
        all_objs.push_back(obj);

        uint64_t u = ((uint64_t)obj) | 0x7ffc000000000000;
        return *(double*)(&u);
    }

    double make_func(TACHYON_FUNC* func) {
        all_funcs.push_back(func);

        uint64_t u = ((uint64_t)func) | 0x7ffe000000000000;
        return *(double*)(&u);
    }

    TACHYON_OBJ* decode_obj(double d) {
        uint64_t pointer_addr = (*(uint64_t*)(&d)) & 0x1ffffffffffff;
        return (TACHYON_OBJ*)(pointer_addr);
    }

    TACHYON_FUNC* decode_func(double d) {
        uint64_t pointer_addr = (*(uint64_t*)(&d)) & 0x1ffffffffffff;
        return (TACHYON_FUNC*)(pointer_addr);
    }

    void free_all() {
        for (TACHYON_OBJ* obj : all_objs) {
            free(obj);
        }
        all_objs.clear();

        for (TACHYON_FUNC* func : all_funcs) {
            free(func);
        }
        all_funcs.clear();
    }

     double get_prop(TACHYON_OBJ* obj, const std::string& key) {
        auto it = obj->find(key);
        if (it != obj->end()) {
            return it->second;
        } else {
            return get_prop(decode_obj(obj->at("proto")), key);
        }
    }

    void set_prop(TACHYON_OBJ* obj, const std::string& key, double val) {
        (*obj)[key] = val;
    }
}

void tachyon_stl_setup() {
    // Standard library setup for Tachyon language (customize as needed)
}