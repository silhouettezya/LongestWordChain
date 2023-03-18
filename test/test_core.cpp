#include <iostream>
#include "gtest/gtest.h"
#include "src/core.h"
#include <vector>
#include <algorithm>
#include <cassert>


void test_gen_chain_word(const char* words[], int len, const char* ans[], int ans_len, char head, char tail, char reject, bool enable_loop) {
    char** result = (char**)malloc(10000);
    int out_len = gen_chain_word(words, len, result, head, tail, reject, enable_loop);
    ASSERT_EQ(ans_len, out_len);
    for (int i = 0;i < ans_len;i++) {
        if (result != nullptr) ASSERT_EQ(strcmp(ans[i], result[i]), 0);
    }
    free(result);
}

// gen_chain_word
// -w
TEST(gen_chain_word, example_w) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant", "trick"};
    test_gen_chain_word(words, 11, ans, 4, 0, 0, 0, false);
}

TEST(gen_chain_word, example_w1) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant", "trick"};
    test_gen_chain_word(words, 11, ans, 4, 0, 0, 0, true);
}

// -h
TEST(gen_chain_word, example_h) {
    const char* words[] = { "algebra","apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "elephant", "trick" };
    test_gen_chain_word(words, 11, ans, 2, 'e', 0, 0, false);
}

// -t
TEST(gen_chain_word, example_t) {
    const char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "algebra", "apple", "elephant" };
    test_gen_chain_word(words, 11, ans, 3, 0, 't', 0, false);
}

// -j
TEST(gen_chain_word, example_j) {
    const char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "leaf", "fox" };
    test_gen_chain_word(words, 11, ans, 2, 0, 0, 'a', false);
}

// -h, -t
TEST(gen_chain_word, example_h_t) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant"};
    test_gen_chain_word(words, 11, ans, 3, 'a', 't', 0, false);
}

// -h, -j
TEST(gen_chain_word, example_h_j) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant", "trick"};
    test_gen_chain_word(words, 11, ans, 4, 'a', 0, 'u', false);
}

// -h -t -j
TEST(gen_chain_word, example_h_t_j) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant"};
    test_gen_chain_word(words, 11, ans, 3, 'a', 't', 'l', false);
}

// -r
TEST(gen_chain_word, example_r) {
        const char* words[] = {"element", "heaven", "table", "teach", "talk"};
        const char* ans[] = {"table", "element", "teach", "heaven"};
        test_gen_chain_word(words, 5, ans, 4, 0, 0, 0, true);
}

TEST(gen_chain_word, example_com) {
    const char* words[] = {"aa", "ab", "bb", "bc", "cc", "de", "ef", "fg", "gh"};
    const char* ans[] = {"aa", "ab", "bb", "bc", "cc"};
    test_gen_chain_word(words, 9, ans, 5, 0, 0, 0, false);
}
// ------------------------------------------------------------------------


// gen_chain_char
void test_gen_chain_char(const char* words[], int len, const char* ans[], int ans_len, char head, char tail, char reject, bool enable_loop) {
    char** result = (char**)malloc(10000);
    int out_len = gen_chain_char(words, len, result, head, tail, reject, enable_loop);
    ASSERT_EQ(ans_len, out_len);
    for (int i = 0;i < ans_len;i++) {
        if (result != nullptr) ASSERT_EQ(strcmp(ans[i], result[i]), 0);
    }
    free(result);
}
// -c
TEST(gen_chain_char, example_c) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"pseudopseudohypoparathyroidism", "moon"};
    test_gen_chain_char(words, 11, ans, 2, 0, 0, 0, false);
}

TEST(gen_chain_char, example_c1) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"pseudopseudohypoparathyroidism", "moon"};
    test_gen_chain_char(words, 11, ans, 2, 0, 0, 0, true);
}

// -h
TEST(gen_chain_char, example_h) {
    const char* words[] = { "algebra","apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "elephant", "trick" };
    test_gen_chain_char(words, 11, ans, 2, 'e', 0, 0, false);
}

// -t
TEST(gen_chain_char, example_t) {
    const char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "algebra", "apple", "elephant" };
    test_gen_chain_char(words, 11, ans, 3, 0, 't', 0, false);
}

