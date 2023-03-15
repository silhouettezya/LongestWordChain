int engine(
        char* words[],
        int len,
        char* result[],
        char head,
        char tail,
        char reject,
        bool enable_loop,
        int type);

/*int engine(
        const char* words[],
        int len,
        char* result[],
        char head,
        char tail,
        char reject,
        bool enable_loop);*/

//const char* gui_engine(const char* input, int type, char head, char tail, bool weighted);

int gen_chains_all(char* words[], int len, char* result[]);

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

