#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include <complex>
#include <thread>

using func_ptr = std::function<uint64_t(const std::vector<uint64_t>&)>;

uint64_t pack_number(float x) {
    uint64_t temp = 0ULL;
    std::memcpy(&temp, &x, sizeof(x));
    return (temp << 2) + 1;
}

float unpack_number(uint64_t x) {
    uint64_t y = x >> 2;
    float temp;
    std::memcpy(&temp, &y, sizeof(temp));
    return temp;
}

class TachyonObject {
public:
    std::unordered_map<std::string, uint64_t>* props;
    void* other_data = nullptr;
    TachyonObject(std::unordered_map<std::string, uint64_t>* props) {
        this->props = props;
    }
    TachyonObject(std::unordered_map<std::string, uint64_t>* props, void* other_data) {
        this->props = props;
        this->other_data = other_data;
    }
    ~TachyonObject() {
        free(props);
        free(other_data);
    }
    uint64_t get(const std::string& key) {
        if (props->count(key)) {
            return props->at(key);
        }
        else {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->get(key);
        }
    }
    void set(const std::string& key, uint64_t val) {
        (*props)[key] = val;
    }
    bool has(const std::string& key) {
        if (props->count(key)) {
            return true;
        }
        else if (props->count("prototype")) {
            uint64_t prototype = props->at("prototype");
            return (*(TachyonObject**)(&prototype))->has(key);
        }
        else {
            return false;
        }
    }
};

std::vector<TachyonObject*> all_objects;

TachyonObject* create_object(std::unordered_map<std::string, uint64_t>* props) {
    TachyonObject* result = new TachyonObject(props);
    all_objects.push_back(result);
    return result;
}

TachyonObject* create_object(std::unordered_map<std::string, uint64_t>* props, void* other_data) {
    TachyonObject* result = new TachyonObject(props, other_data);
    all_objects.push_back(result);
    return result;
}


void free_all_objects() {
    for(int i = 0; i < all_objects.size(); i++) {
        free(all_objects.at(i));
    }
}

uint64_t pack_object(TachyonObject* x) {
    return *(uint64_t*)(&x);
}

TachyonObject* unpack_object(uint64_t x) {
    return *(TachyonObject**)(&x);
}

uint64_t Double = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Math = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t String = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Vector = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Map = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Function = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t ThreadID = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Thread = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t ThisThread = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));
uint64_t Error = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({})));

