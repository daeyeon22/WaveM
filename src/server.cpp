#include "server.h"
#include "graph.h"

ostream& operator << (ostream& os, Vertex* vtx)
{
    return os << "(" << vtx->x << " " << vtx->y << ")";
}

// Singleton 
Server* Server::instance = nullptr;

Server* Server::inst()
{
    if(instance == nullptr)
        instance = new Server();
    return instance;
}

void Server::addClient()
{
    int i = clients.size();

    clients.push_back(Client());

    Vertex* vBeg = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY));
    Vertex* vEnd = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY));

    clients[i].setID(i);
    clients[i].updateStat(Client::IDLE);
    clients[i].setCurLoc(vBeg->x, vBeg->y);
    clients[i].setDesLoc(vEnd->x, vEnd->y);

}

void Server::initClients(int numClients)
{
    clients = vector<Client>(numClients);

    for(int i=0; i < numClients; i++)
    {

        Vertex* vBeg = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY));
        Vertex* vEnd = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY));

        clients[i].setID(i);
        clients[i].updateStat(Client::IDLE);
        clients[i].setCurLoc(vBeg->x, vBeg->y);
        clients[i].setDesLoc(vEnd->x, vEnd->y);
        clients[i].createPath();
    }
}

void Server::next()
{
    for(int i=0; i < numClients(); i++)
    {
        getClient(i)->next();
    }
}




/* Client */
void Client::next()
{
    // do something if job exists
    if(stat == Client::BUSY || stat == Client::WAITING)
    {
        //
        Vertex* currVtx = WMGraph->getVertex(WMGraph->getIndex(c_x, c_y));
        Vertex* nextVtx = WMGraph->getVertex(path.front());

        bool isValid = false;
        switch(nextVtx->stat)
        {
            case Vertex::EMPTY:
                path.pop();
                c_x = nextVtx->x;
                c_y = nextVtx->y;
                currVtx->updateStat(Vertex::EMPTY);
                nextVtx->updateStat(Vertex::NONEMPTY);
                
                if(c_x == d_x && c_y == d_y)
                    stat = Client::IDLE;
                else
                    stat = Client::BUSY;
                
                break;
            case Vertex::NONEMPTY:
                stat = (stat == Client::WAITING) ? Client::BLOCKED : Client::WAITING;
                //cout << "[Rep]"; 
                
                break;
            case Vertex::BLOCKED:
                stat = (stat == Client::WAITING) ? Client::BLOCKED : Client::WAITING;
                cout << "Exception case1" << endl;
                //exit(0);
                break;
        }
    }
    else if(stat == Client::BLOCKED)
    {
        //
        if(createPath())
            stat = Client::BUSY;
    }
}

void Client::setID(int _id)
{
    id = _id;
}

void Client::setCurLoc(int x, int y)
{
    c_x = x, c_y = y;
}

void Client::setDesLoc(int x, int y)
{
    d_x = x, d_y = y;
}

void Client::updateStat(int u_stat)
{
    stat = u_stat;
}


bool Client::createPath()
{
    // Graph initializing
    int g_size = WMGraph->getWidth() * WMGraph->getHeight();
    router.init(g_size);

    //
    int start   = WMGraph->getIndex(c_x, c_y);
    int end     = WMGraph->getIndex(d_x, d_y);
    
    cout << "[Rep] Find shortest path from " << WMGraph->getVertex(start) << " to " << WMGraph->getVertex(end) << endl; 
    if(start == end)
    {
        stat = Client::IDLE;
        cout << "[Rep] already arrived" << endl;
    }
    else
    {
        path = router.findShortestPath(start, end);

        if(path.vertices.size() > 0)
        {
            Vertex* nextVtx = WMGraph->getVertex(path.pop());
            c_x = nextVtx->x;
            c_y = nextVtx->y;
            nextVtx->updateStat(Vertex::NONEMPTY);
            
            stat = Client::BUSY;
        }
    }
    
    
    return true;
}



