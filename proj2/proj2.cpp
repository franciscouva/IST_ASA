#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

class Graph {
public:
    int V;
    vector<vector<int>> adj;

    Graph(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int u, int v) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            adj[u].push_back(v);
        }
    }

    void tarjanDFS(int start, vector<int>& disc, vector<int>& low, stack<int>& st, vector<bool>& onStack, int& time, vector<vector<int>>& scc) {
        struct Frame {
            int v;
            size_t i;
        };

        stack<Frame> frames;
        frames.push({start, 0});
        disc[start] = low[start] = time++;
        st.push(start);
        onStack[start] = true;

        while (!frames.empty()) {
            Frame& top = frames.top();
            if (top.i < adj[top.v].size()) {
                int w = adj[top.v][top.i++];
                if (disc[w] == -1) {
                    disc[w] = low[w] = time++;
                    st.push(w);
                    onStack[w] = true;
                    frames.push({w, 0});
                } else if (onStack[w]) {
                    low[top.v] = min(low[top.v], disc[w]);
                }
            } else {
                if (low[top.v] == disc[top.v]) {
                    vector<int> currentSCC;
                    while (true) {
                        int v = st.top();
                        st.pop();
                        onStack[v] = false;
                        currentSCC.push_back(v);
                        if (v == top.v) break;
                    }
                    scc.emplace_back(currentSCC);
                }
                if (!frames.empty()) {
                    int v = frames.top().v;
                    frames.pop();
                    if (!frames.empty()) {
                        low[frames.top().v] = min(low[frames.top().v], low[v]);
                    }
                }
            }
        }
    }

    vector<vector<int>> tarjanSCC() {
        vector<int> disc(V, -1), low(V, -1);
        vector<bool> onStack(V, false);
        stack<int> st;
        vector<vector<int>> scc;
        int time = 0;

        for (int i = 0; i < V; i++) {
            if (disc[i] == -1) {
                tarjanDFS(i, disc, low, st, onStack, time, scc);
            }
        }

        return scc;
    }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        graph.addEdge(x - 1, y - 1);
    }

    vector<vector<int>> scc = graph.tarjanSCC();

    // Build a new DAG using the SCCs
    int numSCCs = scc.size();
    vector<vector<int>> dagGraph(numSCCs);

    unordered_map<int, int> nodeToSCC;
    for (int i = 0; i < numSCCs; i++) {
        for (const auto& node : scc[i]) {
            nodeToSCC[node] = i;
        }
    }

    for (int u = 0; u < graph.V; ++u) {
        for (int v : graph.adj[u]) {
            if (nodeToSCC.find(u) != nodeToSCC.end() && nodeToSCC.find(v) != nodeToSCC.end()) {
                int uSCC = nodeToSCC[u];
                int vSCC = nodeToSCC[v];
                if (uSCC != vSCC) {
                    dagGraph[uSCC].push_back(vSCC);
                }
            }
        }
    }

    // Find the length of the longest path in the DAG
    int maxLength = 0;
    vector<int> dist(numSCCs, 0);
    vector<int> indegree(numSCCs, 0);
    for (const auto& adj : dagGraph) {
        for (int v : adj) {
            if (v >= 0 && v < numSCCs) {
                indegree[v]++;
            }
        }
    }

    queue<int> q;
    for (int i = 0; i < numSCCs; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : dagGraph[u]) {
            if (v >= 0 && v < numSCCs) {
                if (--indegree[v] == 0) {
                    q.push(v);
                }
                dist[v] = max(dist[v], dist[u] + 1);
            }
        }
    } 

    if (!dist.empty()) {
        maxLength = *max_element(dist.begin(), dist.end());
    }

    cout << maxLength << endl;

    return 0;
}