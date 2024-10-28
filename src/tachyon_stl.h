#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

namespace tachyon_internal {
    class Object;
    class Val {
    public:
        bool is_obj;
        union {
            float num;
            Object* obj;
        };
        Val() {
            this->is_obj = false;
            this->num = 0.0f;
        }
        Val(double num) {
            this->is_obj = false;
            this->num = num;
        }
        Val(Object* obj) {
            this->is_obj = true;
            this->obj = obj;
        }
        // virtual ~Val() = default;
    };

    class Object {
    public:
        std::unordered_map<std::string, Val> props;
        Object* proto;
        void* hidden_data = nullptr;

        Object(const std::unordered_map<std::string, Val>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
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

        Val get(const std::string& key) {
            if (props.count(key)) {
                return props.at(key);
            }

            if (proto != nullptr) {
                return proto->get(key);
            }

            throw std::runtime_error("key not found: " + key);
        }

        void set(const std::string& key, Val val) {
            props[key] = val;
        }

        bool has(const std::string& key) {
            if (props.count(key)) {
                return true;
            }

            if (proto != nullptr) {
                return proto->has(key);
            }

            return false;
        }
    };

    std::vector<Object*> obj_pool;

    Val make_obj(const std::unordered_map<std::string, Val>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
        Object* obj = new Object(props, proto, hidden_data);
        obj_pool.push_back(obj);
        return Val(obj);
    }

    void free_all_objs() {
        for (int i = 0; i < obj_pool.size(); i++) {
            free(obj_pool.at(i));
            obj_pool.at(i) = nullptr;
        }
    }

    using func_type = std::function<Val(const std::vector<Val>&)>;
}

tachyon_internal::Val Object = tachyon_internal::make_obj();
tachyon_internal::Val String = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val Vector = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val Map = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val Function = tachyon_internal::make_obj({}, Object.obj);

void tachyon_stl_setup() {
}