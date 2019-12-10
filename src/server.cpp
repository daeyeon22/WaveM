#include "server.h"
#include "graph.h"
#include <boost/format.hpp>

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
        //Vertex* vEnd = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY));

        clients[i].setID(i);
        clients[i].updateStat(Client::IDLE);
        clients[i].setCurLoc(vBeg->x, vBeg->y);
        //clients[i].setDesLoc(vEnd->x, vEnd->y);
        //clients[i].createPath();
    }
}

void Server::next()
{
    for(int i=0; i < numClients(); i++)
    {
        getClient(i)->next();
    }
}

void Server::initTasks(int _numTasks)
{
    // initialize 100 tasks (pick random) 
    for(int t=0; t < _numTasks; t++)
    {
        int d_v = WMGraph->randVertex(Vertex::EMPTY);
        tasks.push(d_v);
    }
}

int Server::numTasks()
{
    return tasks.size();
}

int Server::numIDLEs()
{
    return idles.size();
}

int Server::getNextTask()
{
    int task = tasks.front();
    tasks.pop();
    return task;
}

int Server::getIDLEClient()
{
    int clientID = idles.front();
    idles.pop();
    return clientID;
}


void Server::taskManage()
{
    while(numTasks() > 0 && numIDLEs() > 0)
    {
        Vertex* vEnd    = WMGraph->getVertex(getNextTask());
        Client* client  = getClient(getIDLEClient()); 
        
        client->setDesLoc(vEnd->x, vEnd->y);
        client->createPath();
    }
}

void Server::toIDLE(int id)
{
    idles.push(id);
}



/* Client */
bool Client::arrival()
{
    return (c_x == d_x && c_y == d_y) ? true : false;
}


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
                
                // goto next
                c_x = nextVtx->x;
                c_y = nextVtx->y;
                
                // map update
                currVtx->updateStat(Vertex::EMPTY);
                nextVtx->updateStat(Vertex::NONEMPTY);
                
                // stat update
                if(arrival())
                    updateStat(Client::IDLE);
                else
                    updateStat(Client::BUSY);
                
                break;

            case Vertex::NONEMPTY:
                
                if(stat == Client::WAITING)
                {
                    if(waitCnt-- < 0)
                    {
                        updateStat(Client::BLOCKED);
                    }
                }   
                else 
                    updateStat(Client::WAITING);
                //cout << "[Rep]"; 
                
                break;
            case Vertex::BLOCKED:
                
                if(stat == Client::WAITING)
                    updateStat(Client::BLOCKED);
                else 
                    updateStat(Client::WAITING);
                cout << "Exception case1" << endl;
                //exit(0);
                break;
        }
    }
    else if(stat == Client::BLOCKED)
    {
        //
        if(createPath()) updateStat(Client::BUSY);
    }
}

bool Client::idle()
{
    return stat == Client::IDLE ? true : false;
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

string statToStr(int stat)
{
    switch(stat)
    {
        case Client::BUSY: return "BUSY";
        case Client::IDLE: return "IDLE";
        case Client::BLOCKED: return "BLOCKED";
        case Client::WAITING: return "WAITING";
    }

    return "";
}


void Client::updateStat(int u_stat)
{
    if(stat!=u_stat)
    {
        cout << boost::format("[Rep] client%d status from %s to %s\n") % id % statToStr(stat) % statToStr(u_stat);
        if(u_stat == Client::WAITING)
        {
            waitCnt = rand() % 10;
        }
    }
    
    if(u_stat == Client::IDLE)
    {
        WMServer->toIDLE(id);
    }
    
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
        updateStat(Client::IDLE);
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
            updateStat(Client::BUSY);
        }
    }
    
    
    return true;
}



