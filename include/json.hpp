#ifndef SAMPLE_JSON_HPP
#define SAMPLE_JSON_HPP

#include <memory>
#include <string>
#include <vector>
#include "json_object.hpp"
#include "json_parser.hpp"
#include "invalid_json.hpp"

namespace sample {
    namespace json {

        class json {
            private:
            std::shared_ptr<internal::json_object> _obj;

            json(std::shared_ptr<internal::json_object>& obj);

            public:
            size_t key_count() const;

            std::vector<std::string> key_set() const;

            std::string get_string(const std::string &key) const;

            int get_int(const std::string &key) const;

            float get_float(const std::string &key) const;

            bool get_bool(const std::string &key) const;

            bool is_value_null(const std::string &key) const;

            json get_object(const std::string &key) const;

            std::vector<json> get_array(const std::string &key) const;

            static json parse_file(const std::string &file_path);

            ~json();
        };

        json::json(std::shared_ptr<internal::json_object>& obj): _obj(obj) {
        }

        json json::parse_file(const std::string &file_path) {
            static internal::json_parser_recdes parser;
            std::shared_ptr<internal::json_object> obj;
            try {
                obj = parser.parse_object(file_path);
            } catch(const std::exception& e){
                throw invalid_json("Invalid json");
            }
            return json(obj);
        }

        size_t json::key_count() const {
            return _obj.get()->size();
        }

        std::vector<std::string> json::key_set() const {
            return _obj.get()->keys();
        }

        std::string json::get_string(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return value->_str;
        }

        int json::get_int(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return std::stoi(value->_str);
        }

        float json::get_float(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return std::stof(value->_str);
        }

        bool json::get_bool(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            if (value->_str == "true") {
                return true;
            } else if (value->_str == "false") {
                return false;
            }
            throw std::runtime_error("Not a bool.");
        }

        bool json::is_value_null(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            return (typeid(*res) == typeid(internal::json_null));
        }

        json json::get_object(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_value_object* value = (internal::json_value_object*)res;
            return json(value->_obj);
        }

        std::vector<json> json::get_array(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key \"" + key + "\" not found.");
            }
            internal::json_array* value = (internal::json_array*)res;
            int len = value->_arr.size();
            std::vector<json> arr;
            arr.reserve(len);
            for (int i = 0; i < len; ++i) {
                arr.push_back(json(value->_arr[i]));
            }
            return arr;
        }

        json::~json() {
            // _obj.reset();
        }
    }
}

#endif

