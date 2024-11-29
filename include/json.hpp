#ifndef SAMPLE_JSON_HPP
#define SAMPLE_JSON_HPP

#include <memory>
#include <string>
#include <vector>
#include "json_object.hpp"
#include "json_parser.hpp"

namespace sample {
    namespace json {

        class json {
            private:
            std::shared_ptr<internal::json_object> _obj;

            json(std::shared_ptr<internal::json_object>& obj);

            public:
            size_t key_count() const;

            std::string get_string(const std::string &key) const;

            int get_int(const std::string &key) const;

            float get_float(const std::string &key) const;

            json get_object(const std::string &key) const;

            std::vector<json> get_array(const std::string &key) const;

            static json parse_file(const std::string &file_path);

            ~json();
        };

        json::json(std::shared_ptr<internal::json_object>& obj): _obj(obj) {
        }

        json json::parse_file(const std::string &file_path) {
            static internal::json_parser_recdes parser;
            std::shared_ptr<internal::json_object> obj = parser.parse_object(file_path);
            return json(obj);
        }

        size_t json::key_count() const {
            return _obj.get()->size();
        }

        std::string json::get_string(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return value->_str;
        }

        int json::get_int(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return std::stoi(value->_str);
        }

        float json::get_float(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key not found.");
            }
            internal::json_string* value = (internal::json_string*)res;
            return std::stof(value->_str);
        }

        json json::get_object(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key not found.");
            }
            internal::json_value_object* value = (internal::json_value_object*)res;
            return json(value->_obj);
        }

        std::vector<json> json::get_array(const std::string &key) const {
            internal::json_value* res = _obj.get()->get(key).get();
            if (res == nullptr) {
                throw std::runtime_error("Key not found.");
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

