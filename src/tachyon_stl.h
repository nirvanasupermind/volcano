#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <random>
#include <thread>
#include <cstdint>
#include <utility>
#include <unordered_set>

using func_ptr = std::function<uint64_t(const std::vector<uint64_t>&)>;

uint64_t pack_float(float x) {
    return (*(uint64_t*)(&x) << 2) + 1;
}

float unpack_float(uint64_t x) {
    uint64_t temp = x >> 2;
    return *(float*)(&temp);
}

class TachyonObject {
public:
    // Using vector instead of unordered_map for faster access
    std::vector<std::pair<std::string, uint64_t>> props;
    uint64_t proto;
    void* hidden_data = nullptr;

    TachyonObject() = default;
    TachyonObject(const std::vector<std::pair<std::string, uint64_t>>& props, uint64_t proto = 0ULL, void* hidden_data = nullptr)
        : props(props), proto(proto), hidden_data(hidden_data) {
    }

    ~TachyonObject() {
        // Memory management should be optimized depending on your allocator
        free(hidden_data);
    }

    // Optimized get with single prototype lookup and inline caching
    uint64_t get(const std::string& key) {
        // Inline cache check (you can implement caching for commonly used keys)
        for (auto& prop : props) {
            if (prop.first == key) {
                return prop.second;
            }
        }
        
        if (proto) {
            return (*(TachyonObject**)(&proto))->get(key);
        }

        throw std::runtime_error("key not found: " + key);
    }

    void set(const std::string& key, uint64_t val) {
        // Linear search and set, could be improved with a hash if necessary
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

        for (auto& prop : props) {
            if (prop.first == "proto") {
                uint64_t prototype = prop.second;
                return (*(TachyonObject**)(&prototype))->has(key);
            }
        }

        return false;
    }
};

uint64_t pack_object(TachyonObject* x) {
    return *(uint64_t*)(&x);
}

TachyonObject* unpack_object(uint64_t x) {
    return *(TachyonObject**)(&x);
}

std::vector<TachyonObject*> all_objects;

uint64_t create_object(const std::vector<std::pair<std::string, uint64_t> >& props) {
    TachyonObject* result = new TachyonObject(props);
    all_objects.push_back(result);
    return pack_object(result);
}


uint64_t create_object(const std::vector<std::pair<std::string, uint64_t> >& props, uint64_t proto) {
    TachyonObject* result = new TachyonObject(props, proto);
    all_objects.push_back(result);
    return pack_object(result);
}

uint64_t create_object(const std::vector<std::pair<std::string, uint64_t> >& props, uint64_t proto, void* hidden_data) {
    TachyonObject* result = new TachyonObject(props, proto, hidden_data);
    all_objects.push_back(result);
    return pack_object(result);
}

void free_all_objects() {
    for (int i = 0; i < all_objects.size(); i++) {
        free(all_objects.at(i));
    }
}

std::string string_repr(uint64_t x) {
    if ((x & 3) == 0) {
        TachyonObject* obj = unpack_object(x);
        if (obj->has("toString")) {
            return *(std::string*)(unpack_object((*(func_ptr*)(unpack_object(obj->get("toString"))->hidden_data))({ x }))->hidden_data);
        }
        else {
            std::ostringstream oss;
            oss << obj;
            return oss.str();
        }
    }
    else if (x == 2ULL) {
        return "false";
    }
    else if (x == 10ULL) {
        return "true";
    }
    else if (x == 6ULL) {
        return "null";
    }
    else {
        std::ostringstream oss;
        oss << unpack_float(x);
        return oss.str();
    }
}

