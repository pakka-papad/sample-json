#ifndef SAMPLE_INVALID_JSON_EXCEPTION
#define SAMPLE_INVALID_JSON_EXCEPTION

#include <stdexcept>

namespace sample {
    namespace json {
        class invalid_json: public std::exception {
            private:
            std::string message;

            public:
            invalid_json(const std::string& message): message(message) {

            }

            const char* what() const throw() {
                return message.c_str();
            }
        };
    }
}

#endif