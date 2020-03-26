#include "server.h"
#include "graph.h"
#include <boost/format.hpp>
#include <cmath>
#include <set>





inline void splitAsTokens(const string& str, const string& delim, vector<string> &_tokens){
    _tokens.clear();
    size_t start, end=0;
    while(end < str.size()){
        start = end;
        while(start < str.size() && (delim.find(str[start]) != string::npos)){
            start++;
        }
        end = start;
        while(end < str.size() && (delim.find(str[end]) == string::npos)){
            end++;
        }
        if(end-start != 0){
            _tokens.push_back(string(str, start, end-start));
        }
    }
}

ostream& operator << (ostream& os, Vertex* vtx)
{
    return os << "(" << vtx->x << " " << vtx->y << ")";
}


ostream& operator << (ostream& os, Client* client)
{
    os << boost::format("[Rep] CLIENT %s\n") % client->getName();
    os << boost::format("   - Size  : ( %d x %d )\n") % client->getSizeX() % client->getSizeY();
    os << boost::format("   - Start : ( %d , %d )\n") % client->getX() % client->getY();
    os << boost::format("   - Speed : %d [pxl/sec]\n\n") % client->getSpeed();
}


// Singleton 
Server* Server::instance = nullptr;

Server* Server::inst()
{
    if(instance == nullptr)
        instance = new Server();
    return instance;
}

//

void Server::readClient(ifstream& is, string clientName)
{
    int i = clients.size();
    clients.push_back(Client());

    Client* client = getClient(i);
    client->setName(clientName);
    client->setID(i);
    client->updateStat(Client::IDLE);

    string line = "";
    string delim = " ";
    vector<string> tokens;


    bool hasStartLoc = false;

    while(!is.eof())
    {
        if(!getline(is, line))
            continue;

        splitAsTokens(line, delim, tokens);

        if(tokens[0] == "END")
            break;
        else if(tokens[0] == "SIZE")
        {
            int sizeX = stoi(tokens[1]);
            int sizeY = stoi(tokens[2]);
            client->setSize(sizeX, sizeY);
        }
        else if(tokens[0] == "SPEED")
        {
            int speed = stoi(tokens[1]);
            client->setSpeed(speed);
        }
        else if(tokens[0] == "START")
        {
            int c_x = stoi(tokens[1]);
            int c_y = stoi(tokens[2]);

            client->setCurLoc(c_x, c_y);
            hasStartLoc = true;
        }
    }

    if(!hasStartLoc)
    {
        int spacingX = ceil( 1.0 * client->getSizeX() / 2 );
        int spacingY = ceil( 1.0 * client->getSizeY() / 2 );
        Vertex* vBeg = WMGraph->getVertex(WMGraph->randVertex(Vertex::EMPTY, spacingX, spacingY));
        client->setCurLoc(vBeg->x, vBeg->y);
        vBeg->updateStat(Vertex::NONEMPTY, client->getID());
    
        
    }

    cout << client;
}


void Server::readTask(ifstream& is)
{
    
    // do parse


    //


    return;
}


void Server::initServer()
{
    time = 0;
}



void Server::readConfig(const char* inFile)
{

    ifstream is(inFile);

    if(!is.good())
    {
        cout << "[Error] Cannot open " << inFile << endl;
        exit(0);
    }


    string line = "";
    string delim = " ";


    vector<string> tokens;

    while(!is.eof())
    {
        if(!getline(is, line))
            continue;


        splitAsTokens(line, delim, tokens);

        if(tokens[0] == "NUM_CLIENT")
        {
            cout << "[Arg] #Clients : " << tokens[1] << endl;

        }
        else if(tokens[0] == "CLIENT")
        {
            readClient(is, tokens[1]);
        }
        else if(tokens[0] == "TASK")
        {
            readTask(is);
        }
    }

    int numTasks = tasks.size();

    if(numTasks == 0)
    {
        cout << "[Arg] task does not exist, initialize randomly" << endl;
        initTasks(100);
    }
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
    // 
    tictok();
    
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
        //int d_v = WMGraph->randVertex(Vertex::EMPTY, 10, 10);
        //tasks.push(d_v);
        tasks.push(Vertex::EMPTY);
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
   
    // To do something
    return WMGraph->randVertex(Vertex::EMPTY, 10, 10);

    //return task;
}

int Server::getIDLEClient()
{
    int clientID = idles.front();
    idles.pop();
    return clientID;
}


void Server::taskManage()
{
    //cout << boost::format("[Server] Task Management\n");
    
    while(numTasks() > 0 && numIDLEs() > 0)
    {
        Vertex* vEnd    = WMGraph->getVertex(getNextTask());
        Client* client  = getClient(getIDLEClient()); 
        client->setDesLoc(vEnd->x, vEnd->y);
        
        
        if(!client->createPath())
        {
            cout << "[Warn] destination " << vEnd << " is not accesible..." << endl;
            client->updateStat(Client::IDLE);
        }
        else
        {
            cout << "     <Assign Task>\n";
            cout << "     - Client : " << client->getName() << endl; 
            cout << "     - Task   : (" << client->getX() << " " << client->getY() << ") to (" << client->getdX() << " " << client->getdY() << ")";
            cout << endl << endl;
            //%d %d) to (%d %d)") % client->getX() % client->getY() % client->getdX() % client->getdY();
        }

    }
}


