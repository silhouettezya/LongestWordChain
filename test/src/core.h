int engine(
        const char* words[],
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

int gen_chains_all(const char* words[], int len, char* result[]);

int gen_chain_word(const char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

int gen_chain_char(const char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop);

