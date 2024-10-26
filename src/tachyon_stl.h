#include <string>
#include <vector>
#include <utility>

class TachyonVal {
public:
    double num;
    bool is_object;
    TachyonVal() {
        this->num = 0.0;
        this->is_object = false;
    }
    TachyonVal(double num) {
        this->num = num;
        this->is_object = false;
    }
};

class TachyonObject : public TachyonVal {
public:
    std::vector<std::pair<std::string, TachyonVal>> props;
    TachyonVal proto;
    void* hidden_data = nullptr;


    TachyonObject(const std::vector<std::pair<std::string, TachyonVal>>& props = {}, const TachyonVal& proto = TachyonVal(), void* hidden_data = nullptr) {
        this->num = 0.0;
        this->props = props;
        this->proto = proto;
        this->hidden_data = hidden_data;
        this->is_object = true;
    }

    ~TachyonObject() {
        free(hidden_data);
    }

    TachyonVal get(const std::string& key) {
        for (auto& prop : props) {
            if (prop.first == key) {
                return prop.second;
            }
        }

        if (proto.is_object) {
            return (*(TachyonObject**)(&proto))->get(key);
        }

        throw std::runtime_error("key not found: " + key);
    }

    void set(const std::string& key, const TachyonVal& val) {
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
            return (*(TachyonObject**)(&proto))->has(key);
        }

        return false;
    }
};

TachyonVal Object = TachyonObject();
TachyonVal String = TachyonObject({}, Object);

void tachyon_stl_setup() {
    
}