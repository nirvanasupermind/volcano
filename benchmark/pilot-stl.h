#include <string>
#include <vector>
#include <utility>

namespace tachyon_internal {
    class Object;
    class Val {
    public:
        bool is_object;
        union {
            float num;
            Object* obj;
        };
        Val() {
            this->is_object = false;
            this->num = 0.0f;
        }
        Val(double num) {
            this->is_object = false;
            this->num = num;
        }
        Val(Object* obj) {
            this->is_object = true;
            this->obj = obj;
        }
        // virtual ~Val() = default;
    };

    class Object {
    public:
        std::vector<std::pair<std::string, Val>> props;
        Object* proto;
        void* hidden_data = nullptr;

        Object(const std::vector<std::pair<std::string, Val>>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
            this->props = props;
            this->proto = proto;
            this->hidden_data = hidden_data;
        }

        ~Object() {
            free(proto);
            free(hidden_data);
        }

        Val get(const std::string& key) {
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

        void set(const std::string& key, Val val) {
            for (auto& prop : props) {
                if (prop.first == key) {
                    prop.second = val;
                    return;
                }
            }
            props.push_back({ key, val });
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

    std::vector<Object*> obj_pool;

    Val make_obj_val(const std::vector<std::pair<std::string, Val>>& props = {}, Object* proto = nullptr, void* hidden_data = nullptr) {
        Object* obj = new Object(props, proto, hidden_data);
        obj_pool.push_back(obj);
        return Val(obj);
    }

    void free_all_objs() {
        for(int i = 0; i < obj_pool.size(); i++) {
            free(obj_pool.at(i));
        }
    }
    

    using func_type = std::function<Val(const std::vector<Val>&)>;
}

tachyon_internal::Val Object = tachyon_internal::Val(new tachyon_internal::Object());