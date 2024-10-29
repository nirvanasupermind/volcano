#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace tachyon_internal {
    using func_type = std::function<double(const std::vector<double>&)>;
    class Object {
    public:
        std::unordered_map<std::string, double> props;
        Object* proto;
        void* hidden_data = nullptr;

        Object(const std::unordered_map<std::string, double>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
            this->props = props;
            this->proto = proto;
            this->hidden_data = hidden_data;
        }

        ~Object() {
            if (proto != nullptr) {
                free(proto);
                proto = nullptr;
            }
            if (hidden_data != nullptr) {
                free(hidden_data);
                hidden_data = nullptr;
            }
        }

        double get(const std::string& key) {
            for (auto& prop : props) {
                if (prop.first == key) {
                    return prop.second;
                }
            }

            if (proto != nullptr) {
                return proto->get(key);
            }

            throw std::runtime_error("key not found: " + key);
        }

        void set(const std::string& key, double val) {
            props[key] = val;
        }

        bool has(const std::string& key) {
            for (auto& prop : props) {
                if (prop.first == key) {
                    return true;
                }
            }

            if (proto != nullptr) {
                return proto->has(key);
            }

            return false;
        }
    };

    std::vector<Object*> all_objs;
    std::vector<func_type*> all_funcs;

    double make_obj(const std::unordered_map<std::string, double>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
        // NaN-boxing
        // Object/function pointers are stored by packing the pointer address into the unused bits of a floating-point NaN value
        Object* obj = new Object(props, proto, hidden_data);
        all_objs.push_back(obj);
        uint64_t u = (uint64_t)obj | 0x7ffc000000000000;
        return *(double*)(&u);
    }

    double make_func(func_type* func) {
        all_funcs.push_back(func);
        uint64_t u = (uint64_t)func | 0x7ffc000000000000;
        return *(double*)(&func);
    }    

    Object* decode_obj(double d) {
        uint64_t pointer_addr = (*(uint64_t*)(&d)) & 0x1ffffffffffff;
        return (Object*)pointer_addr;
    }

    func_type* decode_func(double d) {
        uint64_t pointer_addr = (*(uint64_t*)(&d)) & 0x1ffffffffffff;
        return (func_type*)pointer_addr;
    }

    void free_all() {
        for(int i = 0; i < all_objs.size(); i++) {
            free(all_objs.at(i));
        }

        for(int i = 0; i < all_funcs.size(); i++) {
            free(all_funcs.at(i));
        }
    }

}

double Object = tachyon_internal::make_obj();

void tachyon_stl_setup() {
}