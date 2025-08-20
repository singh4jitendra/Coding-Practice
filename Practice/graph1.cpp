    #include <iostream>

    using namespace std;

    class graph{
        public:
        unordered_map<int, vector<int>> umap;
        vector<int> vt;
        void addEdge(int n1, int n2, bool direction)
        {
            umap[n1].push_back(n2);
            
        }
        void printgraph()
        {
            for(auto a: umap)
            {
                cout << a.first << "->";
                for(auto val : a.second)
                    cout << val << ", ";
                cout << endl;
            }

        }
    };

    int main()
    {
        int edge=6;
        graph G;
        //for(int i=0; i<edge; i++)
        {
            G.addEdge(1,2,0);
            G.addEdge(1,3,0);
            G.addEdge(2,1,0);
            G.addEdge(2,5,0);
            G.addEdge(5,2,0);
            G.addEdge(5,6,0);
            G.addEdge(6,4,0);
            G.addEdge(6,5,0);
            G.addEdge(4,3,0);
            G.addEdge(4,6,0);
            G.addEdge(3,1,0);
            G.addEdge(3,4,0);
        }
        G.printgraph();

        return 0;
    }
