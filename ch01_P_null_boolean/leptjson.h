// include guard
#ifndef LEPTJSON_H__
#define LEPTJSON_H__

#endif

// define a data type with enumerate
// no namespace in C: use abbreviation of project
typedef enum{
    LEPT_TRUE, LEPT_FALSE, LEPT_NUMBER,
    LEPT_NULL, LEPT_STRING, 
    LEPT_ARRAY, LEPT_OBJECT
}lept_type;

// define a node type using struct
// a node indicates a type(where is value?) in tree 
// typedef avoids usage of struct everywhere
typedef struct{
    lept_type type;
}lept_value;

// parse JSON
// parameters: 
// v: pointer to node, allocated by user of this function
// json: raw json string
int lept_parse(lept_value* v, const char* json);

// return values of lept_parse
// enumerate macro values
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR
};

// return type of a node
lept_type lept_get_type(const lept_value* v);
