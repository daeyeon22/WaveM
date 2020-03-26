#ifndef __SERVER__
#define __SERVER__
#include "router.h"
#include "graph.h"
#include <queue>
#include <string>
#include <fstream>

#define WMServer Server::inst()

class Client
{
  private:
    int id;
    int stat;               // current status
    int c_x, c_y;           // current position
    int d_x, d_y;           // destination, those values exist if only if status is BUSY or BLOCKED
    int waitCnt;            // waiting count
    int waitClient;
    int sizeX, sizeY;
    int speed;
    Path path;              // routing path
    Router router;          // router
    string name;

  public:
    enum { BUSY, IDLE, BLOCKED, WAITING };

    Client(): id(0), stat(Client::IDLE) {}
    Client(int id, int stat) :
        id(id), stat(stat) {}

    void setName(string _name);
    void setID(int _id);
    void setCurLoc(int x, int y);
    void setDesLoc(int x, int y);
    void updateStat(int u_stat);
    void setSize(int _sizeX, int _sizeY);
    void setSpeed(int _speed);
    bool createPath();
    bool arrival();
    bool idle();

    int getID(){ return id; }
    int getX(){ return c_x; }
    int getY(){ return c_y; }
    int getdX(){ return d_x; }
    int getdY(){ return d_y; }
    int getStat(){ return stat; }
    int getSizeX(){ return sizeX; }
    int getSizeY(){ return sizeY; }
    int getSpeed(){ return speed; }
    int getSpacingX();
    int getSpacingY();

    vector<int>::iterator begin(){ return path.vertices.begin(); };
    vector<int>::iterator end(){ return path.vertices.end(); };

    Vertex* getCurrent();
    Vertex* getNext();
    int getNextState();
    int getBlockingClient();
    void waitCountDown();

    void moveAt(Vertex* currVtx, Vertex* nextVtx);
    void next();

    string getName(){ return name; }
};


class Server
{
  private:
    static Server* instance;    
    vector<Client> clients;     // clients
    queue<int> tasks;           // destination queue
    queue<int> idles;
    int time;


  public:
    static Server* inst();

    void tictok();
    void toIDLE(int id);

    int getTime();
    int getNextTask();
    int getIDLEClient();
    int numTasks();
    int numIDLEs();

    // parser
    void readConfig(const char *inFile);
    void readClient(ifstream& is, string clientName);
    void readTask(ifstream& is);

    //
    void initServer();
    void initTasks(int _numTasks);
    void initClients(int _numClients);
    void addClient();
    void taskManage();

    // getter
    int numClients(){ return clients.size(); }
    Client* getClient(int id){ return &clients[id]; }

    void next();
     
};



#endif
