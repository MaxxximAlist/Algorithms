#include <fstream>
#include <vector>

using namespace std;

class Edge {
private:
    long long u, v, cost, flow;
    unsigned long long father;
public:
    Edge(long long u, long long v, long long cost, long long flow, unsigned long long father) :
        u(u), v(v), cost(cost), flow(flow), father(father) {}
    long long getU() const {
        return u;
    }
    long long getV() const {
        return v;
    }
    long long getCost() const {
        return cost;
    }
    long long getFlow() const {
        return flow;
    }
    unsigned long long getFather() const {
        return father;
    }
    void addToFlow(long long add) {
        flow += add;
    }
};

void addEdge(vector<vector<Edge>> &g, long long u, long long v, long long p, long long s) {
    Edge edgeOne(v, p, s, 0, g[v].size()), edgeTwo(u, 0, -s, 0, g[u].size());
    g[u].push_back(edgeOne);
    g[v].push_back(edgeTwo);
}

int main() {

    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    long long n, m, a, b;

    fin >> n >> m >> a >> b;
    --a;
    --b;

    vector<vector<Edge>> graph(n);

    for(long long i = 0; i < m; i++) {
        long long u, v, p, s;
        fin >> u >> v >> p >> s;
        --u;
        --v;
        addEdge(graph, u, v, p, s);
        addEdge(graph, v, u, p, s);
    }

    long long flow = 0, cost = 0;

    while (true) {

        vector<long long> has(n, 0), distance(n, INT64_MAX), q(n), parent(n);
        vector<unsigned long long> fathers (n);

        long long qh = 0, qt = 0;
        q[qt++] = a;
        distance[a] = 0;

        while (qh != qt) {

            long long v = q[qh++];
            has[v] = 2;

            if (qh == n)
                qh = 0;

            for (long long i = 0; i < graph[v].size(); ++i) {
                Edge r = graph[v][i];
                if (r.getFlow() < r.getV() && distance[v] + r.getCost() < distance[r.getU()]) {

                    distance[r.getU()] = distance[v] + r.getCost();
                    if (has[r.getU()] == 0) {
                        q[qt++] = r.getU();
                        if (qt == n)
                            qt = 0;
                    }
                    else if (has[r.getU()] == 2) {
                        if (--qh == -1)
                            qh = n-1;
                        q[qh] = r.getU();
                    }
                    has[r.getU()] = 1;
                    parent[r.getU()] = v;
                    fathers[r.getU()] = i;
                }
            }
        }

        if (distance[b] == INT64_MAX)
            break;

        long long cMin = INT64_MAX;

        for (long long i = b; i != a; i = parent[i]) {
            long long pv = parent[i];
            unsigned long long pr = fathers[i];
            cMin = min (cMin, graph[pv][pr].getV() - graph[pv][pr].getFlow());
        }

        for (long long i = b; i != a; i = parent[i]) {
            long long pv = parent[i];
            unsigned long long pr = fathers[i], r = graph[pv][pr].getFather();
            graph[pv][pr].addToFlow(cMin);
            graph[i][r].addToFlow(-cMin);
            cost += graph[pv][pr].getCost() * cMin;
        }
        flow += cMin;
    }

    fout << flow << endl << cost;

    fin.close();
    fout.close();
}