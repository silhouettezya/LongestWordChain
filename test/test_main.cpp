//
// Created by z'ya on 2023/3/19.
//
#include <iostream>
#include "gtest/gtest.h"
#include "src/main.h"
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

TEST(main_serve, example_wrong_arg) {
    try{
        const char* args[] = {"Wordlist.exe", "a"};
        main_serve(2, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("this argument is invalid!", e.what()));
        return;
    }
}

TEST(main_serve, example_repeat_arg) {
    try{
        const char* args[] = {"Wordlist.exe", "-n", "-n", "test.txt"};
        main_serve(4, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("arg: -n repeat!", e.what()));
        return;
    }
}

TEST(main_serve, example_n_with_other) {
    try{
        const char* args[] = {"Wordlist.exe", "-n", "-c", "test.txt"};
        main_serve(4, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("-n can't be used with other arguments!", e.what()));
        return;
    }
}

TEST(main_serve, example_w_with_c) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-c", "test.txt"};
        main_serve(4, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("-w can't be used with -c!", e.what()));
        return;
    }
}

TEST(main_serve, example_h_wrong) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-h", "aa", "test.txt"};
        main_serve(5, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("arg: -h following argument is wrong!", e.what()));
        return;
    }
}

TEST(main_serve, example_h_miss) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-h"};
        main_serve(3, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("arg: -h missing following argument!", e.what()));
        return;
    }
}

TEST(main_serve, example_filename_wrong) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-h", "a"};
        main_serve(4, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("don't get valid filename!", e.what()));
        return;
    }
}

TEST(main_serve, example_without_n_w_c) {
    try{
        const char* args[] = {"Wordlist.exe", "-r", "-h", "a", "test4.txt"};
        main_serve(5, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("need -n or -w or -c!", e.what()));
        return;
    }
}

TEST(main_serve, example_filename_repeat) {
    try{
        const char* args[] = {"Wordlist.exe", "-c", "-h", "a", "test1.txt", "test2.txt"};
        main_serve(6, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("filename is repeat!", e.what()));
        return;
    }
}

TEST(main_serve, example_open_filename_wrong) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-t", "a", "test100000.txt"};
        main_serve(5, args);
    } catch(invalid_argument const &e){
        ASSERT_EQ(0, strcmp("opening file fail!", e.what()));
        return;
    }
}

TEST(main_serve, example_more_than_10000) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-t", "a", "D:\\test_wordchain\\test1.txt"};
        main_serve(5, args);
    } catch(runtime_error const &e){
        ASSERT_EQ(0, strcmp("words are more than 10000!", e.what()));
        return;
    }
}

TEST(main_serve, example_more_than_100) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "-t", "a", "-r", "D:\\test_wordchain\\test2.txt"};
        main_serve(6, args);
    } catch(runtime_error const &e){
        ASSERT_EQ(0, strcmp("words are more than 100 when chains has circle!", e.what()));
        return;
    }
}

TEST(main_serve, example_result_more_than_20000) {
    try{
        const char* args[] = {"Wordlist.exe", "-n", "D:\\test_wordchain\\test3.txt"};
        main_serve(3, args);
    } catch(runtime_error const &e){
        ASSERT_EQ(0, strcmp("results are more than 20000 chains!", e.what()));
        return;
    }
}

TEST(main_serve, has_circle) {
    try{
        const char* args[] = {"Wordlist.exe", "-w", "D:\\test_wordchain\\test4.txt"};
        main_serve(3, args);
    } catch(logic_error const &e){
        ASSERT_EQ(0, strcmp("the chain has circle without -r!", e.what()));
        return;
    }
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}