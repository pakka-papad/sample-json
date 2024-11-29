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
    json obj = json::parse_file("../tests/1.json");
    std::string world = obj.get_string("hello");
    assert_equal("world", world);
    std::cout << "Test 1 passed\n";
}

void test_2() {
    json obj = json::parse_file("../tests/2.json");
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

void test_3() {
    json obj = json::parse_file("../tests/3.json");
    std::string value = obj.get_string("key");
    assert_equal("value", value);
    int n = obj.get_int("key-n");
    assert_equal(101, n);
    assert_equal(0, obj.get_object("key-o").key_count());
    assert_equal(0, obj.get_array("key-l").size());
    std::cout << "Test 3 passed\n";
}

void test_4() {
    json obj = json::parse_file("../tests/4.json");
    std::string value = obj.get_string("key");
    assert_equal("value", value);
    int n = obj.get_int("key-n");
    assert_equal(101, n);
    json key_o = obj.get_object("key-o");
    assert_equal(1, key_o.key_count());
    assert_equal("inner value", key_o.get_string("inner key"));
    std::vector<json> key_l = obj.get_array("key-l");
    assert_equal(1, key_l.size());
    assert_equal("list value", key_l[0].get_string(""));
    std::cout << "Test 4 passed\n";
}

void test_5() {
    try {
        json obj = json::parse_file("../tests/5.json");
    } catch(const invalid_json& e) {
        std::cout << "Test 5 passed\n";
        return;
    } catch(const std::exception& e) {
        std::cout << "Test 5 failed\n";
    }
    std::cout << "Test 5 failed\n";
}

void test_6() {
    json obj = json::parse_file("../tests/6.json");
    assert_equal(5, obj.key_count());
    assert_equal(true, obj.get_bool("key1"));
    assert_equal(false, obj.get_bool("key2"));
    assert_equal(true, obj.is_value_null("key3"));
    assert_equal("value", obj.get_string("key4"));
    assert_equal(101, obj.get_int("key5"));
    std::cout << "Test 6 passed\n";
}

void test_7() {
    try {
        json obj = json::parse_file("../tests/7.json");
    } catch(const invalid_json& e) {
        std::cout << "Test 7 passed\n";
        return;
    } catch(const std::exception& e) {
        std::cout << "Test 7 failed\n";
    }
    std::cout << "Test 7 failed\n";
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    return EXIT_SUCCESS;
}