void tachyon_stl_setup() {
    unpack_object(Math)->set("cos", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cos(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("sin", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sin(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("tan", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tan(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("acos", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::acos(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("asin", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::asin(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atan", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atan(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atan2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atan2(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("cosh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cosh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("sinh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sinh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("tanh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tanh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("acosh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::acosh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("asinh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::asinh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("atanh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::atanh(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("exp", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::exp(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log10", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log10(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("exp2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::exp2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("expm1", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::expm1(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log1p", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log1p(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("pow", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::pow(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("sqrt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::sqrt(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("cbrt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::cbrt(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("hypot", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::hypot(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("erf", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::erf(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("erfc", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::erfc(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("gamma", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::tgamma(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("lgamma", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::lgamma(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("ceil", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::ceil(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("floor", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::floor(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("trunc", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::trunc(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("round", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::round(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("log2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::log2(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("max", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::max(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("min", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::min(unpack_number(_args.at(1)), unpack_number(_args.at(2))));
            }))));

    unpack_object(Math)->set("abs", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::abs(unpack_number(_args.at(1))));
            }))));

    unpack_object(Math)->set("random", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::random_device rd;  // Seed
            std::mt19937 gen(rd());  // Mersenne Twister generator
            std::uniform_real_distribution<float> dis(0.0, 1.0); // Uniform distribution between 0 and 1

            return pack_number(dis(gen)); // Generate random float
            }))));


    unpack_object(Math)->set("PI", 4314120045ULL);

    unpack_object(Math)->set("TAU", 4347674477ULL);

    unpack_object(Math)->set("E", 4307018065ULL);

    unpack_object(Math)->set("INFINITY", pack_number(INFINITY));

    unpack_object(Math)->set("NAN", pack_number(NAN));

    unpack_object(String)->set("at", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.at(index_float))));
            }))));

    unpack_object(String)->set("set", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);
            std::string val_str = *(std::string*)(unpack_object(val)->other_data);

            (*self_str)[(int)index_float] = val_str.at(0);

            return 6ULL;
            }))));

    unpack_object(String)->set("front", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.front())));
            }))));

    unpack_object(String)->set("back", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(1, self_str.back())));
            }))));

    unpack_object(String)->set("empty", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.empty());
            }))));

    unpack_object(String)->set("size", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.size());
            }))));

    unpack_object(String)->set("empty", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);

            return pack_number(self_str.empty());
            }))));

    unpack_object(String)->set("clear", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->clear();

            return 6ULL;
            }))));

    unpack_object(String)->set("insert", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t str = _args.at(2);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            self_str->insert(unpack_number(index), str_str);

            return 6ULL;
            }))));

    unpack_object(String)->set("erase", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t position = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->erase(unpack_number(position));

            return 6ULL;
            }))));

    unpack_object(String)->set("pushBack", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t chr = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            std::string chr_str = *(std::string*)(unpack_object(chr)->other_data);
            self_str->push_back(chr_str.at(0));
            return 6ULL;
            }))));

    unpack_object(String)->set("popBack", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            self_str->pop_back();
            return 6ULL;
            }))));

    unpack_object(String)->set("replace", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t first = _args.at(1);
            uint64_t last = _args.at(2);
            uint64_t str = _args.at(3);

            std::string* self_str = (std::string*)(unpack_object(self)->other_data);
            float first_float = unpack_number(first);
            float last_float = unpack_number(last);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);

            self_str->replace(self_str->begin() + first_float, self_str->begin() + last_float, str_str);
            return 6ULL;
            }))));

    unpack_object(String)->set("find", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.find(str_str));
            }))));

    unpack_object(String)->set("rfind", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.rfind(str_str));
            }))));

    unpack_object(String)->set("compare", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.compare(str_str));
            }))));

    unpack_object(String)->set("contains", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_number(self_str.find(str_str) != std::string::npos);
            }))));

    unpack_object(String)->set("substr", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t count = _args.at(2);

            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            float pos_float = unpack_number(pos);
            float count_float = unpack_number(count);

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str.substr(pos_float, count_float))));
            }))));

    unpack_object(String)->set("concat", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            std::string str_str = *(std::string*)(unpack_object(str)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str + str_str)));
            }))));


    unpack_object(String)->set("split", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->other_data);
            uint64_t delimiter = _args.at(1);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->other_data);
            std::vector<uint64_t> tokens;
            size_t pos = 0;
            std::string token;
            while ((pos = self_str.find(delimiter_str)) != std::string::npos) {
                token = self_str.substr(0, pos);
                tokens.push_back(pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(token))));
                self_str.erase(0, pos + delimiter_str.size());
            }
            tokens.push_back(pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(self_str))));

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(tokens)));
            }))));

    unpack_object(String)->set("toString", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return _args.at(0);
            }))));

    unpack_object(String)->set("from", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t x = _args.at(1);
            if ((x & 3) == 0) {
                TachyonObject* obj = unpack_object(x);
                if (obj->has("toString")) {
                    return (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({ x });
                }
                else {
                    std::ostringstream oss;
                    oss << obj;
                    return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
                }
            }
            else if (x == 2ULL) {
                return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("false")));
            }
            else if (x == 10ULL) {
                return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("true")));
            }
            else if (x == 6ULL) {
                return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string("null")));
            }
            else {
                std::ostringstream oss;
                oss << unpack_number(x);
                return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
            }
            return 6ULL;
            }))));

    unpack_object(Vector)->set("at", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);

            return self_vec.at(index_float);
            }))));

    unpack_object(Vector)->set("set", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);
            self_vec[index_float] = val;
            return 6ULL;
            }))));

    unpack_object(Vector)->set("front", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return self_vec.front();
            }))));

    unpack_object(Vector)->set("back", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return self_vec.back();
            }))));

    unpack_object(Vector)->set("empty", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return pack_number(self_vec.empty());
            }))));

    unpack_object(Vector)->set("size", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            return pack_number(self_vec.size());
            }))));

    unpack_object(Vector)->set("clear", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->clear();
            return 6ULL;
            }))));

    unpack_object(Vector)->set("insert", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t val = _args.at(2);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float pos_float = unpack_number(pos);
            self_vec->insert(self_vec->begin() + pos_float, val);
            return 6ULL;
            }))));

    unpack_object(Vector)->set("pushBack", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->push_back(val);
            ;return 6ULL;
            }))));

    unpack_object(Vector)->set("popBack", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            self_vec->pop_back();
            return 6ULL;
            }))));

    unpack_object(Vector)->set("concat", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            std::vector<uint64_t> result = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::vector<uint64_t> other_vec = *(std::vector<uint64_t>*)(unpack_object(other)->other_data);

            result.insert(result.end(), other_vec.begin(), other_vec.end());
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(result)));
            }))));

    unpack_object(Vector)->set("subvec", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t count = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float pos_float = unpack_number(pos);
            float count_float = unpack_number(count);

            std::vector<uint64_t> subvec(self_vec.begin() + (int)pos_float,
                           self_vec.begin() + (int)pos_float + (int)count_float);
            
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(subvec))); 
            }))));

    unpack_object(Vector)->set("join", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t delimiter = _args.at(1);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->other_data);
            std::string result = "";
            for (int i = 0; i < self_vec.size(); i++) {
                uint64_t temp = ((*(func_ptr*)(unpack_object(unpack_object(String)->get("from"))->other_data))({ String,self_vec.at(i) }));
                result += *(std::string*)(unpack_object(temp)->other_data);
                if (i != self_vec.size() - 1) {
                    result += delimiter_str;
                }
            }
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
            }))));

    unpack_object(Vector)->set("transform", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t func = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::vector<uint64_t> result(self_vec.size());
            std::transform(self_vec.begin(), self_vec.end(), result.begin(), [=] (uint64_t x) -> uint64_t {
                return (*(func_ptr*)(unpack_object(func)->other_data))({x});
            });
        
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Vector} }), new std::vector<uint64_t>(result))); 
            }))));


    unpack_object(Vector)->set("accumulate", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t initial_val = _args.at(1);
            uint64_t func = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);

            std::vector<uint64_t> result(self_vec.size());
            return std::accumulate(self_vec.begin(), self_vec.end(), initial_val, [=] (uint64_t x, uint64_t y) -> uint64_t {
                return (*(func_ptr*)(unpack_object(func)->other_data))({x, y});
            });
            }))));


    unpack_object(Vector)->set("reverse", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->other_data);
            std::reverse(self_vec->begin(), self_vec->end());
            return 6ULL;
            }))));
    
    unpack_object(Vector)->set("toString", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::vector<uint64_t> vec = *(std::vector<uint64_t>*)(unpack_object(_args.at(0))->other_data);
            std::string result = "[";
            if (vec.size() == 0) {
                result = "[]";
            }
            else {
                for (int i = 0; i < vec.size(); i++) {
                    uint64_t temp = ((*(func_ptr*)(unpack_object(unpack_object(String)->get("from"))->other_data))({ String,vec.at(i) }));
                    result += *(std::string*)(unpack_object(temp)->other_data);
                    if (i == vec.size() - 1) {
                        result += "]";
                    }
                    else {
                        result += ",";
                    }
                }
            }

            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
            }))));

    unpack_object(Map)->set("createEmpty", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Map} }), new std::unordered_map<uint64_t, uint64_t>({})));
            }))));

    unpack_object(Map)->set("at", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->other_data);
            float index_float = unpack_number(index);

            return self_vec.at(index_float);
            }))));

    unpack_object(Thread)->set("spawn", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t func = _args.at(1);
            func_ptr thread_func = *(func_ptr*)(unpack_object(func)->other_data);
            return pack_object(create_object(new
                std::unordered_map<std::string, uint64_t>({ {"prototype",Thread} }), new std::thread(thread_func, std::vector<uint64_t>{})));
            }))));

    unpack_object(Thread)->set("joinable", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            return (self_thread->joinable() ? 10ULL : 2ULL);
            }))));

    unpack_object(Thread)->set("getID", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            std::thread::id result = self_thread->get_id();
            return pack_object(create_object(new
                std::unordered_map<std::string, uint64_t>({ {"prototype",ThreadID} }), new std::thread::id(result)));
            }))));
            
    unpack_object(Thread)->set("hardwareConcurrency", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_number(std::thread::hardware_concurrency());
            }))));

    unpack_object(Thread)->set("join", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            self_thread->join();
            return 6ULL;
            }))));
            
    unpack_object(Thread)->set("detach", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->other_data);
            self_thread->detach();
            return 6ULL;
            }))));
            
    unpack_object(ThisThread)->set("yield", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::this_thread::yield();
            return 6ULL;
            }))));
            
    unpack_object(ThisThread)->set("getID", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_object(create_object(new
                std::unordered_map<std::string, uint64_t>({ {"prototype",ThreadID} }), new std::thread::id(std::this_thread::get_id())));
            }))));

    unpack_object(ThisThread)->set("sleepFor", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t ms = _args.at(1);
            std::chrono::milliseconds duration{(int)(unpack_number(ms))};
            std::this_thread::sleep_for(duration);
            return 6ULL;
            }))));
        
    unpack_object(ThisThread)->set("sleepUntil", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t unix_timestamp = _args.at(1);
            std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(unpack_number(unix_timestamp)));
            return 6ULL;
            }))));   
            
    unpack_object(Error)->set("fromMsg", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t msg = _args.at(1);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Error}, { "msg", msg } })));
            }))));

    unpack_object(Error)->set("throw", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            throw std::runtime_error(*(std::string*)(unpack_object(unpack_object(self)->get("msg"))->other_data));
            return 6ULL;
            }))));


    unpack_object(Double)->set("fromFloat", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t arg = _args.at(1);
            float arg_float = unpack_number(arg);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(arg_float)));
            }))));

    unpack_object(Double)->set("fromString", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t arg = _args.at(1);
            std::string arg_str = *(std::string*)(unpack_object(arg)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::stod(arg_str))));
            }))));

    unpack_object(Double)->set("neg", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(-self_double)));
            }))));
    
    unpack_object(Double)->set("add", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(self_double + other_double)));
            }))));

    unpack_object(Double)->set("sub", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(self_double - other_double)));
            }))));

    unpack_object(Double)->set("mul", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(self_double * other_double)));
            }))));

    unpack_object(Double)->set("div", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(self_double / other_double)));
            }))));

    unpack_object(Double)->set("mod", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::fmod(self_double, other_double))));
            }))));


    unpack_object(Double)->set("eq", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double == other_double) ? 10ULL : 2ULL;
            }))));


    unpack_object(Double)->set("ne", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double != other_double) ? 10ULL : 2ULL;
            }))));

    unpack_object(Double)->set("lt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double < other_double) ? 10ULL : 2ULL;
            }))));

    unpack_object(Double)->set("le", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double <= other_double) ? 10ULL : 2ULL;
            }))));

    unpack_object(Double)->set("gt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double > other_double) ? 10ULL : 2ULL;
            }))));

    unpack_object(Double)->set("ge", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return (self_double >= other_double) ? 10ULL : 2ULL;
            }))));

    unpack_object(Double)->set("cos", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::cos(self_double))));
            }))));

    unpack_object(Double)->set("sin", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::sin(self_double))));
            }))));

    unpack_object(Double)->set("tan", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::tan(self_double))));
            }))));

    unpack_object(Double)->set("acos", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::acos(self_double))));
            }))));

    unpack_object(Double)->set("asin", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::asin(self_double))));
            }))));

    unpack_object(Double)->set("atan", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::atan(self_double))));
            }))));

    unpack_object(Double)->set("atan2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::atan2(self_double, other_double))));
            }))));


    unpack_object(Double)->set("sinh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::sinh(self_double))));
            }))));

    unpack_object(Double)->set("cosh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::cosh(self_double))));
            }))));

    unpack_object(Double)->set("tanh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::tanh(self_double))));
            }))));

    unpack_object(Double)->set("asinh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::asinh(self_double))));
            }))));

    unpack_object(Double)->set("acosh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::acosh(self_double))));
            }))));

    unpack_object(Double)->set("atanh", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::atanh(self_double))));
            }))));

    unpack_object(Double)->set("exp", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::exp(self_double))));
            }))));

    unpack_object(Double)->set("log", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::log(self_double))));
            }))));

    unpack_object(Double)->set("log10", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::log10(self_double))));
            }))));

    unpack_object(Double)->set("exp2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::exp2(self_double))));
            }))));

    unpack_object(Double)->set("expm1", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::expm1(self_double))));
            }))));

    unpack_object(Double)->set("log1p", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::log1p(self_double))));
            }))));


    unpack_object(Double)->set("log2", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::log2(self_double))));
            }))));
    
    unpack_object(Double)->set("pow", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::pow(self_double, other_double))));
            }))));

    unpack_object(Double)->set("sqrt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::sqrt(self_double))));
            }))));

    unpack_object(Double)->set("cbrt", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::cbrt(self_double))));
            }))));

    unpack_object(Double)->set("hypot", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::hypot(self_double, other_double))));
            }))));

    unpack_object(Double)->set("erf", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::erf(self_double))));
            }))));

    unpack_object(Double)->set("erfc", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::erfc(self_double))));
            }))));
    
    unpack_object(Double)->set("gamma", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::tgamma(self_double))));
            }))));

    unpack_object(Double)->set("lgamma", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::lgamma(self_double))));
            }))));

    unpack_object(Double)->set("ceil", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::ceil(self_double))));
            }))));


    unpack_object(Double)->set("floor", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::floor(self_double))));
            }))));


    unpack_object(Double)->set("trunc", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::trunc(self_double))));
            }))));

    unpack_object(Double)->set("round", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::round(self_double))));
            }))));


    unpack_object(Double)->set("max", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::max(self_double, other_double))));
            }))));

    unpack_object(Double)->set("min", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->other_data);
            double other_double = *(double*)(unpack_object(other)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::min(self_double, other_double))));
            }))));
            
    unpack_object(Double)->set("abs", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(std::abs(self_double))));
            }))));

    unpack_object(Double)->set("toFloat", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            return pack_number((float)self_double);
            }))));

    unpack_object(Double)->set("toString", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Function} }),
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->other_data);
            std::ostringstream oss;
            oss << self_double;
            return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
            }))));
    
    unpack_object(Double)->set("PI", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(3.141592653589793))));

    unpack_object(Double)->set("TAU", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(6.283185307179586))));

    unpack_object(Double)->set("E", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(2.718281828459045))));

    unpack_object(Double)->set("INFINITY", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(INFINITY))));

    unpack_object(Double)->set("NAN", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Double} }), new double(NAN))));

    // unpack_object(Complex)->set("fromComponents", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         float real = unpack_number(_args.at(1));
    //         float imag = unpack_number(_args.at(2));
    //         return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",Complex} }), new std::complex<float>(real, imag)));
    //         }))));

    // unpack_object(Complex)->set("real", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(self_complex.real());
    //         }))));

    // unpack_object(Complex)->set("imag", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(self_complex.imag());
    //         }))));


    // unpack_object(Complex)->set("abs", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::abs(self_complex));
    //         }))));


    // unpack_object(Complex)->set("arg", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::arg(self_complex));
    //         }))));


    // unpack_object(Complex)->set("norm", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::norm(self_complex));
    //         }))));

    // unpack_object(Complex)->set("norm", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         return pack_number(std::norm(self_complex));
    //         }))));

    // unpack_object(Complex)->set("toString", pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->other_data);
    //         std::ostringstream oss;
    //         oss << self_complex;
    //         return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(oss.str())));
    //         }))));

}

uint64_t input = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    new func_ptr([](const std::vector<uint64_t>& _args) {
        std::string result;
        std::cin >> result;
        return pack_object(create_object(new std::unordered_map<std::string, uint64_t>({ {"prototype",String} }), new std::string(result)));
        })));

uint64_t print = pack_object(create_object(new std::unordered_map<std::string, uint64_t>({}),
    new func_ptr([](const std::vector<uint64_t>& _args) {
        uint64_t x = _args.at(0);
        if ((x & 3) == 0) {
            TachyonObject* obj = unpack_object(x);
            if (obj->has("toString")) {
                uint64_t str = (*(func_ptr*)(unpack_object(obj->get("toString"))->other_data))({ x });
                std::cout << *(std::string*)(unpack_object(str)->other_data) << '\n';
            }
            else {
                std::cout << obj << '\n';
            }
        }
        else if (x == 2ULL) {
            std::cout << "false" << '\n';
        }
        else if (x == 10ULL) {
            std::cout << "true" << '\n';
        }
        else if (x == 6ULL) {
            std::cout << "null" << '\n';
        }
        else {
            std::cout << unpack_number(x) << '\n';
        }
        return 6ULL;
        })));