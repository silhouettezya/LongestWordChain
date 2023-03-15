//
// Created by hp on 2023/3/8.
//
//#define CORE
#include "core.h"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define MAX_VERTEX 10001

struct Graph {
    int degree;
    bool hasCircle;
    bool matrix[MAX_VERTEX][MAX_VERTEX];
    bool noUse[MAX_VERTEX];
    int inDegree[MAX_VERTEX];
};
typedef struct Graph *Gptr;
vector<string> strings;

void init_words(char* words[], int len){
    strings.clear();
    for(int i=0;i<len;i++){
        string S = words[i];
        strings.push_back(S);
    }
}

int inDegree[MAX_VERTEX];   // used in topo_check()
Gptr create_graph(vector<string> Strings, int n) {
    Gptr graph = (Gptr) malloc(sizeof(struct Graph));
    graph->degree = n;
    graph->hasCircle = false;

    vector<int> first[27];
    vector<int> last[27];
    int i = 0;
    for (i = 0; i < graph->degree; i++) {
        inDegree[i] = 0;
        graph->inDegree[i] = 0;
    }
    for (i = 0; i < Strings.size(); i++) {
        string cur = Strings[i];
        graph->noUse[i] = false;
        int f = cur[0] - 'a';
        int l = cur[cur.length() - 1] - 'a';
        for (auto next: first[l]) {
            graph->matrix[i][next] = true;
            inDegree[next] = inDegree[next] + 1;
            graph->inDegree[next] = graph->inDegree[next] + 1;
            cout << "add " << Strings[i] << " -> " << Strings[next] << endl;
        }
        for (auto lastV: last[f]) {
            graph->matrix[lastV][i] = true;
            inDegree[i] = inDegree[i] + 1;
            graph->inDegree[i] = graph->inDegree[i] + 1;
            cout << "add " << Strings[lastV] << " -> " << Strings[i] << endl;
        }
        first[f].insert(first[f].end(), i);
        last[l].insert(last[l].end(), i);
    }
    return graph;
}

bool visited_topo[MAX_VERTEX] = {false};
bool visited_head[MAX_VERTEX] = {false};

int *topo_check(Gptr graph, char head, char tail, char nhead, bool enable_loop) {
    if (head != '\0') {
        // delete useless word:
        queue<int> q;
        for (int i = 0; i < graph->degree; i++) {
            if (inDegree[i] == 0 && strings[i][0] != head) {
                q.push(i);
                visited_head[i] = true;
            }
        }
        while (!q.empty()) {
            int w = q.front();
            q.pop();
            graph->noUse[w] = true;
            for (int j = 0; j < graph->degree; j++) {
                if (!visited_head[j] && graph->matrix[w][j]) inDegree[j] = inDegree[j] - 1;
                if (!visited_head[j] && inDegree[j] == 0 && strings[j][0] != head) {
                    q.push(j);
                    visited_head[j] = true;
                }
            }
        }
    }
    // refresh inDegree
    for (int i = 0; i < graph->degree; i++) {
        inDegree[i] = graph->inDegree[i];
    }
    if (nhead != '\0') {
        // delete useless word:
        for (int i = 0; i < graph->degree; i++) {
            if (strings[i][0] == nhead) {
                graph->noUse[i] = true;
            }
        }
    }

    /*cout << "noUse : ";
    for (int i = 0; i < graph->degree; i++) {
        if (graph->noUse[i]) {
            cout << strings[i] + " ";
        }
    }
    cout << "\nend!\n";*/

    queue<int> q;
    int *ret = (int *) malloc(MAX_VERTEX * sizeof(int));
    int retIndex = 0;
    for (int i = 0; i < graph->degree; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
            visited_topo[i] = true;
        }
    }
    while (!q.empty()) {
        int w = q.front();
        q.pop();
        ret[retIndex++] = w;
        for (int j = 0; j < graph->degree; j++) {
            if (!visited_topo[j] && graph->matrix[w][j]) inDegree[j] = inDegree[j] - 1;
            if (!visited_topo[j] && inDegree[j] == 0) {
                q.push(j);
                visited_topo[j] = true;
            }
        }
    }
    if (retIndex != graph->degree) {
        cout << "has circle! ";
        graph->hasCircle = true;
        ret[0] = -1;
        return ret;
    } else {
        /*for (int i = 0; i < graph->degree; i++) {
            cout << strings[ret[i]] << " ";
        }*/
    }
    return ret;
}

