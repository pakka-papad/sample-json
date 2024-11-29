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
                std::shared_ptr<json_object> parse_object(const std::string &path);
            };

            inline void json_parser_recdes::invalid_token(json_lexer &lexer) {
                throw std::runtime_error("Invalid token: " + lexer.get_token_value());
            } 

            void json_parser_recdes::object(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() != json_token::LEFT_CURLY_BRACKET ||
                    lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                json_value_object* value = new json_value_object();
                if (lexer.get_token_type() == json_token::RIGHT_CURLY_BRACKET) {
                    res->set(key, std::shared_ptr<json_value>(value));
                    lexer.next();
                    return;
                }
                members(lexer, value->_obj.get());
                res->set(key, std::shared_ptr<json_value>(value));
                if (lexer.get_token_type() != json_token::RIGHT_CURLY_BRACKET) {
                    invalid_token(lexer);
                }
                lexer.next();
            }

            void json_parser_recdes::members(json_lexer &lexer, json_object *res) {
                if (lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                member(lexer, res);
                if (lexer.get_token_type() != json_token::COMMA) return;
                lexer.next();
                members(lexer, res);
            }

            void json_parser_recdes::member(json_lexer &lexer, json_object *res) {
                if (lexer.get_token_type() != json_token::STRING || 
                    lexer.get_token_type() == json_token::ERROR) {
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
                if (lexer.get_token_type() != json_token::LEFT_SQUARE_BRACKET ||
                    lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                lexer.next();
                json_array* a = new json_array();
                res->set(key, std::shared_ptr<json_value>(a));
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
            }

            void json_parser_recdes::elements(json_lexer &lexer, json_array *arr) {
                if (lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                std::string key2 = "";
                arr->_arr.emplace_back(new json_object());
                element(lexer, arr->_arr.back().get(), key2);
                if (lexer.get_token_type() != json_token::COMMA) {
                    return;
                }
                lexer.next();
                elements(lexer, arr);
            }

            void json_parser_recdes::element(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                value(lexer, res, key);
            }

            void json_parser_recdes::value(json_lexer &lexer, json_object *res, std::string &key) {
                if (lexer.get_token_type() == json_token::ERROR) {
                    invalid_token(lexer);
                    return;
                }
                if (lexer.get_token_type() == json_token::LEFT_CURLY_BRACKET) {
                    object(lexer, res, key);
                } else if (lexer.get_token_type() == json_token::LEFT_SQUARE_BRACKET) {
                    array(lexer, res, key);
                } else if (lexer.get_token_type() == json_token::NUMBER ||
                    lexer.get_token_type() == json_token::STRING ||
                    lexer.get_token_type() == json_token::TRUE ||
                    lexer.get_token_type() == json_token::FALSE) {
                    json_string* s = new json_string();
                    s->_str = lexer.get_token_value();
                    res->set(key, std::shared_ptr<json_value>(s));
                    lexer.next();
                } else if (lexer.get_token_type() == json_token::JNULL) {
                    json_null* s = new json_null();
                    res->set(key, std::shared_ptr<json_value>(s));
                    lexer.next();
                } 
            }

            std::shared_ptr<json_object> json_parser_recdes::parse_object(const std::string &path) {
                json_object res;
                json_lexer lexer(path);
                lexer.next();
                std::string key = "";
                element(lexer, &res, key);
                std::shared_ptr<json_value> val = res.get(key);
                return ((json_value_object*)val.get())->_obj;
            }
        }
    }
}

#endif