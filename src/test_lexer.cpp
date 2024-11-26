#include "json_lexer.hpp"
#include <iostream>

int main() {
    sample::json_lexer l("/home/sumit/projects/json-parser/tests/2.json");
    while (l.get_token_type() != sample::json_token::END) {
        l.next();
        std::cout << l.get_token_value() << "\n";
        if (l.get_token_type() == sample::json_token::ERROR) break;
    }
    return 0;
}