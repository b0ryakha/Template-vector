#pragma once

#define CHECK(test_func) ({                          \
    test_func();                                     \
    printf("[TEST]: "#test_func"() - successful\n"); \
})

#define ASSERT(assertion) ({                                                           \
    if (!(assertion)) {                                                                \
        char buffer[256];                                                              \
        snprintf(buffer, sizeof(buffer), "%s:%d: test failure\n", __FILE__, __LINE__); \
        fprintf(stderr, "%s", buffer);                                                 \
        exit(EXIT_FAILURE);                                                            \
    }                                                                                  \
})

#define INFO(message) \
    printf("[INFO]: %s\n", message);

#define DUMP(vec) ({                                                              \
    if (vec == NULL) {                                                            \
        printf("[DUMP]: "__FILE_NAME__": "#vec"<T> = NULL\n");                    \
    }                                                                             \
    else {                                                                        \
        printf("[DUMP]: "__FILE_NAME__": "#vec"<%s> = {\n",                       \
            _Generic(vec_at(vec, 0),                                              \
                const char*: "const char*",                                       \
                char*: "char*",                                                   \
                char: "char",                                                     \
                int: "int",                                                       \
                short: "short",                                                   \
                long: "long",                                                     \
                bool: "bool",                                                     \
                size_t: "size_t",                                                 \
                double: "double",                                                 \
                float: "float",                                                   \
                default: "[user_t]"                                               \
            )                                                                     \
        );                                                                        \
        printf("\t    size: %zu, cap: %zu,\n", vec_size(vec), vec_capacity(vec)); \
        printf("\t    data: {");                                                  \
        if (!vec_empty(vec)) printf(" ");                                         \
        for (size_t _i = 0; _i < vec_size(vec); ++_i) {                           \
            printf(_Generic(vec_at(vec, _i),                                      \
                const char*: "\"%s\"",                                            \
                char*: "\"%s\"",                                                  \
                char: "\'%c\'",                                                   \
                int: "%d",                                                        \
                short: "%hu",                                                     \
                long: "%ld",                                                      \
                bool: "%d",                                                       \
                size_t: "%zu",                                                    \
                double: "%lf",                                                    \
                float: "%f",                                                      \
                default: "[user_t]"                                               \
            ), vec_at(vec, _i));                                                  \
            if ((_i + 1) < vec_size(vec))                                         \
                printf(", ");                                                     \
        }                                                                         \
        if (!vec_empty(vec)) printf(" ");                                         \
        printf("}\n");                                                            \
        printf("\t};\n");                                                         \
    }                                                                             \
})