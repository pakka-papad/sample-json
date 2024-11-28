#include <iostream>
#include <string>
#include <stdexcept>
#include "../include/json.hpp"
using namespace sample::json;

void assert_equal(const std::string &s1, const std::string &s2) {
    if (s1 != s2) {
        throw std::runtime_error("Expected: " + s1 + ", found: " + s2);
    }
}

void assert_equal(const int i1, const int i2) {
    if (i1 != i2) {
        throw std::runtime_error("Expected: " + std::to_string(i1) + ", found: " + std::to_string(i2));
    }
}

void assert_equal(const float f1, const float f2) {
    if (f1 != f2) {
        throw std::runtime_error("Expected: " + std::to_string(f1) + ", found: " + std::to_string(f2));
    }
}

void test_1() {
    json obj = json::parse_file("tests/1.json");
    std::string world = obj.get_string("hello");
    assert_equal("world", world);
    std::cout << "Test 1 passed\n";
}

void test_2() {
    json obj = json::parse_file("tests/2.json");
    std::string world = obj.get_string("hello");
    assert_equal("world", world);
    json obj2 = obj.get_object("say");
    int _23 = obj2.get_int("number");
    assert_equal(23, _23);
    std::string wow = obj2.get_string("string");
    assert_equal("wow\"wow", wow);
    std::vector<json> arr = obj2.get_array("special");
    assert_equal("true", arr[0].get_string(""));
    assert_equal("false", arr[1].get_string(""));
    assert_equal("null", arr[2].get_string(""));
    std::cout << "Test 2 passed\n";
}

int main() {
    test_1();
    test_2();
    return EXIT_SUCCESS;
}