int getMaxWordChain(Gptr graph, const int *topo, char **results,
                    char head, char tail, char nhead, bool enable_loop) {
    // used when not has Circle!
    //initial:
    int dp[MAX_VERTEX] = {0};
    int rec[MAX_VERTEX] = {0};
    int i = 0;
    for (i = 0; i < graph->degree; i++) {
        if (graph->noUse[i]) dp[i] = -1;
        else dp[i] = 1;
    }
    for (int k = 0; k < graph->degree; k++) {
        i = topo[k];
        if (graph->noUse[i]) {
            continue;
        }
        int max = dp[i];
        rec[i] = i;
        for (int j = 0; j < graph->degree; j++) {
            if (!graph->noUse[j] && graph->matrix[j][i] && dp[j] + 1 >= max) {
                max = dp[j] + 1;
                rec[i] = j;
            }
        }
        dp[i] = max;
    }
    int max = 1;
    int maxIndex = 0;
    if (tail == '\0') {
        for (i = 0; i < graph->degree; i++) {
            if (dp[i] > max) {
                max = dp[i];
                maxIndex = i;
            }
        }
    } else {
        for (i = 0; i < graph->degree; i++) {
            if (strings[i][(strings[i].length() - 1)] == tail && dp[i] > max) {
                max = dp[i];
                maxIndex = i;
            }
        }
    }
    cout << "max: " << max << " \n";
    cout << "chain: ";
    int chain[max];
    int chainIndex = max - 1;
    int curNode = maxIndex;
    while (rec[curNode] != curNode) {
        chain[chainIndex] = curNode;
        curNode = rec[curNode];
        chainIndex--;
    }
    chain[chainIndex] = curNode;
    for (int j = 0; j < max; j++) {
        cout << strings[chain[j]] << " ";
        int len = strings[chain[j]].length();
        results[j] = (char *)malloc((len+1)*sizeof(char));
        strings[chain[j]].copy(results[j],len,0);
        *(results[j] + len)='\0';
    }
    return max;
}

int getMaxCharChain(Gptr graph, const int *topo, char **results,
                    char head, char tail, char nhead, bool enable_loop) {
    //initial:
    int dp[MAX_VERTEX] = {0};
    int rec[MAX_VERTEX] = {0};
    int i = 0;
    for (i = 0; i < graph->degree; i++) {
        if (graph->noUse[i]) dp[i] = -1;
        else dp[i] = strings[i].length();
    }
    for (int k = 0; k < graph->degree; k++) {
        i = topo[k];
        if (graph->noUse[i]) {
            continue;
        }
        int max = dp[i];
        rec[i] = i;
        for (int j = 0; j < graph->degree; j++) {
            if (!graph->noUse[j] && graph->matrix[j][i] && dp[j] + strings[i].length() >= max) {
                max = dp[j] + strings[i].length();
                rec[i] = j;
            }
        }
        dp[i] = max;
    }
    int max = 1;
    int maxIndex = 0;
    if (tail == '\0') {
        for (i = 0; i < graph->degree; i++) {
            if (dp[i] > max) {
                max = dp[i];
                maxIndex = i;
            }
        }
    } else {
        for (i = 0; i < graph->degree; i++) {
            if (strings[i][(strings[i].length() - 1)] == tail && dp[i] > max) {
                max = dp[i];
                maxIndex = i;
            }
        }
    }
    cout << "max: " << max << " \n";
    cout << "chain: ";
    int chain[MAX_VERTEX];
    int chainIndex = 0;
    int curNode = maxIndex;
    while (rec[curNode] != curNode) {
        chain[chainIndex] = curNode;
        curNode = rec[curNode];
        chainIndex++;
    }
    chain[chainIndex] = curNode;
    while (chainIndex >= 0) {
        cout << strings[chain[chainIndex]] << " ";
        int len = strings[chain[chainIndex]].length();
        results[chainIndex] = (char *)malloc((len+1)*sizeof(char));
        strings[chain[chainIndex]].copy(results[chainIndex],len,0);
        *(results[chainIndex] + len)='\0';
        chainIndex--;
    }
    return max;
}

