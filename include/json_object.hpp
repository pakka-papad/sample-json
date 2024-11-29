#ifndef SAMPLE_JSON_OBJECT 
#define SAMPLE_JSON_OBJECT

#include <stdexcept>
#include <unordered_map>
#include <memory>
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
                std::unordered_map<std::string, std::shared_ptr<json_value>> _values;

                public:
                size_t size() const;

                std::vector<std::string> keys() const;

                void set(const std::string &key, const std::shared_ptr<json_value> &value);

                std::shared_ptr<json_value> get(const std::string &key);

                ~json_object() {
                    for (auto it: _values) {
                        if (it.second != nullptr) {
                            // it.second.reset();
                        }
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
                std::vector<std::shared_ptr<json_object>> _arr;

                virtual ~json_array() {
                    for (auto it: _arr) {
                        if (it != nullptr) {
                            // it.reset();
                        }
                    }
                }
            };

            class json_value_object : public json_value {
                public:
                std::shared_ptr<json_object> _obj;

                json_value_object() {
                    json_object* obj = new json_object();
                    _obj = std::shared_ptr<json_object>(obj);
                }

                virtual ~json_value_object() {
                    if (_obj != nullptr) {
                        // _obj.reset();
                    }
                }
            };

            void json_object::set(const std::string &key, const std::shared_ptr<json_value> &value) {
                _values[key] = value;
            }

            std::shared_ptr<json_value> json_object::get(const std::string &key) {
                if (_values.count(key) == 0) {
                    return nullptr;
                }
                return _values[key];
            }

            size_t json_object::size() const {
                return _values.size();
            }

            std::vector<std::string> json_object::keys() const {
                std::vector<std::string> res;
                for (auto &it: _values) {
                    res.emplace_back(it.first);
                }
                return res;
            }
        }
    }
}

#endif
