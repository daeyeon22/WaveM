#include "graph.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include "bitmap_image.hpp"

using namespace std;


Graph* Graph::instance = nullptr;

Graph* Graph::inst()
{
    if(instance == nullptr)
        instance = new Graph();
    return instance;
}




void Vertex::setInfo(int _id, int _x, int _y, int _stat)
{
    id = _id;
    x = _x;
    y = _y;
    stat = _stat;
}


void Vertex::updateStat(int _stat, int _client)
{
    stat = _stat;
    client = _client;
}


void Vertex::addAdj(int v)
{
    auto it = find(adj.begin(), adj.end(), v);
    if(it == adj.end())
    {
        adj.push_back(v);
    }
}

void Vertex::removeAdj(int v)
{
    auto it = find(adj.begin(), adj.end(), v);
    if(it != adj.end())
    {
        adj.erase(it);
    }
}

void Graph::updateStat(int stat, int lx, int ly, int ux, int uy, int client)
{
    for(Vertex* vtx : getInside(lx, ly, ux, uy))
        vtx->updateStat(stat, client);
}



vector<Vertex*> Graph::getInside(int lx, int ly, int ux, int uy)
{
    vector<Vertex*> container;

    lx = max(0, lx);
    ux = min(width-1, ux);
    ly = max(0, ly);
    uy = min(height-1, uy);

    //cout << "(" << lx << " " << ly << ") (" << ux << " " << uy << ")" << endl;

    for(int x=lx; x <= ux; x++)
    {
        for(int y=ly; y <= uy; y++)
        {
            container.push_back(getVertex(getIndex(x, y)));
        }
    }

    return container;
}



Vertex* Graph::getVertex(int id)
{
    return &vertices[id];
}

int Graph::getIndex(int x, int y)
{
    return key2vertex[ KeyType(x, y) ];
}

Edge* Graph::getEdge(int v1, int v2)
{
    if(v1 > v2)
        swap(v1, v2);

    KeyType key(v1, v2);
   
    return &edges[ key2edge[ key ] ];
}

double Graph::calcDist(int v1, int v2, bool calcManh)
{
    int x1 = getVertex(v1)->x;
    int y1 = getVertex(v1)->y;
    int x2 = getVertex(v2)->x;
    int y2 = getVertex(v2)->y;
    int dx = abs( x1 - x2 );
    int dy = abs( y1 - y2 );
    
    if(calcManh)
        return dx + dy;
    else
        return sqrt( pow(dx, 2) + pow(dy, 2) );        
}


void Graph::readImgMap(const char* inFile)
{

    //load bitmap image
    bitmap_image bmp(inFile);

    int g_width = bmp.width();
    int g_height = bmp.height();
    _map = new bool*[g_width];

    //make boolean map
    for(int i=0; i < g_width; i++)
    {
        _map[i] = new bool[g_height];

        for(int j=0; j < g_height; j++)
        {
            //vector<unsigned> RGB = bmp.getPixel(i,j);
            rgb_t RGB = bmp.get_pixel(i,j);

            //if(RGB[0] == 0 && RGB[1] == 0 && RGB[2] == 0)
            if(RGB.red == 0 && RGB.green == 0 && RGB.blue == 0)
                _map[i][j] = false;
            else
                _map[i][j] = true;

        }

    }

    // initialize graph
    init(g_width, g_height);
    cout << "[Rep] Graph Initialization Done (" << g_width << " " << g_height << ")" <<  endl;

}



void Graph::init(int i_width, int i_height)
{
   
    width = i_width;
    height = i_height;

    vertices = vector<Vertex>(width * height);

    vector<pair<int,int>> delta = { {1, 0},  {0, 1} };
    

    // Grid Graph
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            int id = x + y * width;
            int stat = _map[x][y] ? Vertex::EMPTY : Vertex::BLOCKED;
            vertices[id].setInfo(id, x, y, stat);


            KeyType key(x, y);

            key2vertex.insert( { key, id } );

            // if the vertex is blocked,
            // no edge exists
            if(stat == Vertex::BLOCKED)
                continue;

            for(auto it : delta)
            {
                int dx = x + it.first;
                int dy = y + it.second;
                
                // out of range
                if(dx < 0 || dx > width)
                    continue;

                if(dy < 0 || dy > height)
                    continue;


                // adj
                int v1 = x + y * width;
                int v2 = dx + dy * width;

                // uniform edge cost
                double cost = 1.0;
                
                // create edge e(v1,v2)
                createEdge(v1, v2, cost);
            }
        }
    }

    

}


void Graph::createEdge(int v1, int v2, double cost)
{
    if(v1 > v2)
        swap(v1, v2);

    KeyType key(v1, v2);
    
    if(key2edge.find( key ) == key2edge.end())
    {
        key2edge.insert( { key, (int)edges.size() } );
        edges.push_back(Edge( v1, v2, cost));

        getVertex(v1)->adj.push_back(v2);
        getVertex(v2)->adj.push_back(v1);
    }
}


void Graph::removeEdge(int v1, int v2)
{
    if(v1 > v2)
        swap(v1, v2);

    KeyType key(v1, v2);
   
    auto it = key2edge.find( key );

    if(it != key2edge.end())
    {
        key2edge.erase(it);

        getVertex(v1)->removeAdj(v2);
        getVertex(v2)->removeAdj(v1);
    }
}

int Graph::randVertex(int stat, int spacingX, int spacingY)
{
    while(true)
    {
        int vid = rand() % vertices.size();
        Vertex* current = getVertex(vid);
        if(current->stat == Vertex::EMPTY)
        {
            int lx = current->x - spacingX;
            int ux = current->x + spacingX;
            int ly = current->y - spacingY;
            int uy = current->y + spacingY;

            bool valid = true;
            for(Vertex* vertex : getInside(lx, ly, ux, uy))
            {
                if(vertex->stat != Vertex::EMPTY)
                {
                    valid =  false;
                    break;
                }
            }
            if(valid)
                return vid;
        }
    }
}

