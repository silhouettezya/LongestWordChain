//
// Created by hp on 2023/3/8.
//
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

#define MAX_VERTEX 10001

struct Graph {
    int degree;
    bool hasCircle;
    bool matrix[MAX_VERTEX][MAX_VERTEX];
};
typedef struct Graph *Gptr;
vector<string> strings;

int inDegree[MAX_VERTEX];   // used in topo_check()
Gptr create_graph(vector<string> strings) {
    Gptr graph = (Gptr) malloc(sizeof(struct Graph));
    graph->degree = strings.size();
    graph->hasCircle = false;

    vector<int> first[27];
    vector<int> last[27];
    int i = 0;
    for (i = 0; i < strings.size(); i++) {
        string cur = strings[i];
        inDegree[i] = 0;
        int f = cur[0] - 'a';
        int l = cur[cur.length() - 1] - 'a';
        for (auto next: first[l]) {
            graph->matrix[i][next] = true;
            cout << "add " << strings[i] << " -> " << strings[next] << "\n";
        }
        for (auto last: last[f]) {
            graph->matrix[last][i] = true;
            inDegree[i] = inDegree[i] + 1;;
            cout << "add " << strings[last] << " -> " << strings[i] << "\n";
        }
        first[f].insert(first[f].end(), i);
        last[l].insert(last[l].end(), i);
    }
    return graph;
}

bool visited_topo[MAX_VERTEX] = {false};
int *topo_check(Gptr graph) {
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
        return NULL;
    } else {
        for (int i = 0; i < graph->degree; i++) {
            cout << strings[i] << " ";
        }
    }
    return ret;
}

int getMaxWordChain(Gptr graph, int *topo, char **results) {
    //initial:
    int dp[MAX_VERTEX] = {0};
    int rec[MAX_VERTEX] = {NULL};
    int i = 0;
    for (i = 0; i < graph->degree; i++) {
        dp[i] = 1;
    }
    for (int k = 0; k < graph->degree; k++) {
        i = topo[k];
        int max = dp[i];
        rec[i] = i;
        for (int j = 0; j < graph->degree; j++) {
            if (graph->matrix[j][i] && dp[j] + 1 >= max) {
                max = dp[j] + 1;
                rec[i] = j;
            }
        }
        dp[i] = max;
    }
    int max = 1;
    int maxIndex = 0;
    for (i = 0; i < graph->degree; i++) {
        if (dp[i] > max) {
            max = dp[i];
            maxIndex = i;
        }
    }
    cout << "max: " << max << " \n";
    cout << "chain: ";
    int chain[max];
    int chainIndex = max-1;
    int curNode = maxIndex;
    while (rec[curNode] != curNode) {
        chain[chainIndex] = curNode;
        curNode = rec[curNode];
        chainIndex--;
    }
    for (int i = 0; i<max; i++) {
        cout << strings[chain[i]] << " ";
    }
    return max;
}

// zya int gen_chains_all(char* words[], int len, char* result[]);
bool visited_dfs[10001] = {false};
string result[10001];
int top = -1;
int chains = 0;
void dfs(Gptr graph, int cur);
void print_all_chain();
int get_all_wordchain(Gptr graph) {
    for (int i = 0; i < graph->degree; i++) {
        dfs(graph, i);
    }
    return chains;
}

void dfs(Gptr graph, int cur) {
    visited_dfs[cur] = true;
    result[++top] = strings[cur];
    if (top > 0) {
        chains++;
        print_all_chain();
    }
    for (int i = 0; i < graph->degree; i++) {
        if (graph->matrix[cur][i] == true && visited_dfs[i] == false) {
            dfs(graph, i);
        }
    }
    top--;
    visited_dfs[cur] = false;
}

void print_all_chain() {
    cout << "chains" << chains << " ";
    for (int i = 0; i < top; i++) {
        cout << result[i] << " ";
    }
    cout << result[top] << endl;
}




int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        strings.insert(strings.end(), s);
    }
    // char* [] -> vector<string>
    /*
     *
     */
    Gptr graph = create_graph(strings);
    int * topo = topo_check(graph);
    int max = getMaxWordChain(graph, topo, NULL);
    // zya get_all_wordchain test
    int all_chains = get_all_wordchain(graph);
    return 0;
}
