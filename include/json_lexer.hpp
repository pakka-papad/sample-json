#ifndef SAMPLE_JSON_LEXER
#define SAMPLE_JSON_LEXER

#include <fstream>
#include <iostream>
#include <stdexcept>
#include "json_token.hpp"

namespace sample {
    namespace json {
        namespace internal {

            class json_lexer {
                private:
                std::ifstream _file;

                std::string _token;
                json_token _token_type;

                std::string _line;
                size_t _line_num;
                size_t _pos;

                void require_open() const;

                bool find_single_char_token();
                bool find_string();
                bool find_number();
                bool find_special_string();

                void load_next_char();
                bool is_blank(const char &c) const;
                bool is_digit(const char &c) const;

                public:

                /**
                 * @param file_name 
                 */
                json_lexer(std::string file_name);
                
                ~json_lexer(); 

                void next();

                std::string get_token_value() const noexcept;

                json_token get_token_type() const noexcept;
            };


            void json_lexer::require_open() const {
                if (!_file.is_open()) {
                    throw std::runtime_error("Failed to read file");
                }
            }

            json_lexer::json_lexer(std::string file_name) {
                _file.open(file_name);
                if (!_file.is_open()) {
                    throw std::runtime_error("Failed to open file: " + file_name);
                }
                _token = "";
                _token_type = json_token::START;
                _line = "";
                _line_num = 0;
                _pos = 0;
            }

            json_lexer::~json_lexer() {
                try {
                    _file.close();
                } catch (std::exception e) {

                }
            }

            void json_lexer::next() {
                if (_token_type == json_token::ERROR) {
                    return;
                }
                require_open();
                load_next_char();
                if (find_single_char_token()) {
                    return;
                }
                if (find_string()) {
                    return;
                }
                if (find_number()) {
                    return;
                }
                if (find_special_string()) {
                    return;
                }
                _token_type = (_token.size() == 0) ? json_token::END : json_token::ERROR;        
            }

            bool json_lexer::find_single_char_token() {
                if (_token.size() != 1) return false;
                bool done = true;
                if (_token[0] == '{') {
                    _token_type = json_token::LEFT_CURLY_BRACKET;
                } else if (_token[0] == '}') {
                    _token_type = json_token::RIGHT_CURLY_BRACKET;
                } else if (_token[0] == '[') {
                    _token_type = json_token::LEFT_SQUARE_BRACKET;
                } else if (_token[0] == ']') {
                    _token_type = json_token::RIGHT_SQUARE_BRACKET;
                } else if (_token[0] == ',') {
                    _token_type = json_token::COMMA;
                } else if (_token[0] == ':') {
                    _token_type = json_token::COLON;
                } else {
                    done = false;
                }
                return done;
            }

            bool json_lexer::find_string() {
                if (_token.size() != 1) return false;
                if (_token[0] != '"') return false;
                _token.pop_back();
                bool done = false;
                bool escape = false;
                while (!done) {
                    while(!done && _pos < _line.size()) {
                        _token.push_back(_line[_pos]);
                        _pos++;
                        if (!escape && _token.back() == '"') {
                            _token.pop_back();
                            done = true;
                        }
                        escape = false; 
                        if (_token.back() == '\\') {
                            escape = true;
                            _token.pop_back();
                        }
                    }
                    if (done) break;
                    if (_file.eof()) break;
                    std::getline(_file, _line);
                    _line_num++;
                    _pos = 0;
                }
                _token_type = done ? json_token::STRING : json_token::ERROR;
                return done;
            }

            bool json_lexer::find_number() {
                if (_token.size() != 1) return false;
                if (_token[0] != '+' && _token[0] != '-' && !is_digit(_token[0])) {
                    return false;
                }
                bool dot = false;
                while (_pos < _line.size()) {
                    char &c = _line[_pos];
                    if (dot && c == '.') {
                        _token_type = json_token::ERROR;
                        return false;
                    }
                    if (c == '.' && !is_digit(_token.back())) {
                        _token_type = json_token::ERROR;
                        return false;
                    }
                    if (c == '.' || is_digit(c)) {
                        _token.push_back(c);
                        _pos++;
                        dot = (c == '.');
                    } else {
                        break;        
                    }
                }
                _token_type = json_token::NUMBER;
                return true;
            }
            
            bool json_lexer::find_special_string() {
                if (_token.size() != 1) return false;
                if (_token[0] != 't' && _token[0] != 'f' && _token[0] != 'n') return false;
                for (int i = 0; i < 3; i++) {
                    if (_pos >= _line.size()) {
                        _token_type = json_token::ERROR;
                        return false;
                    }
                    _token.push_back(_line[_pos]);
                    _pos++;
                }
                if (_token == "true" || _token == "null") {
                    _token_type = (_token == "true") ? json_token::TRUE : json_token::JNULL;
                    return true;
                }
                if (_pos >= _line.size()) {
                    _token_type = json_token::ERROR;
                    return false;
                }
                _token.push_back(_line[_pos]);
                _pos++;
                if (_token == "false") {
                    _token_type = json_token::FALSE;
                    return true;
                }
                _token_type = json_token::ERROR;
                return false;
            }

            void json_lexer::load_next_char() {
                _token = "";
                while (true) {
                    while (_pos < _line.size()) {
                        char &c = _line[_pos];
                        _pos++;
                        if (!is_blank(c)) {
                            _token.push_back(c);
                            return;
                        }
                    }
                    if (_file.eof()) break;
                    std::getline(_file, _line);
                    _pos = 0;
                    _line_num++;
                }
            }
            
            bool json_lexer::is_blank(const char &c) const {
                return (c == ' ' || c == '\t' || c == '\n');
            }

            bool json_lexer::is_digit(const char &c) const {
                return (c >= '0' && c <= '9');
            }

            std::string json_lexer::get_token_value() const noexcept {
                return _token;
            }

            json_token json_lexer::get_token_type() const noexcept {
                return _token_type;
            }
        }
    }
}

#endif