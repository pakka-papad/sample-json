#ifndef SAMPLE_JSON_OBJECT 
#define SAMPLE_JSON_OBJECT

#include <stdexcept>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

namespace sample {
    namespace json {
        namespace internal {

            class json_value {
                public:
                virtual ~json_value() {
                }
            };

            class json_object {
                private:
                std::unordered_map<std::string, json_value*> _values;

                public:
                void set(const std::string &key, json_value* value);

                json_value* get(const std::string &key);

                ~json_object() {
                    for (auto &it: _values) {
                        if (it.second != nullptr) delete(it.second);
                    }
                }
            };
            
            class json_string : public json_value {
                public:
                std::string _str;

                virtual ~json_string() {
                }
            };

            class json_array : public json_value {
                public:
                std::vector<json_object*> _arr;

                json_array() {
                }

                virtual ~json_array() {
                    for (auto it: _arr) {
                        if (it != nullptr) delete(it);
                    }
                }
            };

            class json_value_object : public json_value {
                public:
                json_object* _obj;

                json_value_object() {
                    _obj = new json_object();
                }

                virtual ~json_value_object() {
                    if (_obj != nullptr) delete(_obj);
                }
            };

            void json_object::set(const std::string &key, json_value* value) {
                _values[key] = value;
            }

            json_value* json_object::get(const std::string &key) {
                return _values[key];
            }

        }
    }
}

#endif
