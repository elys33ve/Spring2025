#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

/*  ALGORITHMS ASSIGNMENT 3 - Connected Components of a Network

Inputs a graph G by first inputting the number of vertices n 
followed by a sequence of pairs i j where i and j are integers 
between 0 and n-1, inclusive, representing the edges of the graph, 
and ending with a negative integer sentinel to indicate the end of 
the input. 
    - Implement the graph G with using pointer-based adjacency lists 
    (using an array of header nodes and linked list for each header node). 
    Your class Graph should include constructors and a destructor, 
    operations of edge addition, edge deletion, etc.,
    - Implement the function DFS(G,v) for performing depth-first search.
    - Implement function Components(G) for computing the vertex sets
    of the connected components of G. Components() will employ a 
    DFT (Depth-First Traversal), which involves calling DFS(G,v), 
    v = 0, 1, .. n – 1.

-- tests:
make && main.exe 5 0 1 2 1 3 4 -1
make && main.exe 4 0 1 1 2 1 3 2 3 3 0 -1
make && main.exe 5 0 1 1 4 2 3 1 3 3 4 -1
*/


// ===========================================================================

// --- Node structure for adjacency list
struct Node {
    int vertex;
    Node* next;
};


// --- Graph
class Graph {
 private:
    int n_vertices;             // number of vertices
    vector<Node*> adj_list;     // adjacency list - array of header nodes    
    bool is_directed = false;

 public:
    // --- Constructor
    Graph(int vertices) : n_vertices(vertices) {
        adj_list.resize(vertices, nullptr);     // resize array and initialize to nullptr
    }

    // --- Destructor
    ~Graph() {
        // free memory
        for (int i=0; i<n_vertices; ++i) {
            Node* current = adj_list[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }

    // --- Add edge to the graph
    void addEdge(int src, int dest) {
        if (src >= 0 && dest >= 0 && src < n_vertices && dest < n_vertices) {
            // add edge from src to dest
            Node* new_node = new Node{dest, adj_list[src]};
            adj_list[src] = new_node;
        
            if (!is_directed) {
                // add edge from dest to src 
                Node* new_node = new Node{src, adj_list[dest]};
                adj_list[dest] = new_node;
            }
        }
        else {
            cout << "invalid vertices " << src << ", " << dest << "\n";
        }
    }
    // --- Delete edge from the graph
    void deleteEdge(int src, int dest) {
        if (src >= 0 && dest >= 0 && src < n_vertices && dest < n_vertices) {
            // delete edge from src to dest
            Node* current = adj_list[src];
            Node* prev = nullptr;
            // find dest vertex
            while (current != nullptr && current->vertex != dest) {
                prev = current;
                current = current->next;
            }
            if (current != nullptr) {
                // remove and free memory
                if (prev == nullptr) {
                    adj_list[src] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
            }

            if (!is_directed) {
                // delete edge from dest to src
                current = adj_list[dest];
                prev = nullptr;
                // find src vertex
                while (current != nullptr && current->vertex != src) {
                    prev = current;
                    current = current->next;
                }
                if (current != nullptr) {
                    // remove and free memory
                    if (prev == nullptr) {
                        adj_list[dest] = current->next;
                    } else {
                        prev->next = current->next;
                    }
                    delete current;
                }
            }
        } else {
            cout << "invalid vertices " << src << ", " << dest << "\n";
        }
    }

    // --- Depth-First Search (DFS)
    void DFS(int vertex, vector<bool>& visited, vector<int>& component) {
        visited[vertex] = true;
        component.push_back(vertex);

        // traverse adjacent vertices
        Node* temp = adj_list[vertex];
        while (temp != nullptr) {
            if (!visited[temp->vertex]) {
                DFS(temp->vertex, visited, component);
            }
            temp = temp->next;
        }
    }

    // --- Compute connected components of the graph
    vector<vector<int>> components() {
        vector<vector<int>> components;
        vector<bool> visited(n_vertices, false);

        // iterate through vertices and recursive DFS
        for (int i=0; i<n_vertices; ++i) {
            if (!visited[i]) {
                vector<int> component;
                DFS(i, visited, component);
                components.push_back(component);
            }
        }
        return components;
    }
    // --- print connected component
    void printComponents(vector<vector<int>> c) {
        cout << "\nConnected Components:\n";
        for (int i = 0; i < c.size(); ++i) {
            cout << "{ ";
            for (int j = 0; j < c[i].size(); ++j) {
                cout << c[i][j] << " ";
            }
            cout << "} " << endl;
        }
        cout << endl;
    }

    // --- Print adjacency matrix
    void printAdjacencyMatrix() {
        cout << "\nAdjacency Matrix:\n";

        for (int i=0; i<n_vertices; ++i) {
            for (int j=0; j<n_vertices; ++j) {
                // check for edge between i and j
                bool is_connected = false;
                Node* node = adj_list[i];
                while (node != nullptr) {
                    if (node->vertex == j) {
                        is_connected = true;
                        break;
                    }
                    node = node->next;
                }
                cout << is_connected << " "; 
            }
            cout << endl;
        }
    }

    // --- Print adjacency list of graph
    void printGraph() {
        cout << "\nAdjacency, Linked List:\n";

        // iterate through each vertex
        for (int i=0; i<adj_list.size(); ++i) {
            cout << i << ": ";      // print vertex
            Node* current = adj_list[i];

            // iterate through adjacency list of vertex
            while (current != nullptr) {
                cout << current->vertex << " -> ";
                current = current->next;
            }
            cout << "NULL" << endl;
        }
    }

};


// ===========================================================================

int main(int argc, char* argv[]) {
    int n, i, j;
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <number_of_vertices> <i1> <j1> <i2> <j2> ...\n";
        return 1;
    }

    // get number of vertices from command line args
    n = atoi(argv[1]);  
    cout << "\nnumber of vertices: " << n << "\nV = {0"; 
    for (int k=1; k<n; k++) {
        cout << "," << k;
    } 
    cout << "}\n";

    Graph G(n);

    // get edges from command line args
    cout << "E = {";
    for (int k=2; k<argc; k += 2) {
        int u = atoi(argv[k]);
        int v = atoi(argv[k + 1]);

        if (u >= 0 && u < n && v >= 0 && v < n) {
            G.addEdge(u, v);
            cout << "{" << u << "," << v << "}";
        }

        if (k+3 < argc) { cout << ","; }
    }
    cout << "}\n";

    // print adjacency matrix
    G.printAdjacencyMatrix();

    // print connected components
    G.printComponents(G.components());

    return 0;
}





