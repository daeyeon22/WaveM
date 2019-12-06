#ifndef __ROUTER__
#define __ROUTER__

#include <algorithm>
#include <queue>
#include <limits>
#include <cfloat>

using namespace std;

struct Path
{
    vector<int> vertices;
};

class Router
{
  private:
    vector<double> actCost;
    vector<double> estCost;
    vector<int> parent;
   
    // priority queue sorting function
    struct Sort
    {
        bool operator () (pair<int,double> &elem1, pair<int,double> &elem2) 
        {
            return elem1.second > elem2.second;
        }
    };

    // prioirty queue
    priority_queue<pair<int, double>, vector<pair<int, double>>, Sort> PQ;



  public:


    Router(int size)
    {
        actCost = vector<double>(size, DBL_MAX);
        estCost = vector<double>(size, DBL_MAX);
        parent = vector<int>(size, -1);
    }

    ~Router()
    {
        //delete actCost;
        //delete estCost;
        //delete parent;
    }

    void    update(int cur, int par, int tar);
    void    pushNeighbors(int node, int end);
    void    push(int node);
    int     pop();
    bool    isVisited(int node);
    bool    isEmpty();
   
    Path    findShortestPath(int start, int end);

};

#endif
