#include <bits/stdc++.h>     // Includes all standard libraries
#include <omp.h>             // OpenMP library for parallel computing

using namespace std;

// Graph class representing an undirected graph using adjacency list
class Graph
{
private:
    unordered_map<int, vector<int>> adj; // Adjacency list to store graph
    int edges, vertices;                 // Total number of edges and vertices

public:
    // Constructor to initialize the graph
    Graph()
    {
        adj.clear();         // Clear adjacency list
        edges = vertices = 0;
    }

    // Function to take graph input from user
    void input()
    {
        cout << "Enter number of vertices: ";
        cin >> vertices;
        cout << "Enter number of edges: ";
        cin >> edges;

        // Initialize each vertex with an empty adjacency list
        for (int i = 0; i < vertices; i++)
        {
            adj[i] = {};
        }

        // Input each edge and add to adjacency list (undirected graph)
        for (int i = 0; i < edges; i++)
        {
            int src, dst;
            cout << "Enter source vertex No. (0 - " << vertices - 1 << " ): ";
            cin >> src;
            cout << "Enter destination vertex No. (0 - " << vertices - 1 << " ): ";
            cin >> dst;
            adj[src].push_back(dst);  // Add edge from src to dst
            adj[dst].push_back(src);  // Add edge from dst to src (undirected)
            cout << endl;
        }
    }

    // Function to display the adjacency list of the graph
    void display()
    {
        cout << "Adjacency list:\n";
        for (int i = 0; i < vertices; i++)
        {
            cout << i << " => ";
            for (auto it : adj[i])
            {
                cout << it << " ";
            }
            cout << endl;
        }
    }

    // Helper for sequential DFS traversal
    void DFSHelper()
    {
        vector<bool> visited(vertices, false); // Track visited nodes
        cout << "Enter starting vertex for sequential DFS: ";
        int startVertex;
        cin >> startVertex;

        visited[startVertex] = true;
        cout << startVertex << " ";

        // Traverse each neighbor of the starting vertex
        for (auto it : adj[startVertex])
        {
            if (!visited[it])
            {
                visited[it] = true;
                DFS(it, visited);  // Call recursive DFS
            }
        }
        cout << endl;
    }

    // Helper for parallel DFS traversal using OpenMP
    void ParallelDFSHelper()
    {
        vector<bool> visited(vertices, false); // Track visited nodes
        cout << "Enter starting vertex for parallel DFS: ";
        int startVertex;
        cin >> startVertex;

        visited[startVertex] = true;
        cout << startVertex << " ";

        // Parallel for loop over neighbors of startVertex
#pragma omp parallel for
        for (int i = 0; i < adj[startVertex].size(); i++)
        {
            int it = adj[startVertex][i];

            // Critical section to avoid data race on 'visited'
#pragma omp critical
            {
                if (!visited[it])
                {
                    visited[it] = true;
                    DFS(it, visited);  // Call recursive DFS (still sequential per call)
                }
            }
        }
        cout << endl;
    }

    // Recursive DFS function
    void DFS(int it, vector<bool> &visited)
    {
        visited[it] = true;
        cout << it << " ";

        // Visit all unvisited neighbors recursively
        for (int i = 0; i < adj[it].size(); i++)
        {
            int node = adj[it][i];
            if (!visited[node])
            {
                DFS(node, visited);
            }
        }
    }

    // Sequential BFS traversal
    void BFS()
    {
        int startVertex;
        cout << "Enter starting vertex for BFS: ";
        cin >> startVertex;

        vector<bool> visited(vertices, false); // Track visited nodes
        queue<int> q;
        q.push(startVertex);                   // Start BFS from startVertex

        while (!q.empty())
        {
            int node = q.front();
            q.pop();

            if (visited[node])
                continue;

            visited[node] = true;
            cout << node << " ";

            // Push all unvisited neighbors into the queue
            for (int i = 0; i < adj[node].size(); i++)
            {
                int it = adj[node][i];
                if (!visited[it])
                {
                    q.push(it);
                }
            }
        }
        cout << endl;
    }

    // Parallel BFS traversal using OpenMP
    void ParallelBFS()
    {
        int startVertex;
        cout << "Enter starting vertex for parallel BFS: ";
        cin >> startVertex;

        vector<bool> visited(vertices, false); // Track visited nodes
        queue<int> q;
        q.push(startVertex);                   // Start BFS

        while (!q.empty())
        {
            int node;

            // Critical section to safely read and pop from shared queue
#pragma omp critical
            {
                node = q.front();
                q.pop();
            }

            bool alreadyVisited;
            // Critical block to check and mark as visited
#pragma omp critical
            {
                alreadyVisited = visited[node];
                if (!visited[node])
                {
                    visited[node] = true;
                    cout << node << " ";
                }
            }

            // Skip if node was already visited by another thread
            if (alreadyVisited)
                continue;

            // Parallelize neighbor traversal
#pragma omp parallel for
            for (int i = 0; i < adj[node].size(); i++)
            {
                int it = adj[node][i];
                // Critical section to avoid data race while pushing to queue
#pragma omp critical
                {
                    if (!visited[it])
                        q.push(it);
                }
            }
        }
        cout << endl;
    }
};

// Main function
int main()
{
    Graph g;       // Create graph object
    g.input();     // Input graph
    g.display();   // Display adjacency list

    double start_time, end_time;  // To measure execution time

    // Sequential DFS
    start_time = omp_get_wtime();
    g.DFSHelper();
    end_time = omp_get_wtime();
    cout << "Time taken by Sequential DFS is " << end_time - start_time << " seconds" << endl;

    // Parallel DFS
    start_time = omp_get_wtime();
    g.ParallelDFSHelper();
    end_time = omp_get_wtime();
    cout << "Time taken by Parallel DFS is " << end_time - start_time << " seconds" << endl;

    // Sequential BFS
    start_time = omp_get_wtime();
    g.BFS();
    end_time = omp_get_wtime();
    cout << "Time taken by Sequential BFS is " << end_time - start_time << " seconds" << endl;

    // Parallel BFS
    start_time = omp_get_wtime();
    g.ParallelBFS();
    end_time = omp_get_wtime();
    cout << "Time taken by Parallel BFS is " << end_time - start_time << " seconds" << endl;

    return 0;
}
