
#include<string>
#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<array>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;


#ifdef CORE
#define EXPOSED_FUNCTION extern "C" __attribute__((dllexport))
#else
#define EXPOSED_FUNCTION extern "C" __attribute__((dllimport))
#endif
EXPOSED_FUNCTION
int process(
         char *wordList[], char *result[], int len,
        int type, bool letterSum, char head, char tail, char j
        );
EXPOSED_FUNCTION
int gen_chain_word( char *words[], int len, char *result[],char head, char tail, char j, bool enable_loop);
EXPOSED_FUNCTION
int gen_chains_all(  char *words[], int len, char *result[]);
EXPOSED_FUNCTION
int gen_chain_char( char *words[], int len, char *result[], char head, char tail, char j, bool enable_loop);
EXPOSED_FUNCTION
int hello(char* word,char* out);