// -j
TEST(gen_chain_char, example_j) {
    const char* words[] = { "algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism" };
    const char* ans[] = { "pseudopseudohypoparathyroidism", "moon"};
    test_gen_chain_char(words, 11, ans, 2, 0, 0, 'a', false);
}

// -h, -t
TEST(gen_chain_char, example_h_t) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant"};
    test_gen_chain_char(words, 11, ans, 3, 'a', 't', 0, false);
}

// -h, -j
TEST(gen_chain_char, example_h_j) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant", "trick"};
    test_gen_chain_char(words, 11, ans, 4, 'a', 0, 'u', false);
}

// -h -t -j
TEST(gen_chain_char, example_h_t_j) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra", "apple", "elephant"};
    test_gen_chain_char(words, 11, ans, 3, 'a', 't', 'u', false);
}

// -r
TEST(gen_chain_char, example_r) {
    const char* words[] = {"element", "heaven", "table", "teach", "talk"};
    const char* ans[] = {"table", "element", "teach", "heaven"};
    test_gen_chain_char(words, 5, ans, 4, 0, 0, 0, true);
}
// ------------------------------------------------------------------------


// gen_chain_all
void test_gen_chains_all(const char* words[], int len, const char* ans[], int ans_len) {
    char** result = (char**)malloc(10000);
    int out_len = gen_chains_all(words, len, result);
    ASSERT_EQ(ans_len, out_len);
    for (int i = 0;i < ans_len;i++) {
        if (result != nullptr) ASSERT_EQ(strcmp(ans[i], result[i]), 0);
    }
    free(result);
}
// -n
TEST(gen_chains_all, example_n) {
    const char* words[] = {"algebra", "apple", "zoo", "elephant", "under", "fox", "dog", "moon", "leaf", "trick", "pseudopseudohypoparathyroidism"};
    const char* ans[] = {"algebra apple", "algebra apple elephant", "algebra apple elephant trick", "apple elephant", "apple elephant trick", "elephant trick", "leaf fox", "pseudopseudohypoparathyroidism moon"};
    test_gen_chains_all(words, 11, ans, 8);
}


// --------------------------------------------------------------

// exception
TEST(engine, example_hasCircle1) {
    const char* words[] = {"element", "heaven", "table", "teach", "talk"};
    const char* ans[] = {"table", "element", "teach", "heaven"};
    try {
        char** result = (char**)malloc(10000);
        int out_len = engine(words,5, result, 0, 0, 0, false, 0);
    }
    catch(std::logic_error const& e) {
        ASSERT_EQ(strcmp("the chain has circle without -r!", e.what()), 0);
    }
}

TEST(engine, example_hasCircle2) {
    const char* words[] = {"element", "heaven", "table", "teach", "talk"};
    const char* ans[] = {"table", "element", "teach", "heaven"};
    try {
        char** result = (char**)malloc(10000);
        int out_len = engine(words,5, result, 0, 0, 0, false, 1);
    }
    catch(std::logic_error const& e) {
        ASSERT_EQ(strcmp("the chain has circle without -r!", e.what()), 0);
    }
}

TEST(engine, example_hasCircle3) {
    const char* words[] = {"element", "heaven", "table", "teach", "talk"};
    const char* ans[] = {"table", "element", "teach", "heaven"};
    try {
        char** result = (char**)malloc(10000);
        int out_len = engine(words,5, result, 0, 0, 0, false, 2);
    }
    catch(std::logic_error const& e) {
        ASSERT_EQ(strcmp("the chain has circle without -r!", e.what()), 0);
    }
}

TEST(engine, example_hasCircle4) {
const char* words[] = {"element", "heaven", "table", "teach", "talk"};
const char* ans[] = {"table", "element", "teach", "heaven"};
try {
char** result = (char**)malloc(10000);
int out_len = engine(words,5, result, 0, 0, 0, false, 2);
}
catch(std::logic_error const& e) {
ASSERT_EQ(strcmp("the chain has circle without -r!", e.what()), 0);
}
}

TEST(engine, example_wrong_type) {
    const char* words[] = {"element", "heaven", "table", "teach", "talk"};
    char** result = (char**)malloc(10000);
    int out_len = engine(words,5, result, 0, 0, 0, false, -1);
    ASSERT_EQ(out_len, 0);
}


int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}