#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leptjson.h"

// Return value indicating status
static int main_ret = 0;
// Number of passed test
static int test_pass = 0;
// Rounds of testing
static int test_count = 0;
// expect equality
// parameter: 
// equality: judge if expect == actual
// expect: expected value
// actual: actual value
// format: format for printing
// 宏里有if else语句时，需要用do while(0)
// 否则会出现else脱离的问题
// 测试：
// 记录测试轮次
// 判断expect是否等于actual
// 如果不对，用format输出
// __FILE__ 表示当前文件名
// __LINE__ 表示行号
// fprintf()?
#define EXPECT_EQ_BASE(equality, expect, actual, format)\
    do{\
        test_count++;\
        if(equality)\
            test_pass++;\
        else{\
            fprintf(stderr, "%s:%d: expect:" format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    }while(0)

// kind of inheritance
// base -> int
#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect == actual), expect, actual, "%d")

// parse null test
static void test_parse_null(){
    // it doesn't matter what initial value this node has
    lept_value v;
    v.type = LEPT_TRUE; // whatever
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "null"));
    // Has type changed after parsing?
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

// copy & paste
static void test_parse_true() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "true"));
    EXPECT_EQ_INT(LEPT_TRUE, lept_get_type(&v));
}

static void test_parse_false() {
    lept_value v;
    v.type = LEPT_TRUE;
    EXPECT_EQ_INT(LEPT_PARSE_OK, lept_parse(&v, "false"));
    EXPECT_EQ_INT(LEPT_FALSE, lept_get_type(&v));
}

static void test_parse_expect_value() {
    lept_value v;

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, ""));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_EXPECT_VALUE, lept_parse(&v, " "));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

static void test_parse_invalid_value() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "nul"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));

    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_INVALID_VALUE, lept_parse(&v, "?"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

static void test_parse_root_not_singular() {
    lept_value v;
    v.type = LEPT_FALSE;
    EXPECT_EQ_INT(LEPT_PARSE_ROOT_NOT_SINGULAR, lept_parse(&v, "null x"));
    EXPECT_EQ_INT(LEPT_NULL, lept_get_type(&v));
}

// 总集合
static void test_parse(){
    test_parse_null();
}

// 调用test总集
// 输出正确率
int main(){
    test_parse();
    // print at least 3 digits of number in precision of 2
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}