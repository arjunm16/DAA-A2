///@file
/*
Group Details

|       Name            	|          ID            	|
|---------------------------|---------------------------|	    
| Harsh Vardhan Gupta     	|     2019B3A70630H 	    |
| Aryan Kapadia         	|     2019B3A70412H 	    |
| Arjun Muthiah  	        |     2019B3A70374H 	    |
*/
#include<bits/stdc++.h>
#include<ctime>
#include<chrono>
using namespace std;
// #define M 6
// #define N 6
int max_flow;

/// @brief Prints the 2D Vector
/// @param g Graph g
void printVec(vector<vector<int> > g){
    for(int i = 0; i < g.size(); i++){
        // cout << i << ": ";
        for(int j = 0; j < g[i].size(); j++){
            cout << g[i][j] << " ";
        }
        cout << endl;
    }
}
/// @brief Performs Breadth-First Search on Residual Graph to find a path between s and t
/// @param residualGraph residualGraph where initial node are 0
/// @param s source node s
/// @param t destination node t
/// @param store array for storing nodes
/// @param V Total number of Vertices
/// @return Returns if there is a path between s to t
bool bfs(vector<vector<int> > residualGraph, int s, int t, int store[], int V){
    bool visited[V];
    for(int i = 0; i<V; i++)
        visited[i] = 0;
    
    queue<int> temp;
    temp.push(s);
    visited[s] = 1;
    store[s] = -1;

    while(!temp.empty()){
        int u = temp.front();
        temp.pop();
        for(int i = 0; i<V; i++){
            if(visited[i] == 0 && residualGraph[u][i] > 0){
                if(i == t){  // Simple path found, then return true
                    store[i] = u;
                    return 1;
                }
                temp.push(i);
                store[i] = u;
                visited[i] = 1;
            }
        }

    }
    return 0;
}      

/// @brief Ford Fulkerson algorithm 
/// @param G Graph g
/// @param s Source node s
/// @param t Destination node t
/// @param V Total number of Vertices
/// @return Returns the Residual Graph for Graph G
vector<vector<int> > FF(vector<vector<int> > G, int s, int t, int V){
    vector<vector<int> > residualGraph(V, vector<int> (V));
    // Initializing residualGraph with original Graph only
    int u, v;
    for(u = 0; u<V; u++){
        for(v = 0; v<V; v++){
            residualGraph[u][v] = G[u][v];
        }
    }
    // printVec(residualGraph);
    int flow_checker = 0;

    int store[V];
    while(bfs(residualGraph, s, t,store, V)){
        int path_flow = INT_MAX;
        for(v = t; v!=s; v = store[v]){
            u = store[v];
            path_flow = min(path_flow, residualGraph[u][v]);
        }

        for(v = t; v != s; v = store[v]){
            u = store[v];
            residualGraph[u][v] = residualGraph[u][v] - path_flow;
            residualGraph[v][u] = residualGraph[v][u] + path_flow;
        }
        flow_checker = flow_checker + path_flow;
    }    
    max_flow = flow_checker;
    return residualGraph;
}
/// @brief Transforms the Bipartite Graph into Adjacency list and calculates the maximum flow to find Maximum Bipartite Matching
/// @param argc Command Line Arguments
/// @param argv Command Line Arguments
/// @return Returns the Maximum Bipartite Matching in a Bipartite Graph
int main(int argc, char* argv[]){
    freopen(argv[1], "r", stdin);
    int M, N;
    cin >> M >> N;
    // bool bpGraph[M][N] = {{0, 1, 1, 0, 0, 0},
    //                       {1, 0, 0, 1, 0, 0},
    //                       {0, 0, 1, 0, 0, 0},
    //                       {0, 0, 1, 1, 0, 0},
    //                       {0, 0, 0, 0, 0, 0},
    //                       {0, 0, 0, 0, 0, 1}};
    vector<vector<int> > bpGraph(M, vector<int> (N));
    for(int i = 0; i<M; i++){
        for(int j = 0; j<N; j++){
            cin >> bpGraph[i][j];
        }
    }
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // clock_t start, end;

    // start = clock();

    vector<vector<int> > transformedGraph(M+N+2, vector<int> (M+N+2));
    transformedGraph[0][0] = 0;
    for(int i = 1; i<=N; i++){
        transformedGraph[0][i] = 1;
    }
    // cout << "0: 0 1 2 3 4 5 6 7 8 9 10 11 12 13" << endl;
    for(int i = 0; i<M; i++){
        for(int j = 0; j<N; j++){
            transformedGraph[i+1][j+N+1] = bpGraph[i][j];
        }
    }

    for(int i = M+1; i<M+N+1; i++){
        transformedGraph[i][M+N+1] = 1;
    }

    // printVec(transformedGraph);
    vector<vector<int> > rG;
    rG = FF(transformedGraph, 0, M+N+1, M+N+2);
    cout << max_flow << endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

}