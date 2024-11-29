#ifndef SAMPLE_JSON_TOKEN
#define SAMPLE_JSON_TOKEN

namespace sample {
    namespace json {
        namespace internal {
            
            enum json_token {
                START,
                ERROR,
                LEFT_CURLY_BRACKET,
                RIGHT_CURLY_BRACKET,
                LEFT_SQUARE_BRACKET,
                RIGHT_SQUARE_BRACKET,
                COMMA,
                COLON,
                NUMBER,
                STRING,
                JNULL,
                TRUE,
                FALSE,
                END
            };
        }
    }
}

#endif