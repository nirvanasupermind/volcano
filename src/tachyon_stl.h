#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <thread>
#include <utility>

#define TACHYON_OBJ std::vector<std::pair<std::string, double> >
#define TACHYON_FUNC std::function<double(const std::vector<double>&)>
#define STR_TAG 0x7ffc000000000000
#define VEC_TAG 0x7ffd000000000000
#define FUNC_TAG 0x7ffe000000000000
#define OBJ_TAG 0x7fff000000000000
#define PTR_MASK 0xffffffffffff
#define TYPECHECK_MASK 0xffff000000000000

// Namespace to encapsulate internals for Tachyon
namespace tachyon_internal {
    std::vector<void*> all_ptrs;

    inline double make_str(std::string* str) {
        all_ptrs.push_back(str);
        uint64_t u = ((uint64_t)str) | STR_TAG;
        return *(double*)(&u);
    }

    inline double make_vec(std::vector<double>* vec) {
        all_ptrs.push_back(vec);
        uint64_t u = ((uint64_t)vec) | VEC_TAG;
        return *(double*)(&u);
    }

    inline double make_func(TACHYON_FUNC* func) {
        all_ptrs.push_back(func);
        uint64_t u = ((uint64_t)func) | FUNC_TAG;
        return *(double*)(&u);
    }

    inline double make_obj(TACHYON_OBJ* obj) {
        all_ptrs.push_back(obj);
        uint64_t u = ((uint64_t)obj) | OBJ_TAG;
        return *(double*)(&u);
    }

    inline std::string* decode_str(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (std::string*)(ptr);
    }

    inline std::vector<double>* decode_vec(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (std::vector<double>*)(ptr);
    }

    inline TACHYON_FUNC* decode_func(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (TACHYON_FUNC*)(ptr);
    }

    inline TACHYON_OBJ* decode_obj(double d) {
        uint64_t ptr = (*(uint64_t*)(&d)) & PTR_MASK;
        return (TACHYON_OBJ*)(ptr);
    }

    inline void free_all() {
        for (void* ptr : all_ptrs) {
            free(ptr);
        }
        all_ptrs.clear();
    }


    inline double get_prop(TACHYON_OBJ* obj, const std::string& key) {

        for (auto& pair : *obj) {
            if (pair.first == key) return pair.second;
        }

        for (auto& pair : *obj) {
            if (pair.first == "proto") return get_prop(decode_obj(pair.second), key);
        }


        throw std::runtime_error("key not found: " + key);
    }

    inline bool has_prop(TACHYON_OBJ* obj, const std::string& key) {
        for (auto& pair : *obj) {
            if (pair.first == key) return true;
        }
        for (auto& pair : *obj) {
            if (pair.first == "proto") return has_prop(decode_obj(pair.second), key);
        }
        return false;
    }

    inline double set_prop(TACHYON_OBJ* obj, const std::string& key, double val) {
        for (size_t i = 0; i < obj->size(); ++i) {
            if ((*obj)[i].first == key) {
                (*obj)[i].second = val;
                return val;
            }
        }
        obj->emplace_back(key, val);
        return val;
    }


    inline double is_eq(double a, double b) {
        uint64_t a_u = *(uint64_t*)(&a);
        uint64_t b_u = *(uint64_t*)(&b);

        return a_u == b_u;
    }



    inline double is_ne(double a, double b) {
        uint64_t a_u = *(uint64_t*)(&a);
        uint64_t b_u = *(uint64_t*)(&b);

        return a_u != b_u;
    }


    inline bool is_str(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == STR_TAG;
    }

    inline bool is_vec(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == VEC_TAG;
    }

    inline bool is_func(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == FUNC_TAG;
    }

    inline bool is_obj(double d) {
        uint64_t u = *(uint64_t*)(&d);
        return (u & TYPECHECK_MASK) == OBJ_TAG;
    }

    inline double get_subscript(double base, double idx) {
        if (is_str(base)) {
            return make_str(new std::string(1, (*decode_str(base)).at(idx)));
        }
        else if (is_vec(base)) {
            return (*decode_vec(base)).at(idx);
        }
        else if (is_obj(base)) {
            return get_prop(decode_obj(base), *decode_str(idx));
        }
        else {
            throw std::runtime_error("illegal subscript");
        }
    }


    inline double set_subscript(double base, double idx, double val) {
        if (is_str(base)) {
            (*decode_str(base))[idx] = val;
        }
        else if (is_vec(base)) {
            (*decode_vec(base))[idx] = val;
        }
        else if (is_obj(base)) {
            set_prop(decode_obj(base), *decode_str(idx), val);
        }
        else {
            throw std::runtime_error("illegal subscript");
        }
        return val;
    }
}

