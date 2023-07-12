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
#define pb push_back
#include<chrono>
using namespace std;
// #define V 6
int max_flow;  // For storing Max Flow

/// @brief Prints the 2-D vector
/// @param g Graph g 
void printVec(vector<vector<int>> g){
    for(int i = 0; i < g.size(); i++){
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
bool bfs(vector<vector<int>> residualGraph, int s, int t, int store[], int V){
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
vector<vector<int>> FF(vector<vector<int>> G, int s, int t, int V){
    vector<vector<int>> residualGraph(V, vector<int> (V));
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
            path_flow = min(path_flow, residualGraph[u][v]); // bottleneck capacity
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

/// @brief Depth First Search for finding for reachable nodes from source node s
/// @param residualGraph Residual Graph of Graph G
/// @param s source node s
/// @param v1 Boolean array for checking if that node is visited or not
/// @param V Total number of Vertices
void dfs(vector<vector<int>> residualGraph, int s, bool v1[], int V){
    v1[s] = true;
    for (int i = 0; i < V; i++)
       if (residualGraph[s][i] && !v1[i])
           dfs(residualGraph, i, v1, V);
}

/// @brief Calculates Min Cut from Max Flow graph obtained from Ford Fulkerson
/// @param s Source node s
/// @param t Destination node t
/// @param V Total number of Vertices
/// @param residualGraph Residual Graph of Graph G
/// @param v1 Boolean array for obtaining Set S and Set T
/// @param G Graph G
void minCut(int s, int t, int V, vector<vector<int>> residualGraph, bool v1[], vector<vector<int>> G){
    bool visited[V];
	memset(visited, false, sizeof(visited));
	dfs(residualGraph, s, visited, V);

    int minccap = 0;
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
        if (visited[i] && !visited[j] && G[i][j]){    
	        minccap += G[i][j];
            cout << i << " - " << j << endl;
	}
    cout << "Min-cut capacity = " << minccap << endl;
    cout << "Min-cut:" << endl << "Set S = {" << s;
    for (int i = 0; i < V; i++)
    	if (visited[i] && i != s)
		cout << ", " << i;
     
    cout << "}" << endl << "Set T = {" << t;
    for (int i = 0; i < V; i++)
    	if (!visited[i] && i != t)
		cout << ", " << i;
    cout << "}" << endl;
}

/// @brief Main Function
/// @param argc Command line arguments
/// @param argv Command line arguments
/// @return Returns the Maximum Flow and Minimum Capacity
int main(int argc, char* argv[]){
    freopen(argv[1], "r", stdin);
    int V;
    cin >> V;
    vector<vector<int>> g(V, vector<int> (V));
    for(int i = 0; i<V; i++){
        for(int j = 0; j<V; j++){
            cin >> g[i][j];
        }
    }
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    vector<vector<int>> rG;
    rG = FF(g, V-2, V-1, V);
    cout << "Max Flow : " << max_flow << endl;  
    bool visited[V];
    memset(visited, false, sizeof(visited));
    minCut(V-2, V-1, V, rG, visited, g);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
}