int Server::getTime()
{
    return time;
}


void Server::tictok()
{
    time++;
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

int Client::getSpacingX()
{
    return ceil(1.0* sizeX / 2);
}

int Client::getSpacingY()
{
    return ceil(1.0* sizeY / 2);
}


Vertex* Client::getCurrent()
{
    return WMGraph->getVertex(WMGraph->getIndex(c_x, c_y));
}


Vertex* Client::getNext()
{
    vector<int>::iterator it = path.begin();
    it += (speed-1);

    return WMGraph->getVertex(*it);
}

void Client::moveAt(Vertex* currVtx, Vertex* nextVtx)
{
    while(true)
    {
        if(path.pop() == nextVtx->id)
            break;
    }

    // goto next
    c_x = nextVtx->x;
    c_y = nextVtx->y;

    int c_lx = currVtx->x - getSpacingX();
    int c_ux = currVtx->x + getSpacingX();
    int c_ly = currVtx->y - getSpacingY();
    int c_uy = currVtx->y + getSpacingY();

    int n_lx = nextVtx->x - getSpacingX();
    int n_ux = nextVtx->x + getSpacingX();
    int n_ly = nextVtx->y - getSpacingY();
    int n_uy = nextVtx->y + getSpacingY();


    // map update
    WMGraph->updateStat(Vertex::EMPTY, c_lx, c_ly, c_ux, c_uy);
    WMGraph->updateStat(Vertex::NONEMPTY, n_lx, n_ly, n_ux, n_uy, getID());

    // stat update
    if(arrival())
    {
        
        updateStat(Client::IDLE);
    }
    else
        updateStat(Client::BUSY);

}

void Client::waitCountDown()
{
    waitCnt--;
}

int Client::getBlockingClient()
{
    set<int> nodes; // all passing nodes

    for(vector<int>::iterator it = path.begin(); it != path.begin() + speed; it++)
    {
        Vertex* currVtx = WMGraph->getVertex(*it);

        int c_lx = currVtx->x - getSpacingX();
        int c_ux = currVtx->x + getSpacingX();
        int c_ly = currVtx->y - getSpacingY();
        int c_uy = currVtx->y + getSpacingY();

        for(Vertex* aroundVtx : WMGraph->getInside(c_lx, c_ly, c_ux, c_uy))
        {
            nodes.emplace(aroundVtx->id);
        }
    }
    
    for(auto nodeID : nodes)
    {
        Vertex* currVtx = WMGraph->getVertex(nodeID);
        if(currVtx->stat == Vertex::NONEMPTY && currVtx->client !=id)
        {
            return currVtx->client;
        }
    }

    return INT_MAX;
}


int Client::getNextState()
{

    if(stat == Client::BLOCKED)
    {
        return Client::BLOCKED;
    }
    else
    {
        set<int> nodes; // all passing nodes
        
        for(vector<int>::iterator it = path.begin(); it != path.begin() + speed; it++)
        {
            Vertex* currVtx = WMGraph->getVertex(*it);

            int c_lx = currVtx->x - getSpacingX();
            int c_ux = currVtx->x + getSpacingX();
            int c_ly = currVtx->y - getSpacingY();
            int c_uy = currVtx->y + getSpacingY();

            for(Vertex* aroundVtx : WMGraph->getInside(c_lx, c_ly, c_ux, c_uy))
            {
                nodes.emplace(aroundVtx->id);
            }
        }

        for(auto nodeID : nodes)
        {
            Vertex* currVtx = WMGraph->getVertex(nodeID);
            if(currVtx->stat == Vertex::NONEMPTY && currVtx->client !=id)
            {
                if(stat == Client::WAITING)
                    if(waitCnt < 0) 
                        return Client::BLOCKED;
                    else
                        return Client::WAITING; 
                else
                    return Client::WAITING;
            }
            else if(currVtx->stat == Vertex::BLOCKED)
            {
                return Client::BLOCKED;
            }
        }

        return Client::BUSY;
    }
}




void Client::next()
{
    switch(getNextState())
    {
        case Client::BUSY:
            // if it is busy, move
            updateStat(Client::BUSY);
            moveAt(getCurrent() /* from */, getNext() /* to */);
            break;
        case Client::WAITING:
            // if it is waiting, count down until path is empty
            if(stat == Client::WAITING)
                waitCountDown();
            else
                updateStat(Client::WAITING);

            break;
        case Client::BLOCKED:
            if(createPath())
                // find new path
                updateStat(Client::BUSY);
            else
                // if failed, remain as idle... 
                updateStat(Client::IDLE);
            break;
        default: 
            cout << "!!!!!!!!!!!!!!????" << endl;
                 break;
    }


    /* OLD  
    // do something if job exists
    if(stat == Client::BUSY || stat == Client::WAITING)
    {


        
        //
        Vertex* currVtx = getCurrent(); //WMGraph->getVertex(WMGraph->getIndex(c_x, c_y));
        Vertex* nextVtx = getNext(); //WMGraph->getVertex(path.front());

        int c_lx, c_ux, c_ly, c_uy;
        int n_lx, n_ux, n_ly, n_uy;

        bool isValid = false;
        switch(nextVtx->stat)
        {
            case Vertex::EMPTY:
                moveAt(currVtx, nextVtx);
                break;

            case Vertex::NONEMPTY:
               
                if(nextVtx->client == getID())      
                    moveAt(currVtx, nextVtx);
                else if(stat == Client::WAITING)
                    if(waitCnt-- < 0) updateStat(Client::BLOCKED);
                else 
                    updateStat(Client::WAITING);
                break;
            case Vertex::BLOCKED:
                
                if(stat == Client::WAITING) 
                    updateStat(Client::BLOCKED);
                else 
                    updateStat(Client::WAITING);
                cout << "Exception case1" << endl;
                break;
        }
    }
    else if(stat == Client::BLOCKED)
    {
        //
        if(createPath()) updateStat(Client::BUSY);
    }
    */
}

bool Client::idle()
{
    return stat == Client::IDLE ? true : false;
}

void Client::setName(string _name)
{
    name = _name;
}

void Client::setSize(int _sizeX, int _sizeY)
{
    sizeX = _sizeX, sizeY = _sizeY;
}

void Client::setSpeed(int _speed)
{
    speed = _speed;
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
        cout << boost::format("[%s] updated status from %s to %s\n") % name % statToStr(stat) % statToStr(u_stat);


        /*
        switch(u_stat)
        {
            case Client::BUSY:
                cout << "   <Assign Task>" << endl;
                break;
            case Client::IDLE:
                cout << "   <Waiting Task>" << endl;
                break;
            case Client::WAITING:
                cout << "   <Waiting Client>" << endl;
                break;
            case Client::BLOCKED:
                cout << "   <Path Blocked, Find New Path>" << endl;
                break;
        }
        */

    }
   

    if(u_stat == Client::WAITING)
    {

        if(stat == Client::BUSY)
        {
            int blkClient = getBlockingClient();

            if(waitClient == blkClient)
            {
                stat = Client::BLOCKED;
                return;
            }
            else
            {
                waitCnt = rand() % 10;
                waitClient = blkClient;
            }
        }
        else if(stat == Client::WAITING)
        {
        
        }
        else if(stat == Client::BLOCKED)
        {
            // exceprtion case
            cout << "[Err] exception case1" << endl;
            exit(0);
        }
        else if(stat == Client::IDLE)
        {
            // exception case
            cout << "[Err] exception case2" << endl;
            exit(0);
        }

        stat = Client::WAITING;
    }
    else if(u_stat == Client::IDLE)
    {
        //
        WMServer->toIDLE(id);
        stat = Client::IDLE;
    }
    else if(u_stat == Client::BLOCKED)
    {
        //
        stat = Client::BLOCKED;
    }
    else if(u_stat == Client::BUSY)
    {
        //
        stat = Client::BUSY;
    }
    else
    {
        // exception case
        cout << "[Err] exception case3" << endl;
        exit(0);
    }


    //stat = u_stat;
}


