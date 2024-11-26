#include <iostream>
#include "../include/json_parser.hpp"
#include "../include/json_object.hpp"

int main() {
    sample::json::internal::json_parser_recdes p;
    try {
        sample::json::internal::json_object* o = p.parse_object("/home/sumit/projects/json-parser/tests/2.json");
        sample::json::internal::json_string* w = (sample::json::internal::json_string*) o->get("hello");
        std::cout << "hello: " << w->_str << "\n";
        delete(o);
    } catch (const std::exception &e)  {
        std::cout << e.what() << "\n";
    }
    return EXIT_SUCCESS;
}