//
// Created by polina on 24.12.22.
//

#include <vector>
#include <map>
#include <queue>
#include <iostream>
#include "vertices.h"

using namespace std;

typedef pair<int, int> pair_entry;

const int NV = 27;
const int start_node = 22;
const int target_node = 5;

struct graph {
    vector<vector<int>> adjacency_list;
    vector<int> dfs_prev;
    vector<int> dls_prev;
    vector<int> ids_prev;
};


//struct inf_path {
//    vector<int> distances;
//    int length;
//};

struct weight_graph {
    bool is_bfs_heuristic = false;
    vector<int> distances;
    vector<vector<pair_entry>> adj_weight_list;
};

void fill_adjacency_list(struct graph* gr) {
    map<string, int> vg = { {vertices[0], 0} };
    for (int i = 1; i < vertices.size(); i++) {
        if (!vg.contains(vertices[i])) vg[vertices[i]] = vg.size();
    }
    gr->adjacency_list.resize(NV);
    for (auto & edge : graph_list) {
        int begin = vg[edge[0]];
        int end = vg[edge[1]];
        gr->adjacency_list[begin].push_back(end);
        gr->adjacency_list[end].push_back(begin);
    }
}

void print_adjacency_list(struct graph* gr) {
    for (int j = 0; j < gr->adjacency_list.size(); j++) {
        cout << j << " vertice: ";
        for (int l : gr->adjacency_list[j]) {
            cout << l << ' ';
        }
        cout << endl;
    }
}

void bfs(struct graph* gr) {
    cout << "---BFS---" << endl;
    int node = start_node;
    queue<int> q;
    vector<bool> used;
    vector<int> dist;
    used.resize(NV, false);
    dist.resize(NV, 0);

    used[node] = true;
    q.push(node);

    while (!q.empty()) {
        node = q.front();
        if (node == target_node) {
            cout << "target node ";
            cout << node << ", dist = " << dist[node];
            cout << endl;
            break;
        }
        q.pop();

        for (int& v: gr->adjacency_list[node]) {
            if (!used[v]) {
                used[v] = true;
                dist[v] = dist[node] + 1;
                q.push(v);
            }
        }
    }
}

int depth = 0;
int shortest_depth = INT32_MAX;
vector<int> visited(NV);
void dfs(int node, int t, struct graph* g) {
    depth++;
    if (depth > shortest_depth) return;

    if (node == t) {
        shortest_depth = depth;
        return;
    }

    visited[node] = true;
    vector<vector<int>> adj_list = g->adjacency_list;
    vector<int> adj_v = adj_list[node];
    for (int &v: adj_v) {
        if (!visited[v]) {
            g->dfs_prev[v] = node;
            dfs(v, t, g);
        }
    }
    depth--;
}

void print_dfs(struct graph* gr) {
    vector<int> verts;
    int vertex = target_node;
    while (vertex!=-1) {
        verts.push_back(vertex);
        vertex = gr->dfs_prev[vertex];
    }
    std::reverse(verts.begin(), verts.end());
    for (int v: verts) {
        cout << v << ' ';
    }
    cout << endl;
    cout << "dist = " << verts.size() - 1 << endl;
}

void start_dfs(struct graph* gr) {
    cout << "---DFS---" << endl;
    gr->dfs_prev.resize(NV);
    gr->dfs_prev[start_node] = -1;
    dfs(start_node, target_node, gr);
    print_dfs(gr);
}

int get_intersection(const bool *s_used, const bool *t_used) {
    for (int i = 0; i < NV; i++) {
        if (s_used[i] && t_used[i])
            return i;
    }
    return -1;
}

void BFS(queue<int> &v_queue, bool *visited, int *parent, struct graph *gr) {
    int node = v_queue.front();
    v_queue.pop();
    vector<vector<int>> adj_list = gr->adjacency_list;
    for (int &v: adj_list[node]) {
        if (!visited[v]) {
            parent[v] = node;
            visited[v] = true;
            v_queue.push(v);
        }
    }
}

void print_BS(const int *s_parent, const int *t_parent, int i_node) {
    cout << "---BS---" << endl;
    vector<int> path;
    vector<int> dist;
    dist.resize(NV,0);
    path.push_back(i_node);
    int i = i_node;
    while (i!=start_node){
        int v = s_parent[i];
        path.push_back(v);
        i = v;
    }
    reverse(path.begin(), path.end());
    i = i_node;
    while (i!=target_node) {
        int v = t_parent[i];
        path.push_back(v);
        i = v;
    }
    for (int v: path) {
        cout << v << ' ';
    }
    cout << endl;
    cout << "dist = " << path.size() - 1 << endl;
}

void BS(struct graph* gr) {
    bool s_used[NV], t_used[NV];
    int s_parent[NV], t_parent[NV];
    queue<int> s_queue, t_queue;

    for (int i = 0; i < NV; i++) {
        s_used[i] = false;
        t_used[i] = false;
    }

    s_queue.push(start_node);
    s_used[start_node] = true;
    s_parent[start_node] = -1;
    t_queue.push(target_node);
    t_used[target_node] = true;
    t_parent[target_node] = -1;

    while (!s_queue.empty() && !t_queue.empty()) {
        BFS(s_queue, s_used, s_parent, gr);
        BFS(t_queue, t_used, t_parent, gr);
    }
    int i_node = get_intersection(s_used, t_used);
    if (i_node != -1) {
//        cout << "i_node: " << i_node;
        print_BS(s_parent, t_parent, i_node);
    }
}