bool visited_dfs[MAX_VERTEX] = {false};
bool visited_tarjan[MAX_VERTEX] = {false};
bool inStack_tarjan[MAX_VERTEX] = {false};
int color_tarjan[MAX_VERTEX] = {false};
int colorNum = 0;
int dfn[MAX_VERTEX];
int low[MAX_VERTEX];
int Index = 0;
int Stack[MAX_VERTEX];
int top = 0;

void tarjan(int u, Gptr graph) {
    dfn[u] = low[u] = ++Index;
    Stack[++top] = u;
    visited_tarjan[u] = inStack_tarjan[u] = true;
    for (int j = 0; j < graph->degree; j++) {
        if (graph->matrix[u][j] && !visited_tarjan[j]) {
            tarjan(j, graph);
            low[u] = min(low[u], low[j]);
        } else if (inStack_tarjan[j]) {
            low[u] = min(low[u], dfn[j]);
        }
    }
    int v;
    if (dfn[u] == low[u]) {
        do {
            v = Stack[top--];
            inStack_tarjan[v] = false;
            color_tarjan[v] = colorNum;
        } while (low[v] != dfn[v]);
        colorNum++;
    }
}

void DFSWord(Gptr graph, int v, int *cur, int *curlen, int *max, int *maxlen, char nhead, char tail) {
    visited_dfs[v] = true;
    cur[*curlen] = v;
    *curlen = *curlen + 1;
    bool hasTail = tail!='\0';
    if (*curlen > *maxlen && (!hasTail || strings[v][strings[v].length()-1] == tail)) {
        *maxlen = *curlen;
        for (int i = 0; i < *maxlen; i++) {
            max[i] = cur[i];
        }
    }
    for (int j = 0; j < graph->degree; j++) {
        if (graph->matrix[v][j] && !visited_dfs[j] && ((nhead=='\0')||strings[j][0]!=nhead)) {
            DFSWord(graph, j, cur, curlen, max, maxlen, nhead, tail);
        }
    }
    visited_dfs[v] = false;
    *curlen = *curlen - 1;
}
void DFSChar(Gptr graph, int v, int *cur, int *curlen, int *curWeight, int *max, int *maxlen, int *maxWeight, char nhead, char tail) {
    visited_dfs[v] = true;
    cur[*curlen] = v;
    *curlen = *curlen + 1;
    *curWeight = *curWeight + strings[v].length();
    bool hasTail = tail!='\0';
    if (*curWeight > *maxWeight && (!hasTail || strings[v][strings[v].length()-1] == tail)) {
        *maxlen = *curlen;
        *maxWeight = *curWeight;
        for (int i = 0; i < *maxlen; i++) {
            max[i] = cur[i];
        }
    }
    for (int j = 0; j < graph->degree; j++) {
        if (graph->matrix[v][j] && !visited_dfs[j] && ((nhead=='\0')||strings[j][0]!=nhead)) {
            DFSChar(graph, j, cur, curlen, curWeight, max, maxlen, maxWeight, nhead, tail);
        }
    }
    visited_dfs[v] = false;
    *curlen = *curlen - 1;
    *curWeight = *curWeight - strings[v].length();
}

int getMaxWordChainCircle(Gptr graph, int *topo, char **results,
                          char head, char tail, char nhead, bool enable_loop) {
    int maxlen = 0, curlen = 0;
    int max[MAX_VERTEX], cur[MAX_VERTEX];
    for (int i = 0; i < graph->degree; i++) {
        if(head!='\0' && strings[i][0] != head ||
            (nhead!='\0' && strings[i][0] == nhead)){
            continue;
        }
        DFSWord(graph, i, cur, &curlen, max, &maxlen, nhead, tail);
    }
    cout << "chain:\n";
    for (int i = 0; i < maxlen; i++) {
        cout << strings[max[i]] << " ";
        int len = strings[max[i]].length();
        results[i] = (char *)malloc((len+1)*sizeof(char));
        strings[max[i]].copy(results[i],len,0);
        *(results[i] + len)='\0';
    }
    return maxlen;
}

