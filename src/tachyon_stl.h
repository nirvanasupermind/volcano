#include <string>
#include <vector>
#include <utility>

namespace tachyon_internal {
    class Val {
    public:
        double num;
        bool is_object;
        Val() {
            this->num = 0.0;
            this->is_object = false;
        }
        Val(double num) {
            this->num = num;
            this->is_object = false;
        }
        // virtual ~Val() = default;
    };

    class Object : public Val {
    public:
        std::vector<std::pair<std::string, Val>> props;
        Val proto;
        void* hidden_data = nullptr;

        Object(const std::vector<std::pair<std::string, Val>>& props = {}, const Val& proto = Val(), void* hidden_data = nullptr) {
            this->num = 0.0;
            this->props = props;
            this->proto = proto;
            this->hidden_data = hidden_data;
            this->is_object = true;
        }

        // ~Object() {
        //     free(hidden_data);
        // }

        Val get(const std::string& key) {
            for (auto& prop : props) {
                if (prop.first == key) {
                    return prop.second;
                }
            }

            if (proto.is_object) {
                return (*(Object**)(&proto))->get(key);
            }

            throw std::runtime_error("key not found: " + key);
        }

        void set(const std::string& key, const Val& val) {
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

            if (proto.is_object) {
                return (*(Object**)(&proto))->has(key);
            }

            return false;
        }
    };

    union ValToObject {
        Val val;
        Object object;
    };


    Object to_object(const Val& x) {
        ValToObject val_to_object = { x };
        return val_to_object.object;
    }

    using func_type = std::function<Val(const std::vector<Val>&)>;
}

tachyon_internal::Val Object = tachyon_internal::Object();
tachyon_internal::Val String = tachyon_internal::Object({}, Object);
tachyon_internal::Val Vector = tachyon_internal::Object({}, Object);
tachyon_internal::Val Map = tachyon_internal::Object({}, Object);
tachyon_internal::Val Function = tachyon_internal::Object({}, Object);