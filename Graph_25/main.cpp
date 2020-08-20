#include <fstream>
#include "vector"
#include "math.h"

int n, m;

std::vector<int> *graph;
std::vector<int> matchs;
std::vector<bool> used;

double distance(int x1, int y1, int x2, int y2) {
    return pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
}

bool checkCond(int* p1, int* p2, int* p0) {
    return (distance(p1[0], p1[1], p0[0], p0[1]) +
    distance(p0[0], p0[1], p2[0], p2[1])) <= 2 * (distance(p1[0], p1[1], p2[0], p2[1]));
}

bool kun(int v) {
    if (used[v])
        return false;
    used[v] = true;
    for (int i = 0; i < graph[v].size(); ++i) {
        int u = graph[v][i];
        if (matchs[u] == -1 || kun(matchs[u])) {
            matchs[u] = v;
            return true;
        }
    }
    return false;
}

int main() {

    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    fin >> n >> m;

    int **way = new int*[n], **places = new int*[m];
    for(int i = 0; i < n; ++i)
        way[i] = new int[2];
    for(int i = 0; i < m; ++i)
        places[i] = new int[2];


    for(int i = 0; i < n; ++i)
        fin >> way[i][0] >> way[i][1];

    for(int i = 0; i < m; ++i)
        fin >> places[i][0] >> places[i][1];


    --n;
    graph = new std::vector<int>[n];
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < m; ++j)
            if(checkCond(way[i], way[i + 1], places[j]))
                graph[i].push_back(j);


    int count = 0;

    for(int i = 0; i < m; ++i)
        matchs.push_back(-1);

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j)
            used.push_back(false);
        if(kun(i))
            ++count;
        used.clear();
    }

    fout << n + 1 + count << ' ' << count;

    fin.close();
    fout.close();

    return 0;
}
