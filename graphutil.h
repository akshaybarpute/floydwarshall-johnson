#include<vector>
using namespace std;
class GraphUtil{
    private:
        int size=0, max=0, min=0;
        bool sparse=false;
        int minDist(int dist[], bool sptSet[], int V);
        void dijkstra(vector<vector<int>> graph, vector<vector<int>> *mdGraph, int src);
        vector<int> bellmanFord(vector<tuple<int,int,int>> edges, vector<vector<int>>graph);

    public:
        GraphUtil();
        GraphUtil(int maxSize, int maxWeight, int minWeight);
        GraphUtil(int maxSize, int maxWeight);
        void setSparse(bool val);
        vector<vector<int>> generateRandom();
        void print(vector<vector<int>> res);
        void print(vector<vector<int>> res, string filePath);
        vector<vector<int>> buildGraphFromText(string filePath);
        pair<vector<vector<int>>,bool> floydWarshall(vector<vector<int>> input);
        vector<vector<int>> johnsons(vector<vector<int>> graph);
        void runTests(string algo);
};