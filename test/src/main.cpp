#include <iostream>
#include <fstream>
#include <ctype.h>
#include<vector>
#include <string.h>
#include "core.h"

using namespace std;

bool is_filename(string s) {
    int len = s.length();
    return (len > 4 && s[len - 4] == '.' && s[len - 3] == 't' && s[len - 2] == 'x' && s[len - 1] == 't');
}

int main_serve(int argc, const char* argv[]);

//int main(int argc,char* argv[]) {
//    try {
//        main_serve(argc, (const char**) argv);
//    } catch (invalid_argument const& e) {
//        cerr << e.what() << endl;
//    }
//    catch (logic_error const& e) {
//        cerr << e.what() << endl;
//    }
//    catch (runtime_error const& e) {
//        cerr << e.what() << endl;
//    }
//    return 0;
//}

int main_serve(int argc, const char* argv[]) {
    // 处理命令行参数
    bool is_n = false;
    bool is_w = false;
    bool is_c = false;
    bool is_h = false;
    char h_char = '\0';
    bool is_t = false;
    char t_char = '\0';
    bool is_j = false;
    char j_char = '\0';
    bool is_r = false;
    string filename = "";
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        // cout << "arg: " << i << " " << arg << endl;
        if (arg.length() == 2 && arg[0] == '-') {
            if (arg[1] == 'n') {
                if (is_n == false) {
                    is_n = true;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 'w') {
                if (is_w == false) {
                    is_w = true;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 'c') {
                if (is_c == false) {
                    is_c = true;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 'r') {
                if (is_r == false) {
                    is_r = true;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 'h') {
                if (is_h == false) {
                    is_h = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        throw invalid_argument("arg: " + arg + " missing following argument!");
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        h_char = tolower(arg_next[0]);
                    } else {
                        throw invalid_argument("arg: " + arg + " following argument is wrong!");
                    }
                    i++;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 't') {
                if (is_t == false) {
                    is_t = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        throw invalid_argument("arg: " + arg + " missing following argument!");
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        t_char = tolower(arg_next[0]);
                    } else {
                        throw invalid_argument("arg: " + arg + " following argument is wrong!");
                    }
                    i++;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            } else if (arg[1] == 'j') {
                if (is_j == false) {
                    is_j = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        throw invalid_argument("arg: " + arg + " missing following argument!");
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        j_char = tolower(arg_next[0]);
                    } else {
                        throw invalid_argument("arg: " + arg + " following argument is wrong!");
                    }
                    i++;
                } else {
                    throw invalid_argument("arg: " + arg + " repeat!");
                }
            }
        } else {
            if (is_filename(arg)) {
                if (filename != "") {
                    throw invalid_argument("filename is repeat!");
                } else {
                    filename = arg;
                }
            } else {
                throw invalid_argument("this argument is invalid!");
            }

        }
    }
    if (!(is_n || is_w || is_c)) {
        throw invalid_argument("need -n or -w or -c!");
    }
    if (is_n && (is_w || is_c || is_h || is_t || is_j || is_r)) {
        throw invalid_argument("-n can't be used with other arguments!");
    }
    if (is_w && is_c) {
        throw invalid_argument("-w can't be used with -c!");
    }
    if (filename == "") {
        throw invalid_argument("don't get valid filename!");
    }

    // 读文件
    ifstream ifs;

    //打开文件并且判断是否打开成功
    ifs.open(filename,ios::in);
    if (!ifs.is_open()) {
        throw invalid_argument("opening file fail!");
    }
    char c;
    string new_word;
    vector<string> words;
    int is_word = 0;
    while ((c = ifs.get()) != EOF) { // EOF end of file
        if (is_word) {
            if (isalpha(c)) {
                new_word.push_back(tolower(c));
            } else {
                // 去重
                int in_vector = 0;
                for (int i = 0; i < words.size(); i++) {
                    if (words[i] == new_word) {
                        in_vector = 1;
                    }
                }
                if (!in_vector) {
                    words.push_back(new_word);
                    if (words.size() > 10000) {
                        throw runtime_error("words are more than 10000!");
                    } else if (is_r && words.size() > 100) {
                        throw runtime_error("words are more than 100 when chains has circle!");
                    }
                }
                is_word = 0;
            }
        } else {
            if (isalpha(c)) {
                is_word = 1;
                new_word = "";
                new_word.push_back(tolower(c));
            }
        }
    }
    ifs.close();
    if (is_word) {
        int in_vector = 0;
        for (int i = 0; i < words.size(); i++) {
            if (words[i] == new_word) {
                in_vector = 1;
            }
        }
        if (!in_vector) {
            words.push_back(new_word);
            if (words.size() > 10000) {
                throw runtime_error("words are more than 10000!");
            } else if (is_r && words.size() > 100) {
                throw runtime_error("words are more than 100 when chains has circle!");
            }
        }
    }

    char* input_word[10001];
    char* result[20001];
    int num_result;

    for (int i = 0; i < words.size(); i++) {
        cout << words[i] << endl;
        int len = words[i].length();
        input_word[i] = (char *)malloc((len+1)*sizeof(char));
        words[i].copy(input_word[i],len,0);
        *(input_word[i] + len) = '\0';
    }

    // cout << "finish read words" << endl;

    ofstream outfile;
    outfile.open("solution.txt");
    if (is_n) {
        num_result = gen_chains_all((const char**)input_word, words.size(), result);
        if (num_result > 20000) {
            throw runtime_error("results are more than 20000 chains!");
        }
        cout << num_result << endl;
        outfile << num_result << endl;
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    } else if (is_w) {
        num_result = gen_chain_word((const char**)input_word, words.size(), result, h_char, t_char, j_char, is_r);
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    } else if (is_c) {
        num_result = gen_chain_char((const char**)input_word, words.size(), result, h_char, t_char, j_char, is_r);
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    }


    return 0;
}
