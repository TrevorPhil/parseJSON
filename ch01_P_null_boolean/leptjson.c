// JSON语言
// 使用 RFC7159 中的 ABNF 表示
// JSON-text = ws value ws
// whitespace = ws
// ws = *(%x20 / %x09 / %x0A / %x0D)
// value = null / true / false
// LEPT_PARSE_EXPECT_VALUE 表示 ws 后没有值
// LEPT_PARSE_ROOT_NOT_SINGLE 表示一个正常节点后又来了其他字符
// LEPT_PARSE_INVALID_VALUE 表示 value 不是正常可接受的值
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// assert在release时候不会做检测，在debug时会
#include "leptjson.h"

// 为了防止过长的参数列表
typedef struct{
    const char* json;
}lept_context;
// 递归下降检测器
// JSON语法简单，不需要分词器
// 需要处理空白
// 检查下一个字符是什么
// 注意C语言中，-> 取到的是地址
#define EXPECT(c, ch) do{ assert(*c -> json == (ch)); c -> json++; } while(0)

static void lept_parse_whitespace(lept_context* c){
    const char* curr = c -> json;
    while(*curr == ' ' || *curr == '\t' || *curr == '\n' || *curr == '\r')
        curr++;
    c -> json = curr;
}

static int lept_parse_null(lept_context* c, lept_value* v){
    EXPECT(c, 'n');
    if(c -> json[0] != 'u' || c -> json[1] != 'l' || c -> json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c -> json += 3;
    v -> type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context* c, lept_value* v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c, lept_value* v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

// lept parse要parse掉空白
int lept_parse(lept_value* v, const char* json){
    lept_context c;
    int res;
    assert(v != NULL);
    c.json = json;
    // 若lept_parse失败，需要把v的返回值设置为NULL
    // 先设置为NULL，lept_parse_value负责写入真正的值
    v -> type = LEPT_NULL;
    lept_parse_whitespace(&c);
    if(res = lept_parse_value(&c, &v) == LEPT_PARSE_OK){
        lept_parse_whitespace(&c);
        if(*c -> json != '\0') res = LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    return res;
}

