#include <iostream>
#include <list>
#include <chrono>
using namespace std;
using namespace std::chrono;

class Graph {
    int numOfEdges;
    int numOfVertices;
    list<int> *l;
    int** edge;
public:
    Graph(int numOfEdges, int numOfVertices, int** edge) {
        this->numOfEdges = numOfEdges;
        this->numOfVertices = numOfVertices;
        this->edge = edge;
    }
    void addEdge(int x, int y) {
        l[x].push_back(y);
        l[y].push_back(x);
    }
    void printGeneratedGraph() {
        int count;
        cout << "\nThe generated random graph is: " << endl;
        for (int i = 0; i < numOfVertices; i++) {
            count = 0;
            cout << "\t" << i + 1 << "-> { ";
            for (int j = 0; j < numOfEdges; j++) {
                if (edge[j][0] == i + 1) {
                    cout << edge[j][1] << " ";
                    count++;
                }
                else if (edge[j][1] == i + 1) {
                    cout << edge[j][0] << " ";
                    count++;
                }
                else if (j == numOfEdges - 1 && count == 0)
                {
                    cout << "Isolated Vertex!";
                }
            }
            cout << " }" << endl;
        }
    }
};

int** GenerateRandomGraph(int numOfEdges, int numOfVertices);

bool** ConvertToAdjMatrix(int** edge, int numOfVertices, int numOfEdges);

void PrintSolution(pair<int*, bool> path, int numOfVertices);

pair<int*, bool> GetHamiltonianCycle(bool** graph, int numOfVertices);

int main()
{
    int numOfVertices, numOfEdges;
    cout << "Enter number of vertices:" << endl;
    cin >> numOfVertices;
    cout << "\nEnter number of edges:" << endl;
    cin >> numOfEdges;
    int** edge = GenerateRandomGraph(numOfEdges, numOfVertices);
    Graph g(numOfEdges, numOfVertices, edge);
    g.printGeneratedGraph();
    bool** graph = ConvertToAdjMatrix(edge, numOfVertices, numOfEdges);

    auto start = steady_clock::now();
    pair<int*, bool> hamiltonianCycle = GetHamiltonianCycle(graph, numOfVertices);
    auto end = steady_clock::now();

    PrintSolution(hamiltonianCycle, numOfVertices);

    auto duration = duration_cast<milliseconds>(end - start);
    cout << "\nTime taken by program is : " << duration.count() << " milliseconds" << endl;

    for (int i = 0; i < numOfEdges; i++) {
        delete[] edge[i];
    }
    delete[] edge;

    for (int i = 0; i < numOfVertices; i++) {
        delete[] graph[i];
    }
    delete[] graph;

    delete[] hamiltonianCycle.first;
}

int** GenerateRandomGraph(int numOfEdges, int numOfVertices) {
    srand(time(0));
    int i, j;
    int** edge = new int* [numOfEdges];
    for (int i = 0; i < numOfEdges; i++)
    {
        edge[i] = new int[2];
    }
    i = 0;
    while (i < numOfEdges)
    {
        edge[i][0] = rand() % numOfVertices + 1;
        edge[i][1] = rand() % numOfVertices + 1;
        if(edge[i][0] == edge[i][1]) continue;
        else {
            for (j = 0; j < i; j++) {
                if ((edge[i][0] == edge[j][0] && edge[i][1] == edge[j][1]) || (edge[i][0] == edge[j][1] && edge[i][1] == edge[j][0]))
                {
                    i--;
                }
            }
        }
        i++;
    }
    return edge;
}

bool** ConvertToAdjMatrix(int** edge, int numOfVertices, int numOfEdges) {
    bool** adjMatrix = new bool* [numOfVertices];
    for (int i = 0; i < numOfVertices; i++)
    {
        adjMatrix[i] = new bool[numOfVertices];
        for (int j = 0; j < numOfVertices; j++)
        {
            adjMatrix[i][j] = 0;
        }
    }
    for (int i = 0; i < numOfEdges; i++)
    {
        adjMatrix[edge[i][0] - 1][edge[i][1] - 1] = 1;
        adjMatrix[edge[i][1] - 1][edge[i][0] - 1] = 1;
    }
    return adjMatrix;
}

bool isAdjacentToPreviousVertex(int v, bool** graph, int path[], int pos)
{
    if (graph[path[pos - 1]][v] == 0) return false;
    for (int i = 0; i < pos; i++) 
    {
        if (path[i] == v) 
        {
            return false;
        }
    }
    return true;
}

bool DoesHamiltonianCycleExist(bool** graph, int path[], int pos, int numOfVertices)
{
    if (pos == numOfVertices)
    {
        if (graph[path[pos - 1]][path[0]] == 1) return true;
        else return false;
    }
    for (int v = 1; v < numOfVertices; v++)
    {
        if (isAdjacentToPreviousVertex(v, graph, path, pos))
        {
            path[pos] = v;
            if (DoesHamiltonianCycleExist(graph, path, pos + 1, numOfVertices)) return true;
            path[pos] = -1;
        }
    }
    return false;
}

pair<int*, bool> GetHamiltonianCycle(bool** graph, int numOfVertices)
{
    int* path = new int[numOfVertices];
    bool doesExist = true;
    for (int i = 0; i < numOfVertices; i++)
    {
        path[i] = -1;
    }
    path[0] = 0;
    if (!DoesHamiltonianCycleExist(graph, path, 1, numOfVertices))
    {
        doesExist = false;
    }
    return make_pair(path, doesExist);
}

void PrintSolution(pair<int*, bool> path, int numOfVertices)
{
    if (!path.second) {
        cout << "\nSolution does not exist";
    }
    else
    {
        cout << "\nHamiltonian Cycle:\n";
        for (int i = 0; i < numOfVertices; i++)
        {
            cout << path.first[i] << " ";
        }
        cout << path.first[0] << " ";
        cout << endl;
    }
}
