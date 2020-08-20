#include <fstream>
#include <vector>
#include <algorithm>

int n, m, minCost = 0;
bool stopAll = false;

class Graph;
Graph *graph;

std::vector<int> list, size, edgeList;
std::vector<std::vector<int>> matrixOfMST;

class Edge {
private:
    int u, v, cost, num;
public:
    Edge(int u, int v, int cost, int num) :
            u(u), v(v), cost(cost), num(num) {}
    int getU() const {
        return u;
    }
    int getV() const {
        return v;
    }
    int getCost() const {
        return cost;
    }
    int getNum() const {
        return num;
    }
};

class Graph {
private:
    int n;
    std::vector<Edge> edges;
    struct edges_sort {
        inline bool operator() (const Edge& one, const Edge& two) {
            return one.getCost() < two.getCost();
        }
    };
public:
    Graph(int n) : n(n) {}
    void addEdge(const int &u, const int &v, const int &cost, const int &num) {
        edges.push_back(Edge(u, v, cost, num));
    }
    void sort() {
        std::sort(edges.begin(), edges.end(), edges_sort());
    }
    Edge getForI(const int i) const {
        return edges[i];
    }
};

int findSet(int x) {
    if(x == list[x])
        return x;
    list[x] = findSet(list[x]);
    return list[x];
}

void myUnion(int x, int y) {
    x = findSet(x);
    y = findSet(y);
    if (x != y) {
        if (size[x] < size[y])
            std::swap(x, y);
        list[y] = x;
        size[x] += size[y];
    }
}

void findMinCost() {
    matrixOfMST.resize(1);
    for (int i = 0; i <= n; ++i) {
        list.push_back(i);
        size.push_back(1);
    }
    //Алгоритм Краскала
    for (int i = 0; i < m; ++i) {
        if (findSet(graph->getForI(i).getU()) != findSet(graph->getForI(i).getV())) {
            myUnion(graph->getForI(i).getU(), graph->getForI(i).getV());
            minCost += graph->getForI(i).getCost();
            edgeList.push_back(i);
            matrixOfMST[0].push_back(graph->getForI(i).getNum());
        }
    }
    if(matrixOfMST[0].size() != n - 1)
        stopAll = true;
}

bool findMST(std::vector<int> include, std::vector<bool> exclude) {
    int cost = 0, edgeNum = 0;
    edgeList.clear();
    list.clear();
    size.clear();
    for (int i = 0; i <= n; ++i) {
        list.push_back(i);
        size.push_back(1);
    }
    for (int i = 0; i < include.size(); ++i) {
        myUnion(graph->getForI(include[i]).getU(), graph->getForI(include[i]).getV());
        cost += graph->getForI(include[i]).getCost();
        ++edgeNum;
    }
    //Алгоритм Краскала
    for (int i = 0; i < m; ++i) {
        if (!exclude[i] && findSet(graph->getForI(i).getU()) != findSet(graph->getForI(i).getV())) {
            myUnion(graph->getForI(i).getU(), graph->getForI(i).getV());
            cost += graph->getForI(i).getCost();
            edgeList.push_back(i);
            ++edgeNum;
        }
    }
    if (cost == minCost) {
        if(edgeNum == n - 1) {
            std::vector<int> currentSet;
            for (int i = 0; i < include.size(); ++i)
                currentSet.push_back(graph->getForI(include[i]).getNum());
            for (int i = 0; i < edgeList.size(); ++i)
                currentSet.push_back(graph->getForI(edgeList[i]).getNum());
            matrixOfMST.push_back(currentSet);
        }
        return true;
    }
    return false;
}

void enumeration(std::vector<int> include, std::vector<bool> exclude, std::vector<int> currentSet) {
    for (int i = 0; i < currentSet.size(); ++i) {
        exclude[currentSet[i]] = true;
        if (i != 0) {
            exclude[currentSet[i - 1]] = false;
            include.push_back(currentSet[i - 1]);
        }
        if (findMST(include, exclude))
            enumeration(include, exclude, edgeList);
    }
}

void solveProblem() {
    graph->sort();
    fiговndMinCost();
    if(!stopAll) {
        std::vector<int> include;
        std::vector<bool> exclude(m, false);
        enumeration(include, exclude, edgeList);
       }
}

void readInfo() {
    std::ifstream fin("input.txt");

    fin >> n >> m;

    graph = new Graph(n);
    for(int i = 0 ; i < m; ++i) {
        int u, v, cost;
        fin >> u >> v >> cost;
            graph->addEdge(u, v, cost, i);
    }

    fin.close();
}

void writeAnwers() {
    std::ofstream fout("output.txt");

    if(!stopAll) {
        fout << matrixOfMST.size() << '\n';
        for (int i = 0; i < matrixOfMST.size(); ++i) {
            for (int j = 0; j < matrixOfMST[i].size(); ++j)
                fout << matrixOfMST[i][j] + 1 << ' ';
            fout << '\n';
        }
    }
    else
        fout << 0 << '\n';

    delete graph;
    fout.close();
}

int main() {

    readInfo();
    solveProblem();
    writeAnwers();

    return 0;
}