bool Client::createPath()
{
    // Graph initializing
    int g_size = WMGraph->getWidth() * WMGraph->getHeight();
    router.init(g_size, id); //sizeX, sizeY);

    
    //
    int start   = WMGraph->getIndex(c_x, c_y);
    int end     = WMGraph->getIndex(d_x, d_y);
   


    cout << "[" << name << "] find shortest path from " << WMGraph->getVertex(start) << " to " << WMGraph->getVertex(end) << endl; 

    if(start == end)
    {
        updateStat(Client::IDLE);
        cout << "[Rep] already arrived" << endl;
        return false;
    }
    else
    {
        path = router.findShortestPath(start, end);

        if(path.vertices.size() > 0)
        {
            Vertex* nextVtx = WMGraph->getVertex(path.pop());
            c_x = nextVtx->x;
            c_y = nextVtx->y;
            int   n_lx = nextVtx->x - getSpacingX();
            int   n_ux = nextVtx->x + getSpacingX();
            int   n_ly = nextVtx->y - getSpacingY();
            int   n_uy = nextVtx->y + getSpacingY();
            // map update
            WMGraph->updateStat(Vertex::NONEMPTY, n_lx, n_ly, n_ux, n_uy, getID());
 
            //nextVtx->updateStat(Vertex::NONEMPTY);
            updateStat(Client::BUSY);
            
            return true;
        }
    }
    
    
    return false;
}