double Math = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double StringUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));
double VectorUtils = tachyon_internal::make_obj(new TACHYON_OBJ({}));

double print = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    double x = _args.at(0);
    if (tachyon_internal::is_obj(x)) {
        TACHYON_OBJ* obj = tachyon_internal::decode_obj(x);
        if (tachyon_internal::has_prop(obj, "toString")) {
            double temp = (*tachyon_internal::decode_func(tachyon_internal::get_prop(obj, "toString")))({ x });
            std::cout << *tachyon_internal::decode_str(temp);
        }
        else {
            std::cout << obj;
        }
    }
    else if (tachyon_internal::is_str(x)) {
        std::cout << *tachyon_internal::decode_str(x);
    }
    else if (tachyon_internal::is_vec(x)) {
        std::vector<double> vec = *tachyon_internal::decode_vec(x);
        std::cout << '[';
        for (int i = 0; i < vec.size(); i++) {
            (*tachyon_internal::decode_func(print))({ vec.at(i) });
            if (i != vec.size() - 1) {
                std::cout << ',';
            }
        }
        std::cout << ']';
    }
    else if (tachyon_internal::is_func(x)) {
        std::cout << tachyon_internal::decode_func(x);
    }
    else {
        std::cout << x;
    }
    }));

double println = tachyon_internal::make_func(new TACHYON_FUNC([](const std::vector<double>& _args) -> double {
    double x = _args.at(0);
    (*tachyon_internal::decode_func(print))({ x });
    std::cout << '\n';
    }));

// Tachyon standard library setup function
void tachyon_stl_setup() {
    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "abs", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::abs(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "exp", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::exp(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "exp2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::exp2(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "expm1", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::expm1(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "log", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "log10", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log10(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "log2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log2(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "log1p", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::log1p(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "pow", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::pow(_args.at(1), _args.at(2));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "sqrt", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sqrt(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "cbrt", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cbrt(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "hypot", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::hypot(_args.at(1), _args.at(2));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "cos", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cos(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "sin", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sin(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "tan", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tan(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "acos", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::acos(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "asin", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::asin(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "atan", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atan(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "atan2", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atan2(_args.at(1), _args.at(2));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "cosh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::cosh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "sinh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::sinh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "tanh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tanh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "acosh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::acosh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "asinh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::asinh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "atanh", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::atanh(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "erf", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::erf(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "erfc", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::erfc(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "gamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tgamma(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "lgamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::lgamma(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "ceil", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::ceil(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "floor", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::floor(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "gamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::tgamma(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "lgamma", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::lgamma(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "ceil", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::ceil(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "floor", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::floor(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "trunc", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::trunc(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "round", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::round(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "isfinite", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isfinite(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "isinf", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isinf(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "isnan", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::isnan(_args.at(1));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(Math), "ceil", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        return std::ceil(_args.at(1));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "front", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        return tachyon_internal::make_str(new std::string(1, str->front()));
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "back", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        return tachyon_internal::make_str(new std::string(1, str->back()));
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "empty", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        return str->empty();
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "size", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        return str->size();
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "clear", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        str->clear();
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "insert", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        double idx = _args.at(2);
        std::string* str2 = tachyon_internal::decode_str(_args.at(3));
        str->insert(idx, *str2);
        })));


    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "erase", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        double idx = _args.at(1);
        double count = _args.at(2);
        str->erase(idx, count);
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "popBack", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        str->pop_back();
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "append", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        std::string* str2 = tachyon_internal::decode_str(_args.at(2));
        str->append(*str2);
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "replace", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        double pos = _args.at(2);
        double count = _args.at(3);
        std::string* str2 = tachyon_internal::decode_str(_args.at(4));
        str->replace(pos, count, *str2);
        })));

    tachyon_internal::set_prop(tachyon_internal::decode_obj(StringUtils), "find", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
        std::string* str = tachyon_internal::decode_str(_args.at(1));
        std::string* str2 = tachyon_internal::decode_str(_args.at(2));
        return str->find(*str2);
        })));

    // tachyon_internal::all_objs.reserve(1000000);
    // Initialize any required standard library components here
    // Initialize any required standard library components here
    // tachyon_internal::set_prop(tachyon_internal::decode_obj(String), "toString", tachyon_internal::make_func(new TACHYON_FUNC([=](const std::vector<double>& _args) -> double {
    //     return _args.at(0);
    // })));
}