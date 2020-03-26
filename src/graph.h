#ifndef __GRAPH__
#define __GRAPH__

#include <iostream>
#include <vector>
#include <map>
#include <climits>

//#define BLOCKED -1212
//#define EMPTY -13224
//#define NONEMPTY -14543
#define WMGraph Graph::inst()
#define NONE INT_MIN

using namespace std;

typedef pair<int,int> KeyType;

struct Edge
{
    int v1, v2;
    double cost;

    Edge(int v1, int v2, double cost):
        v1(v1), v2(v2), cost(cost) {}

};


struct Vertex
{
    enum { EMPTY, NONEMPTY, BLOCKED };

    int id;
    int x,y; 
    int stat;
    
    int client;         // occupied


    vector<int> adj;

    Vertex(){}

    Vertex(int id, int x, int y, int stat): 
        id(id), x(x), y(y), stat(stat) {}


    ostream& operator << (ostream& os) { return os << "(" << x << " " << y << ")"; }

    void setInfo(int _id, int _x, int _y, int _stat);
    void updateStat(int _stat, int _client=NONE); 
    void addAdj(int v);
    void removeAdj(int v);
};


class Graph
{
    
  private:
    static Graph* instance;
    bool** _map;  
    int width, height;
    
    // objects
    vector<Vertex> vertices;
    vector<Edge> edges;

    // hash map
    map<KeyType, int> key2vertex;
    map<KeyType, int> key2edge;

  public:
    static Graph* inst();

    int getWidth(){ return width; }
    int getHeight(){ return height; }
    bool** get2DMap(){ return _map; } 
    Vertex* getVertex(int id);
    Edge*   getEdge(int v1, int v2);
    int     getIndex(int x, int y);
    int     randVertex(int stat = Vertex::EMPTY, int spacingX = 0, int spacingY = 0);    
    void    readImgMap(const char* inFile);

    vector<Vertex*> getInside(int lx, int ly, int ux, int uy);


    void    init(int width, int height);
    void    createEdge(int v1, int v2, double cost);
    void    removeEdge(int v1, int v2);
    void    updateStat(int stat, int lx, int ly, int ux, int uy, int client=NONE);
    double  calcDist(int v1, int v2, bool calcManh);
};

#endif