int dls_depth = 0;
int depth_limit = 6;
vector<int> dls_visited(NV);
void dls(int node, int t, struct graph* gr) {
    dls_depth++;
    if (dls_depth > depth_limit) return;
    dls_visited[node] = true;
    vector<vector<int>> adj_list = gr->adjacency_list;
    vector<int> adj_v = adj_list[node];
    for (int &v: adj_v) {
        if (!dls_visited[v]) {
            depth++;
            gr->dls_prev[v] = node;
            dls(v, t, gr);
        }
    }
    dls_depth--;
}

void print_dls(vector<int> &prev) {
    cout << "---DLS---" << endl;
    vector<int> dls_vertices;
    int vertex = target_node;
    while (vertex!=-1) {
        dls_vertices.push_back(vertex);
        vertex = prev[vertex];
    }
    std::reverse(dls_vertices.begin(), dls_vertices.end());
    for (int v: dls_vertices) {
        cout << v << ' ';
    }
    cout << endl;
    cout << "dist = " << dls_vertices.size() - 1 << endl;
}

void start_dls(struct graph* gr) {
    gr->dls_prev.resize(NV);
    gr->dls_prev[start_node] = -1;
    dls(start_node, target_node, gr);
    print_dls(gr->dls_prev);
}

bool ids_dls(int s, int t, int d, struct graph* gr) {
    if (s==t) return true;
    if (d <= 0) return false;
    vector<vector<int>> adj_l = gr->adjacency_list;
    vector<int> adj_v = adj_l[s];
    for (int &v: adj_v) {
        if (ids_dls(v,t,d-1, gr)) {
            gr->ids_prev[v] = s;
            return true;
        }
    }
    return false;
}

bool ids(int s, int t, struct graph* gr) {
    cout << "---IDS---" << endl;
    for (int i = 0; i <= depth_limit; i++) {
        if (ids_dls(s,t,i, gr)) return  true;
    }
    return false;
}

void print_ids(struct graph* gr) {
    vector<int> ids_v;
    int vertex = target_node;
    while (vertex!=-1) {
        ids_v.push_back(vertex);
        vertex = gr->ids_prev[vertex];
    }
    std::reverse(ids_v.begin(), ids_v.end());
    for (int v: ids_v) {
        cout << v << ' ';
    }
    cout << endl;
    cout << "dist = " << ids_v.size() - 1 << endl;
}

void start_ids (struct graph* gr) {
    gr->ids_prev.resize(NV);
    gr->ids_prev[start_node] = -1;
    if (ids(start_node,target_node, gr)) {
        cout << "A path with a given depth was found" << endl;
        print_ids(gr);
    } else cout << "A path with a given depth wasn't found" << endl;
}

void fill_weight_graph(struct weight_graph* wg_ref) {
    map<string, int> vg = { {vertices[0], 0} };
    for (int i = 1; i < vertices.size(); i++) {
        if (!vg.contains(vertices[i])) vg[vertices[i]] = vg.size();
    }
    wg_ref->distances.resize(NV);
    wg_ref->adj_weight_list.resize(NV);
    for (auto & edge: edges_weights_list) {
        int begin = vg[edge[0]];
        int end = vg[edge[1]];
        int weight = stoi(edge[2]);
        if (wg_ref->is_bfs_heuristic) {
            wg_ref->adj_weight_list[begin].emplace_back(weight, end);
            wg_ref->adj_weight_list[end].emplace_back(weight, begin);
        } else {
            int h1 = wg_ref->distances[end];
            int h2 = wg_ref->distances[begin];
            wg_ref->adj_weight_list[begin].emplace_back(weight+h1, end);
            wg_ref->adj_weight_list[end].emplace_back(weight+h2,begin);
        }
    }
}

void find_path(struct weight_graph* wg_ref) {
    if (wg_ref->is_bfs_heuristic) cout << "---Greedy BFS---" << endl;
    else cout << "---A*---" << endl;
    bool visited_nodes[NV];
    priority_queue<pair_entry , vector<pair_entry>, greater<>> pq;
    pq.emplace(0,start_node);
    visited_nodes[start_node] = true;
    int dist = 0;
    while(!pq.empty()) {
        int node = pq.top().second;
        cout << node << ' ';
        pq.pop();
        dist += pq.top().first;
        wg_ref->distances[node] = dist;
//        cout << "node, " << node;
//        cout << " w " << distances[node];
//        cout << endl;
        if (node == target_node) break;
        vector<pair_entry> edge = wg_ref->adj_weight_list[node];
        for (auto vw: edge) {
            if (!visited_nodes[vw.second]) {
                visited_nodes[vw.second] = true;
                pq.emplace(vw.first, vw.second);
            }
        }
    }
    cout << endl;
    cout << "distance: " << wg_ref->distances[target_node] << endl;
}

void start_inf_searches() {
    weight_graph dijkstra;
    dijkstra.is_bfs_heuristic = true;
    fill_weight_graph(&dijkstra);
    find_path(&dijkstra);

    weight_graph astar;
    fill_weight_graph(&astar);
    find_path(&astar);
}

void start_base_searches() {
    graph gr;
    fill_adjacency_list(&gr);
    bfs(&gr);
    start_dfs(&gr);
    BS(&gr);
    start_dls(&gr);
    start_ids(&gr);
}