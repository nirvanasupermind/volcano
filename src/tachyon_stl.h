#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>

namespace tachyon_internal {
    class Object;
    class Val {
    public:
        uint8_t type;
        union {
            float num;
            std::function<Val(const std::vector<Val>&)>* func;
            Object* obj;
        };
        Val() {
            this->type = 0;
            this->num = 0.0f;
        }
        Val(double num) {
            this->type = 0;
            this->num = num;
        }
        Val(std::function<Val(const std::vector<Val>&)>* func) {
            this->type = 1;
            this->func = func;
        }
        Val(Object* obj) {
            this->type = 2;
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

        Val set(const std::string& key, const Val& val) {
            props[key] = val;
            return val;
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

    using func_type = std::function<Val(const std::vector<Val>&)>;

    std::vector<Object*> obj_pool;

    std::vector<func_type*> func_pool;

    Val make_obj(const std::unordered_map<std::string, Val>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
        Object* obj = new Object(props, proto, hidden_data);
        obj_pool.push_back(obj);
        return Val(obj);
    }

    Val make_func(const func_type& func) {
        func_type* func_ptr = new func_type(func);
        func_pool.push_back(func_ptr);
        return Val(func_ptr);
    }

    void free_all() {
        for (int i = 0; i < obj_pool.size(); i++) {
            free(obj_pool.at(i));
            obj_pool.at(i) = nullptr;
        }
    }
}

// Standard library code starts here

tachyon_internal::Val Object = tachyon_internal::make_obj();
tachyon_internal::Val String = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val Vector = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val Map = tachyon_internal::make_obj({}, Object.obj);
// tachyon_internal::Val Function = tachyon_internal::make_obj({}, Object.obj);
tachyon_internal::Val print =  tachyon_internal::make_func([](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
        std::cout << _args.at(0).num << '\n';
    });

void tachyon_stl_setup() {
    Object.obj->set("clone", tachyon_internal::make_func([=](const std::vector<tachyon_internal::Val>& _args) -> tachyon_internal::Val {
        return tachyon_internal::make_obj({}, _args.at(0).obj);
    }));
}
