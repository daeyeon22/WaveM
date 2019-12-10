#include "graph.h"
#include "router.h"


//ostream& operator << (ostream& os, Vertex* vtx)
//{
//    return os << "(" << vtx->x << " " << vtx->y << ")";
//}

bool Router::isVisited(int node)
{
    // if visited, then this array stores its parent node id 
    return parent[node] == -1 ? false : true;
}

void Router::init(int size)
{
    actCost = vector<double>(size, DBL_MAX);
    estCost = vector<double>(size, DBL_MAX);
    parent = vector<int>(size, -1);
}

void Router::update(int cur, int par, int end)
{

    actCost[cur] = actCost[par] + WMGraph->getEdge(cur, par)->cost;
    estCost[cur] = WMGraph->calcDist(cur, end, false);
    parent[cur] = par;
}



void Router::push(int node)
{
    double cost = actCost[node] + estCost[node];
    PQ.push( { node, cost } );
}

int Router::pop()
{
    pair<int,double> elem = PQ.top();
    PQ.pop();
    return elem.first;
}

void Router::pushNeighbors(int node, int end)
{
    
    for(auto neighbor : WMGraph->getVertex(node)->adj)
    {
        if(isVisited(neighbor))
            continue;

        if(WMGraph->getVertex(neighbor)->stat == Vertex::EMPTY)
        {
            update(neighbor, node, end);
            push(neighbor);
        }
    }
}

bool Router::isEmpty()
{
    return PQ.empty();
}

Path Router::findShortestPath(int start, int end)
{
  
    //cout << "[Rep] Find shortest path from " << WMGraph->getVertex(start) << " to " << WMGraph->getVertex(end) << endl;
    bool existPath = false;

    // start point
    actCost[start] = 0;
    estCost[start] = WMGraph->calcDist(start, end, false);
    parent[start] = start;
    push(start);


    // astar search
    while(!isEmpty())
    {
        int current = pop();

        //cout << "Current : " << WMGraph->getVertex(current) << endl;


        if(current == end)
        {
            // arrival!
            existPath = true;
            break;
        }

        pushNeighbors(current, end);
    }


    int itVtx = end;
    Path path;
   
    //cout << "Start backtracking!" << endl;
    if(existPath)
    {
        // backtracking search
        while( true )
        {
            path.vertices.insert(path.vertices.begin(), itVtx);
            //cout << "Current : " << WMGraph->getVertex(itVtx) << endl;

            if(parent[itVtx] == itVtx)
                break;

            itVtx = parent[itVtx];
        }
    }
    else
    {
        cout << "No path exist" << endl; 
    }


    return path;

}









