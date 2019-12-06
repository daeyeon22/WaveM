#include "graph.h"
#include "router.h"



bool Router::isVisited(int node)
{
    // if visited, then this array stores its parent node id 
    return parent[node] == -1 ? false : true;
}

void Router::update(int cur, int par, int end)
{

    actCost[cur] = actCost[par] + graph->getEdge(cur, par)->cost;
    estCost[cur] = graph->calcDist(cur, end, false);
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
    
    for(auto neighbor : graph->getVertex(node)->adj)
    {
        if(isVisited(neighbor))
            continue;

        update(node, neighbor, end);
        push(neighbor);
    }
}

bool Router::isEmpty()
{
    return PQ.empty();
}



Path Router::findShortestPath(int start, int end)
{
   
    bool existPath = false;

    // start point
    actCost[start] = 0;
    estCost[start] = graph->calcDist(start, end, false);
    push(start);


    // astar search
    while(!isEmpty())
    {
        int current = pop();

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
   
    if(existPath)
    {
        // backtracking search
        while( true )
        {
            path.vertices.push_back(itVtx);

            if(parent[itVtx] == itVtx)
                break;

            itVtx = parent[itVtx];
        }
    }


    return path;

}