uint64_t Object = create_object({});
uint64_t Double = create_object({ {"proto", Object} });
uint64_t Math = create_object({ {"proto", Object} });
uint64_t String = create_object({ {"proto", Object} });
uint64_t Vector = create_object({ {"proto", Object} });
uint64_t Set = create_object({ {"proto", Object} });
uint64_t Map = create_object({ {"proto", Object} });
uint64_t Function = create_object({ {"proto", Object} });
// uint64_t Date = create_object({ {"proto", Object} });
uint64_t ThreadID = create_object({ {"proto", Object} });
uint64_t Thread = create_object({ {"proto", Object} });
uint64_t ThisThread = create_object({ {"proto", Object} });
uint64_t Error = create_object({ {"proto", Object} });

void tachyon_stl_setup() {
    unpack_object(Object)->set("clone", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return create_object({}, _args.at(0));
            })));

    unpack_object(Object)->set("getProto", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return unpack_object(_args.at(0))->proto;
            })));


    unpack_object(Object)->set("setProto", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            unpack_object(_args.at(0))->proto = _args.at(1);
            return 6ULL;
            })));

    // unpack_object(Object)->set("getKey", create_object({},Function,
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         return unpack_object(_args.at(0))->get(*(std::string*)(unpack_object(_args.at(1))));
    //         })));

    // unpack_object(Object)->set("setKey", create_object({},Function,
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         unpack_object(_args.at(0))->set(*(std::string*)(unpack_object(_args.at(1))), _args.at(2));
    //         return 6ULL;
    //         })));


    unpack_object(Double)->set("fromFloat", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t arg = _args.at(1);
            float arg_float = unpack_float(arg);
            return create_object({}, Double, new double(arg_float));
            })));

    unpack_object(Double)->set("fromString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t arg = _args.at(1);
            std::string arg_str = *(std::string*)(unpack_object(arg)->hidden_data);
            return create_object({}, Double, new double(std::stod(arg_str)));
            })));

    unpack_object(Double)->set("inc", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double* self_double = (double*)(unpack_object(self)->hidden_data);
            (*self_double)++;
            return 6ULL;
            })));

    unpack_object(Double)->set("neg", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(-self_double));
            })));

    unpack_object(Double)->set("add", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(self_double + other_double));
            })));

    unpack_object(Double)->set("sub", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(self_double - other_double));
            })));

    unpack_object(Double)->set("mul", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(self_double * other_double));
            })));

    unpack_object(Double)->set("div", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(self_double / other_double));
            })));

    unpack_object(Double)->set("mod", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::fmod(self_double, other_double)));
            })));


    unpack_object(Double)->set("eq", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double == other_double) ? 10ULL : 2ULL;
            })));


    unpack_object(Double)->set("ne", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double != other_double) ? 10ULL : 2ULL;
            })));

    unpack_object(Double)->set("lt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double < other_double) ? 10ULL : 2ULL;
            })));

    unpack_object(Double)->set("le", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double <= other_double) ? 10ULL : 2ULL;
            })));

    unpack_object(Double)->set("gt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double > other_double) ? 10ULL : 2ULL;
            })));

    unpack_object(Double)->set("ge", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return (self_double >= other_double) ? 10ULL : 2ULL;
            })));

    unpack_object(Double)->set("cos", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::cos(self_double)));
            })));

    unpack_object(Double)->set("sin", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::sin(self_double)));
            })));

    unpack_object(Double)->set("tan", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::tan(self_double)));
            })));

    unpack_object(Double)->set("acos", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::acos(self_double)));
            })));

    unpack_object(Double)->set("asin", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::asin(self_double)));
            })));

    unpack_object(Double)->set("atan", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::atan(self_double)));
            })));

    unpack_object(Double)->set("atan2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::atan2(self_double, other_double)));
            })));


    unpack_object(Double)->set("sinh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::sinh(self_double)));
            })));

    unpack_object(Double)->set("cosh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::cosh(self_double)));
            })));

    unpack_object(Double)->set("tanh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::tanh(self_double)));
            })));

    unpack_object(Double)->set("asinh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::asinh(self_double)));
            })));

    unpack_object(Double)->set("acosh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::acosh(self_double)));
            })));

    unpack_object(Double)->set("atanh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::atanh(self_double)));
            })));

    unpack_object(Double)->set("exp", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::exp(self_double)));
            })));

    unpack_object(Double)->set("log", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::log(self_double)));
            })));

    unpack_object(Double)->set("log10", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::log10(self_double)));
            })));

    unpack_object(Double)->set("exp2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::exp2(self_double)));
            })));

    unpack_object(Double)->set("expm1", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::expm1(self_double)));
            })));

    unpack_object(Double)->set("log1p", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::log1p(self_double)));
            })));


    unpack_object(Double)->set("log2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::log2(self_double)));
            })));

    unpack_object(Double)->set("pow", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::pow(self_double, other_double)));
            })));

    unpack_object(Double)->set("sqrt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::sqrt(self_double)));
            })));

    unpack_object(Double)->set("cbrt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::cbrt(self_double)));
            })));

    unpack_object(Double)->set("hypot", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::hypot(self_double, other_double)));
            })));

    unpack_object(Double)->set("erf", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::erf(self_double)));
            })));

    unpack_object(Double)->set("erfc", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::erfc(self_double)));
            })));

    unpack_object(Double)->set("gamma", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::tgamma(self_double)));
            })));

    unpack_object(Double)->set("lgamma", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::lgamma(self_double)));
            })));

    unpack_object(Double)->set("ceil", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::ceil(self_double)));
            })));


    unpack_object(Double)->set("floor", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::floor(self_double)));
            })));


    unpack_object(Double)->set("trunc", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::trunc(self_double)));
            })));

    unpack_object(Double)->set("round", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::round(self_double)));
            })));


    unpack_object(Double)->set("max", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::max(self_double, other_double)));
            })));

    unpack_object(Double)->set("min", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            double other_double = *(double*)(unpack_object(other)->hidden_data);
            return create_object({}, Double, new double(std::min(self_double, other_double)));
            })));

    unpack_object(Double)->set("abs", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return create_object({}, Double, new double(std::abs(self_double)));
            })));

    unpack_object(Double)->set("toFloat", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            return pack_float((float)self_double);
            })));

    unpack_object(Double)->set("toString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            double self_double = *(double*)(unpack_object(self)->hidden_data);
            std::ostringstream oss;
            oss << self_double;
            return create_object({}, String, new std::string(oss.str()));
            })));

    unpack_object(Double)->set("PI", create_object({}, Double, new double(3.141592653589793)));

    unpack_object(Double)->set("TAU", create_object({}, Double, new double(6.283185307179586)));

    unpack_object(Double)->set("E", create_object({}, Double, new double(2.718281828459045)));

    unpack_object(Double)->set("INFINITY", create_object({}, Double, new double(INFINITY)));

    unpack_object(Double)->set("NAN", create_object({}, Double, new double(NAN)));

    unpack_object(Math)->set("cos", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::cos(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("sin", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::sin(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("tan", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::tan(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("acos", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::acos(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("asin", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::asin(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("atan", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::atan(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("atan2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::atan2(unpack_float(_args.at(1)), unpack_float(_args.at(2))));
            })));

    unpack_object(Math)->set("cosh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::cosh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("sinh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::sinh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("tanh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::tanh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("acosh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::acosh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("asinh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::asinh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("atanh", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::atanh(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("exp", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::exp(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("log", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::log(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("log10", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::log10(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("exp2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::exp2(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("expm1", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::expm1(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("log1p", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::log1p(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("log2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::log2(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("pow", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::pow(unpack_float(_args.at(1)), unpack_float(_args.at(2))));
            })));

    unpack_object(Math)->set("sqrt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::sqrt(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("cbrt", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::cbrt(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("hypot", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::hypot(unpack_float(_args.at(1)), unpack_float(_args.at(2))));
            })));

    unpack_object(Math)->set("erf", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::erf(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("erfc", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::erfc(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("gamma", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::tgamma(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("lgamma", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::lgamma(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("ceil", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::ceil(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("floor", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::floor(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("trunc", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::trunc(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("round", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::round(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("log2", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::log2(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("max", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::max(unpack_float(_args.at(1)), unpack_float(_args.at(2))));
            })));

    unpack_object(Math)->set("min", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::min(unpack_float(_args.at(1)), unpack_float(_args.at(2))));
            })));

    unpack_object(Math)->set("abs", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::abs(unpack_float(_args.at(1))));
            })));

    unpack_object(Math)->set("random", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::random_device rd;  // Seed
            std::mt19937 gen(rd());  // Mersenne Twister generator
            std::uniform_real_distribution<float> dis(0.0, 1.0); // Uniform distribution between 0 and 1

            return pack_float(dis(gen)); // Generate random float
            })));


    unpack_object(Math)->set("PI", 4314120045ULL);

    unpack_object(Math)->set("TAU", 4347674477ULL);

    unpack_object(Math)->set("E", 4307018065ULL);

    unpack_object(Math)->set("INFINITY", pack_float(INFINITY));

    unpack_object(Math)->set("NAN", pack_float(NAN));

    unpack_object(String)->set("at", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            float index_float = unpack_float(index);

            return create_object({}, String, new std::string(1, self_str.at(index_float)));
            })));

    unpack_object(String)->set("set", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            float index_float = unpack_float(index);
            std::string val_str = *(std::string*)(unpack_object(val)->hidden_data);

            (*self_str)[(int)index_float] = val_str.at(0);

            return 6ULL;
            })));

    unpack_object(String)->set("front", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);

            return create_object({}, String, new std::string(1, self_str.front()));
            })));

    unpack_object(String)->set("back", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);

            return create_object({}, String, new std::string(1, self_str.back()));
            })));

    unpack_object(String)->set("empty", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);

            return pack_float(self_str.empty());
            })));

    unpack_object(String)->set("size", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);

            return pack_float(self_str.size());
            })));

    unpack_object(String)->set("empty", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);

            return pack_float(self_str.empty());
            })));

    unpack_object(String)->set("clear", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            self_str->clear();

            return 6ULL;
            })));

    unpack_object(String)->set("insert", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t str = _args.at(2);
            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            self_str->insert(unpack_float(index), str_str);

            return 6ULL;
            })));

    unpack_object(String)->set("erase", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t position = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            self_str->erase(unpack_float(position));

            return 6ULL;
            })));

    unpack_object(String)->set("pushBack", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t chr = _args.at(1);
            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            std::string chr_str = *(std::string*)(unpack_object(chr)->hidden_data);
            self_str->push_back(chr_str.at(0));
            return 6ULL;
            })));

    unpack_object(String)->set("popBack", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            self_str->pop_back();
            return 6ULL;
            })));

    unpack_object(String)->set("replace", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t first = _args.at(1);
            uint64_t last = _args.at(2);
            uint64_t str = _args.at(3);

            std::string* self_str = (std::string*)(unpack_object(self)->hidden_data);
            float first_float = unpack_float(first);
            float last_float = unpack_float(last);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);

            self_str->replace(self_str->begin() + first_float, self_str->begin() + last_float, str_str);
            return 6ULL;
            })));

    unpack_object(String)->set("find", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            return pack_float(self_str.find(str_str));
            })));

    unpack_object(String)->set("rfind", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            return pack_float(self_str.rfind(str_str));
            })));

    unpack_object(String)->set("compare", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            return pack_float(self_str.compare(str_str));
            })));

    unpack_object(String)->set("contains", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            return pack_float(self_str.find(str_str) != std::string::npos);
            })));

    unpack_object(String)->set("substr", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t count = _args.at(2);

            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            float pos_float = unpack_float(pos);
            float count_float = unpack_float(count);

            return create_object({}, String, new std::string(self_str.substr(pos_float, count_float)));
            })));

    unpack_object(String)->set("concat", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t str = _args.at(1);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            std::string str_str = *(std::string*)(unpack_object(str)->hidden_data);
            return create_object({}, String, new std::string(self_str + str_str));
            })));


    unpack_object(String)->set("split", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::string self_str = *(std::string*)(unpack_object(self)->hidden_data);
            uint64_t delimiter = _args.at(1);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->hidden_data);
            std::vector<uint64_t> tokens;
            size_t pos = 0;
            std::string token;
            while ((pos = self_str.find(delimiter_str)) != std::string::npos) {
                token = self_str.substr(0, pos);
                tokens.push_back(create_object({}, String, new std::string(token)));
                self_str.erase(0, pos + delimiter_str.size());
            }
            tokens.push_back(create_object({}, String, new std::string(self_str)));

            return create_object({}, Vector, new std::vector<uint64_t>(tokens));
            })));

    unpack_object(String)->set("toString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return _args.at(0);
            })));

    unpack_object(String)->set("from", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return create_object({}, String, new std::string(string_repr(_args.at(1))));
            })));

    unpack_object(Vector)->set("at", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            float index_float = unpack_float(index);

            return self_vec.at(index_float);
            })));

    unpack_object(Vector)->set("set", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t index = _args.at(1);
            uint64_t val = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            float index_float = unpack_float(index);
            self_vec[index_float] = val;
            return 6ULL;
            })));

    unpack_object(Vector)->set("front", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);

            return self_vec.front();
            })));

    unpack_object(Vector)->set("back", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);

            return self_vec.back();
            })));

    unpack_object(Vector)->set("empty", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);

            return pack_float(self_vec.empty());
            })));

    unpack_object(Vector)->set("size", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);

            return pack_float(self_vec.size());
            })));

    unpack_object(Vector)->set("clear", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            self_vec->clear();
            return 6ULL;
            })));

    unpack_object(Vector)->set("insert", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t val = _args.at(2);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            float pos_float = unpack_float(pos);
            self_vec->insert(self_vec->begin() + pos_float, val);
            return 6ULL;
            })));

    unpack_object(Vector)->set("pushBack", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            self_vec->push_back(val);
            return 6ULL;
            })));

    unpack_object(Vector)->set("popBack", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t val = _args.at(1);
            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            self_vec->pop_back();
            return 6ULL;
            })));

    unpack_object(Vector)->set("concat", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t other = _args.at(1);
            std::vector<uint64_t> result = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            std::vector<uint64_t> other_vec = *(std::vector<uint64_t>*)(unpack_object(other)->hidden_data);

            result.insert(result.end(), other_vec.begin(), other_vec.end());
            return create_object({}, Vector, new std::vector<uint64_t>(result));
            })));

    unpack_object(Vector)->set("subvec", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t pos = _args.at(1);
            uint64_t count = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            float pos_float = unpack_float(pos);
            float count_float = unpack_float(count);

            std::vector<uint64_t> subvec(self_vec.begin() + (int)pos_float,
                self_vec.begin() + (int)pos_float + (int)count_float);

            return create_object({}, Vector, new std::vector<uint64_t>(subvec));
            })));

    unpack_object(Vector)->set("join", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t delimiter = _args.at(1);
            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            std::string delimiter_str = *(std::string*)(unpack_object(delimiter)->hidden_data);
            std::string result = "";
            for (int i = 0; i < self_vec.size(); i++) {
                result += string_repr(self_vec.at(i));
                if (i != self_vec.size() - 1) {
                    result += delimiter_str;
                }
            }
            return create_object({}, String, new std::string(result));
            })));

    unpack_object(Vector)->set("sort", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);

            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            std::sort(self_vec->begin(), self_vec->end());

            return 6ULL;
            })));

    unpack_object(Vector)->set("transform", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t func = _args.at(1);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            std::vector<uint64_t> result(self_vec.size());
            std::transform(self_vec.begin(), self_vec.end(), result.begin(), [=](uint64_t x) -> uint64_t {
                return (*(func_ptr*)(unpack_object(func)->hidden_data))({ x });
                });

            return create_object({}, Vector, new std::vector<uint64_t>(result));
            })));

    unpack_object(Vector)->set("accumulate", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t initial_val = _args.at(1);
            uint64_t func = _args.at(2);

            std::vector<uint64_t> self_vec = *(std::vector<uint64_t>*)(unpack_object(self)->hidden_data);

            std::vector<uint64_t> result(self_vec.size());
            return std::accumulate(self_vec.begin(), self_vec.end(), initial_val, [=](uint64_t x, uint64_t y) -> uint64_t {
                return (*(func_ptr*)(unpack_object(func)->hidden_data))({ x, y });
                });
            })));


    unpack_object(Vector)->set("reverse", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);

            std::vector<uint64_t>* self_vec = (std::vector<uint64_t>*)(unpack_object(self)->hidden_data);
            std::reverse(self_vec->begin(), self_vec->end());
            return 6ULL;
            })));

    unpack_object(Vector)->set("toString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::vector<uint64_t> vec = *(std::vector<uint64_t>*)(unpack_object(_args.at(0))->hidden_data);
            std::string result = "[";
            if (vec.size() == 0) {
                result = "[]";
            }
            else {
                for (int i = 0; i < vec.size(); i++) {
                    result += string_repr(vec.at(i));
                    if (i == vec.size() - 1) {
                        result += "]";
                    }
                    else {
                        result += ",";
                    }
                }
            }

            return create_object({}, String, new std::string(result));
            })));

    unpack_object(Set)->set("size", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::unordered_set<uint64_t>* self_unordered_set = (std::unordered_set<uint64_t>*)(unpack_object(self)->hidden_data);
            return pack_float(self_unordered_set->size());
            })));

    unpack_object(Map)->set("size", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::unordered_map<std::string, uint64_t>* self_unordered_map = (std::unordered_map<std::string, uint64_t>*)(unpack_object(self)->hidden_data);
            return pack_float(self_unordered_map->size());
            })));


    unpack_object(Map)->set("at", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t key = _args.at(1);
            std::unordered_map<std::string, uint64_t>* self_unordered_map = (std::unordered_map<std::string, uint64_t>*)(unpack_object(self)->hidden_data);
            return self_unordered_map->at(*(std::string*)(unpack_object(key)->hidden_data));
            })));

    unpack_object(Map)->set("set", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t key = _args.at(1);
            uint64_t val = _args.at(2);
            std::unordered_map<std::string, uint64_t>* self_unordered_map = (std::unordered_map<std::string, uint64_t>*)(unpack_object(self)->hidden_data);
            (*self_unordered_map)[*(std::string*)(unpack_object(key)->hidden_data)] = val;
            return 6ULL;
            })));

    unpack_object(Map)->set("has", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            uint64_t key = _args.at(1);
            std::unordered_map<std::string, uint64_t> self_unordered_map = *(std::unordered_map<std::string, uint64_t>*)(unpack_object(self)->hidden_data);
            return (self_unordered_map.count(string_repr(key)) ? 10ULL : 2ULL);
            })));

    unpack_object(Map)->set("toString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::unordered_map<std::string, uint64_t> self_unordered_map = *(std::unordered_map<std::string, uint64_t>*)(unpack_object(self)->hidden_data);
            std::string result = "{";
            for (auto kv : self_unordered_map) {
                result += kv.first + ":" + string_repr(kv.second) + ",";
            }
            if (result != "{") {
                result = result.substr(0, result.size() - 1);
            }

            result += "}";


            return create_object({}, String, new std::string(result));
            })));

    unpack_object(ThreadID)->set("toString", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread::id self_thread_id = *(std::thread::id*)(unpack_object(self)->hidden_data);
            std::ostringstream oss;
            oss << self_thread_id;
            return create_object({}, String, new std::string(oss.str()));
            })));


    unpack_object(Thread)->set("fromFunction", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t func = _args.at(1);
            func_ptr thread_func = *(func_ptr*)(unpack_object(func)->hidden_data);
            return create_object({}, Thread, new std::thread(thread_func, std::vector<uint64_t>{}));
            })));

    unpack_object(Thread)->set("joinable", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->hidden_data);
            return (self_thread->joinable() ? 10ULL : 2ULL);
            })));

    unpack_object(Thread)->set("getID", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->hidden_data);
            std::thread::id result = self_thread->get_id();
            return create_object({}, ThreadID, new std::thread::id(result));
            })));

    unpack_object(Thread)->set("hardwareConcurrency", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return pack_float(std::thread::hardware_concurrency());
            })));

    unpack_object(Thread)->set("join", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->hidden_data);
            self_thread->join();
            return 6ULL;
            })));

    unpack_object(Thread)->set("detach", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            std::thread* self_thread = (std::thread*)(unpack_object(self)->hidden_data);
            self_thread->detach();
            return 6ULL;
            })));

    unpack_object(ThisThread)->set("yield", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            std::this_thread::yield();
            return 6ULL;
            })));

    unpack_object(ThisThread)->set("getID", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            return create_object({}, ThreadID, new std::thread::id(std::this_thread::get_id()));
            })));

    unpack_object(ThisThread)->set("sleepFor", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t ms = _args.at(1);
            std::chrono::milliseconds duration{ (int)(unpack_float(ms)) };
            std::this_thread::sleep_for(duration);
            return 6ULL;
            })));

    unpack_object(ThisThread)->set("sleepUntil", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            double unix_timestamp = *(double*)(unpack_object(_args.at(1))->hidden_data);
            std::this_thread::sleep_until(std::chrono::system_clock::from_time_t(unix_timestamp));
            return 6ULL;
            })));

    unpack_object(Error)->set("fromMsg", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t msg = _args.at(1);
            return create_object({ { "msg", msg } }, Error);
            })));

    unpack_object(Error)->set("throw", create_object({}, Function,
        new func_ptr([](const std::vector<uint64_t>& _args) {
            uint64_t self = _args.at(0);
            throw std::runtime_error(*(std::string*)(unpack_object(unpack_object(self)->get("msg"))->hidden_data));
            return 6ULL;
            })));

    // unpack_object(Map)->set("createEmpty", create_object({},Function,
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         return create_object({{"proto",Map}}, {});
    //         })));



    // unpack_object(Complex)->set("fromComponents", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         float real = unpack_float(_args.at(1));
    //         float imag = unpack_float(_args.at(2));
    //         return create_object({},Complex, new std::complex<float>(real, imag));
    //         })));

    // unpack_object(Complex)->set("real", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(self_complex.real());
    //         })));

    // unpack_object(Complex)->set("imag", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(self_complex.imag());
    //         })));


    // unpack_object(Complex)->set("abs", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(std::abs(self_complex));
    //         })));


    // unpack_object(Complex)->set("arg", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(std::arg(self_complex));
    //         })));


    // unpack_object(Complex)->set("norm", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(std::norm(self_complex));
    //         })));

    // unpack_object(Complex)->set("norm", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         return pack_float(std::norm(self_complex));
    //         })));

    // unpack_object(Complex)->set("toString", create_object({},
    //     new func_ptr([](const std::vector<uint64_t>& _args) {
    //         uint64_t self = _args.at(0);
    //         std::complex<float> self_complex = *(std::complex<float>*)(unpack_object(self)->hidden_data);
    //         std::ostringstream oss;
    //         oss << self_complex;
    //         return create_object({},String, new std::string(oss.str()));
    //         })));

}

uint64_t input = create_object({}, Function,
    new func_ptr([](const std::vector<uint64_t>& _args) {
        std::string result;
        std::cin >> result;
        return create_object({}, String, new std::string(result));
        }));

uint64_t print = create_object({}, Function,
    new func_ptr([](const std::vector<uint64_t>& _args) {
        std::cout << string_repr(_args.at(0)) << '\n';
        return 6ULL;
        }));