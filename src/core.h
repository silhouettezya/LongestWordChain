#ifdef CORE
#define EXPOSED_FUNCTION extern "C" _declspec(dllexport)
#else
#define EXPOSED_FUNCTION extern "C" _declspec(dllimport)
#endif

int engine(
        const char* words[],
        int len,
        char* result[],
        char head,
        char tail,
        char reject,
        bool enable_loop,
        int type);

EXPOSED_FUNCTION
int engine(
        const char* words[],
        int len,
        char* result[],
        char head,
        char tail,
        char reject,
        bool enable_loop);

EXPOSED_FUNCTION
const char* gui_engine(const char* input, int type, char head, char tail, bool weighted);

EXPOSED_FUNCTION
int gen_chains_all(char* words[], int len, char* result[]);

EXPOSED_FUNCTION
int gen_chain_word(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

EXPOSED_FUNCTION
int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);
