#include "graph.h"
#include "router.h"
#include "server.h"
#include <cmath>

//#define DEBUG

bool Router::checkSpacing(int node)
{
    Vertex* current = WMGraph->getVertex(node);

    int lx = current->x - spacingX;
    int ux = current->x + spacingX;
    int ly = current->y - spacingY;
    int uy = current->y + spacingY;

#ifdef DEBUG
    cout << "(" << current->x << " " << current->y << ") -> (" << lx << " " << ly << ") (" << ux << " " << uy << ")" << endl;
#endif
    for(Vertex* vertex : WMGraph->getInside(lx, ly, ux, uy))
    {
        if(vertex->stat == Vertex::BLOCKED)
        {
            return false;
        }
        else if(vertex->stat == Vertex::NONEMPTY)
        {
            if(vertex->client != client)
                return false;
        }

    }


    return true;
}



bool Router::isVisited(int node)
{
    // if visited, then this array stores its parent node id 
    return parent[node] == -1 ? false : true;
}

void Router::init(int size, int _client)
{
    
    actCost = vector<double>(size, DBL_MAX);
    estCost = vector<double>(size, DBL_MAX);
    parent = vector<int>(size, -1);
    clearQueue();
    client = _client;

    spacingX = ceil(1.0 * WMServer->getClient(client)->getSizeX()/2);
    spacingY = ceil(1.0 * WMServer->getClient(client)->getSizeY()/2);
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
        if(!checkSpacing(neighbor))
            continue;

        Vertex* nextVtx = WMGraph->getVertex(neighbor);
        
        if( (nextVtx->stat == Vertex::EMPTY) || (nextVtx->stat == Vertex::NONEMPTY && nextVtx->client == client) )
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





void Router::setSpacing(int _spacingX, int _spacingY)
{
    spacingX = _spacingX, spacingY = _spacingY;
}






