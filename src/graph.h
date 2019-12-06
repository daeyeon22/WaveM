#ifndef __GRAPH__
#define __GRAPH__

#include <iostream>
#include <vector>
#include <map>


#define BLOCKED -1212
#define EMPTY -13224
#define NONEMPTY -14543

#define graph Graph::inst()


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
    
    int id;
    int x,y; 
    int status;

    vector<int> adj;

    Vertex(){}

    Vertex(int id, int x, int y, int status): 
        id(id), x(x), y(y), status(status) {}

    void setInfo(int _id, int _x, int _y, int _stat);
    void updateStat(int stat); 
    void addAdj(int v);
    void removeAdj(int v);
};


class Graph
{
    
  private:
    static Graph* instance;
      
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
    
    Vertex* getVertex(int id);
    Edge*   getEdge(int v1, int v2);
    int     getIndex(int x, int y);
    
    void    init(int width, int height, bool **map);
    void    createEdge(int v1, int v2, double cost);
    void    removeEdge(int v1, int v2);

    double calcDist(int v1, int v2, bool calcManh);
};

#endif