int getMaxCharChainCircle(Gptr graph, int *topo, char **results,
                          char head, char tail, char nhead, bool enable_loop) {
    int maxlen = 0, curlen = 0;
    int maxWeight = 0, curWeight = 0;
    int max[MAX_VERTEX], cur[MAX_VERTEX];
    for (int i = 0; i < graph->degree; i++) {
        if(head!='\0' && strings[i][0] != head ||
           (nhead!='\0' && strings[i][0] == nhead)){
            continue;
        }
        DFSChar(graph, i, cur, &curlen, &curWeight, max, &maxlen, &maxWeight, nhead, tail);
    }
    cout << "chain:\n";
    for (int i = 0; i < maxlen; i++) {
        cout << strings[max[i]] << " ";
        int len = strings[max[i]].length();
        results[i] = (char *)malloc((len+1)*sizeof(char));
        strings[max[i]].copy(results[i],len,0);
        *(results[i] + len)='\0';
    }
    return maxlen;
}

int topAll = -1;
int chains = 0;
int rec[10001]; // 链栈
string print_chain();
void DFSAll(Gptr graph, int cur, char** results) {
    visited_dfs[cur] = true;
    rec[++topAll] = cur;
    if (topAll > 0) {
        string s = print_chain();
        int len = s.length();
        results[chains] = (char *)malloc((len+1)*sizeof(char));
        s.copy(results[chains],len,0);
        *(results[chains] + len)='\0';
        chains++;
    }
    for (int i = 0; i < graph->degree; i++) {
        if (graph->matrix[cur][i] && !visited_dfs[i]) {
            DFSAll(graph, i, results);
        }
    }
    topAll--;
    visited_dfs[cur] = false;
}

int get_all_chain(Gptr graph, char** results) {
    for (int i = 0; i < graph->degree; i++) {
        DFSAll(graph, i, results);
    }
    return chains;
}

string print_chain() {
    string s = "";
    for (int i = 0; i < topAll; i++) {
        s = s + strings[rec[i]] + " ";
    }
    s = s + strings[rec[topAll]];
    //cout << s << endl;
    return s;
}

/*
 * type = 0; refers to gen_chain_all;
 * type = 1; refers to gen_chain_word;
 * type = 2; refers to gen_chain_char;
 */
int engine(
        char* words[],
        int len,
        char* result[],
        char head,
        char tail,
        char reject,
        bool enable_loop,
        int type){
    init_words(words, len);
    Gptr graph = create_graph(strings, len);
    int* topoL = topo_check(graph, head, tail, reject, enable_loop);
    if(type == 0){
        if(graph->hasCircle){
            // TODO hasCirCle!!
        }else{
            return get_all_chain(graph, result);
        }
    }
    else if(type == 1){
        if(!enable_loop && graph->hasCircle){
            // TODO hasCirCle!!
        }else{
            if(!graph->hasCircle){
                return getMaxWordChain(graph, topoL, result, head, tail, reject, enable_loop);
            }else{
                return getMaxWordChainCircle(graph, topoL, result, head, tail, reject, enable_loop);
            }
        }
    }else if(type == 2){
        if(!enable_loop && graph->hasCircle){
            // TODO hasCirCle!!
        }else{
            if(!graph->hasCircle){
                return getMaxCharChain(graph, topoL, result, head, tail, reject, enable_loop);
            }else{
                return getMaxCharChainCircle(graph, topoL, result, head, tail, reject, enable_loop);
            }
        }
    }
    return 0;
}

int gen_chains_all(char* words[], int len, char* result[]){
    return engine(words, len, result, '\0', '\0', '\0', false, 0);
}

int gen_chain_word(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop){
    return engine(words, len, result, head, tail, reject, enable_loop, 1);
}

int gen_chain_char(char* words[], int len, char* result[], char head, char tail, char reject, bool enable_loop){
    return engine(words, len, result, head, tail, reject, enable_loop, 2);
}


/*int main() {
    int n;
    cin >> n;
    const char* words[MAX_VERTEX];
    char* result[MAX_VERTEX];
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        words[i] = s.data();
    }
    char head, tail, reject;
    bool enable_loop = false;
    int type = 0;
    int ret = engine(words, n, result, head, tail, reject, enable_loop, type);
    cout << "ret: " << ret << "\n";
    return 0;
}*/

