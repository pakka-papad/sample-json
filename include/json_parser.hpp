#ifndef SAMPLE_JSON_PARSER
#define SAMPLE_JSON_PARSER

#include <iostream>
#include <string>
#include <stdexcept>
#include "json_object.hpp"
#include "json_lexer.hpp"

namespace sample {
    namespace json {
        namespace internal {

            class json_parser_recdes {
                private:
                void object(json_lexer &lexer, json_object *res, std::string &key);
                void members(json_lexer &lexer, json_object *res);
                void member(json_lexer &lexer, json_object *res);
                void array(json_lexer &lexer, json_object *res, std::string &key);
                void elements(json_lexer &lexer, json_array *arr);
                void element(json_lexer &lexer, json_object *res, std::string &key);
                void value(json_lexer &lexer, json_object *res, std::string &key);
                
                inline void invalid_token(json_lexer &lexer);

                public:
                json_object* parse_object(const std::string &path);
            };

            inline void json_parser_recdes::invalid_token(json_lexer &lexer) {
                throw std::runtime_error("Invalid token: " + lexer.get_token_value());
            } 

            void json_parser_recdes::object(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() != json_token::LEFT_CURLY_BRACKET) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                json_value_object* value = new json_value_object();
                if (lexer.get_token_type() == json_token::RIGHT_CURLY_BRACKET) {
                    // std::cout << "key: " << key << ", value: {}\n";
                    res->set(key, value);
                    lexer.next();
                    return;
                }
                // std::cout << "key: " << key << ", value: (\n\t";
                members(lexer, value->_obj);
                res->set(key, value);
                // std::cout << ")\n";
                if (lexer.get_token_type() != json_token::RIGHT_CURLY_BRACKET) {
                    invalid_token(lexer);
                }
                lexer.next();
            }

            void json_parser_recdes::members(json_lexer &lexer, json_object *res) {
                member(lexer, res);
                if (lexer.get_token_type() != json_token::COMMA) return;
                lexer.next();
                members(lexer, res);
            }

            void json_parser_recdes::member(json_lexer &lexer, json_object *res) {
                if (lexer.get_token_type() != json_token::STRING) {
                    invalid_token(lexer);
                    return;
                }
                std::string key = lexer.get_token_value();
                lexer.next();
                if (lexer.get_token_type() != json_token::COLON) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                element(lexer, res, key);
            }

            void json_parser_recdes::array(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() != json_token::LEFT_SQUARE_BRACKET) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                json_array* a = new json_array();
                // std::cout << "key: " << key << ", value: (\n\t";
                res->set(key, a);
                if (lexer.get_token_type() == json_token::RIGHT_SQUARE_BRACKET) {
                    lexer.next();
                    return;
                }
                elements(lexer, a);
                if (lexer.get_token_type() != json_token::RIGHT_SQUARE_BRACKET) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                // std::cout << ")\n";
            }

            void json_parser_recdes::elements(json_lexer &lexer, json_array *arr) {
                std::string key2 = "";
                arr->_arr.push_back(new json_object());
                element(lexer, arr->_arr.back(), key2);
                if (lexer.get_token_type() != json_token::COMMA) {
                    return;
                }
                lexer.next();
                elements(lexer, arr);
            }

            void json_parser_recdes::element(json_lexer &lexer, json_object *res, std::string &key) {
                value(lexer, res, key);
            }

            void json_parser_recdes::value(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() == json_token::LEFT_CURLY_BRACKET) {
                    object(lexer, res, key);
                } else if (lexer.get_token_type() == json_token::LEFT_SQUARE_BRACKET) {
                    array(lexer, res, key);
                } else if (lexer.get_token_type() == json_token::NUMBER) {
                    json_string* s = new json_string();
                    s->_str = lexer.get_token_value();
                    // std::cout << "key: " << key << ", value: " << s->_str << "\n";
                    res->set(key, s);
                    lexer.next();
                } else if (lexer.get_token_type() == json_token::STRING) {
                    json_string* s = new json_string();
                    s->_str = lexer.get_token_value();
                    // std::cout << "key: " << key << ", value: " << s->_str << "\n";
                    res->set(key, s);
                    lexer.next();
                } 
            }

            json_object* json_parser_recdes::parse_object(const std::string &path) {
                json_object res;
                json_lexer lexer(path);
                lexer.next();
                std::string key = "";
                element(lexer, &res, key);
                json_value* val = res.get(key);
                res.set(key, nullptr);
                return ((json_value_object*)val)->_obj;
            }
        }
    }
}

#endif