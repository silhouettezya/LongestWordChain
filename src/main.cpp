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

int main(int argc,char* argv[]) {
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
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 'w') {
                if (is_w == false) {
                    is_w = true;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 'c') {
                if (is_c == false) {
                    is_c = true;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 'r') {
                if (is_r == false) {
                    is_r = true;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 'h') {
                if (is_h == false) {
                    is_h = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        cout << "参数：" << arg << "后续参数缺失" << endl;
                        return 0;
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        h_char = tolower(arg_next[0]);
                    } else {
                        cout << "参数：" << arg << "后续参数错误" << endl;
                        return 0;
                    }
                    i++;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 't') {
                if (is_t == false) {
                    is_t = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        cout << "参数：" << arg << "后续参数缺失" << endl;
                        return 0;
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        t_char = tolower(arg_next[0]);
                    } else {
                        cout << "参数：" << arg << "后续参数错误" << endl;
                        return 0;
                    }
                    i++;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            } else if (arg[1] == 'j') {
                if (is_j == false) {
                    is_j = true;
                    int i_next = i + 1;
                    if (i_next == argc) {
                        cout << "参数：" << arg << "后续参数缺失" << endl;
                        return 0;
                    }
                    string arg_next = argv[i_next];
                    if (arg_next.length() == 1 && isalpha(arg_next[0])) {
                        j_char = tolower(arg_next[0]);
                    } else {
                        cout << "参数：" << arg << "后续参数错误" << endl;
                        return 0;
                    }
                    i++;
                } else {
                    cout << "参数：" << arg << "重复" << endl;
                    return 0;
                }
            }
        } else {
            if (is_filename(arg)) {
                if (filename != "") {
                    cout << "文件名重复" << endl;
                    return 0;
                } else {
                    filename = arg;
                }
            } else {
                cout << "输入参数错误" << endl;
                return 0;
            }

        }
    }
    if (!(is_n || is_w || is_c)) {
        cout << "功能参数缺失" << endl;
        return 0;
    }
    if (is_n && (is_w || is_c || is_h || is_t || is_j || is_r)) {
        cout << "-n不能与其它参数同时使用" << endl;
        return 0;
    }
    if (is_w && is_c) {
        cout << "-w不能与-c同时使用" << endl;
        return 0;
    }
    if (filename == "") {
        cout << "没有获取到合法的文件名" << endl;
    }

    // 读文件
    ifstream ifs;

    //打开文件并且判断是否打开成功
    ifs.open(filename,ios::in);
    if (!ifs.is_open()) {
        cout << "文件打开失败" << endl;
        return 0;
    }
    char c;
    string word_table[10002];
    vector<string> words;
    int word_num = 0;
    int is_word = 0;
    while ((c = ifs.get()) != EOF) { // EOF end of file
        if (is_word) {
            if (isalpha(c)) {
                word_table[word_num].push_back(tolower(c));
            } else {
                // 去重
                int in_vector = 0;
                for (int i = 0; i < words.size(); i++) {
                    if (words[i] == word_table[word_num]) {
                        in_vector = 1;
                    }
                }
                if (!in_vector) {
                    words.push_back(word_table[word_num]);
                }
                is_word = 0;
            }
        } else {
            if (isalpha(c)) {
                is_word = 1;
                word_num++;
                if (word_num >= 10000) {
                    cout << "单词数不能大于10000" << endl;
                    return 0; // 单词数不能大于10000
                }
                word_table[word_num].push_back(tolower(c));
            }
        }
    }
    ifs.close();
    if (is_word) {
        int in_vector = 0;
        for (int i = 0; i < words.size(); i++) {
            if (words[i] == word_table[word_num]) {
                in_vector = 1;
            }
        }
        if (!in_vector) {
            words.push_back(word_table[word_num]);
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

    cout << "finish read words" << endl;

    ofstream outfile;
    outfile.open("solution.txt");
    if (is_n) {
        num_result = gen_chains_all(input_word, words.size(), result);
        cout << num_result << endl;
        outfile << num_result << endl;
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    } else if (is_w) {
        num_result = gen_chain_word(input_word, words.size(), result, h_char, t_char, j_char, is_r);
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    } else if (is_c) {
        num_result = gen_chain_char(input_word, words.size(), result, h_char, t_char, j_char, is_r);
        for (int i = 0; i < num_result; i++) {
            cout << result[i] << endl;
            outfile << result[i] << endl;
        }
    }


    return 0;
}
