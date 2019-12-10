#ifndef __SERVER__
#define __SERVER__
#include "router.h"

#define WMServer Server::inst()

class Client
{
  private:
    int id;
    int stat;               // current status
    int c_x, c_y;           // current position
    int d_x, d_y;           // destination, those values exist if only if status is BUSY or BLOCKED
    Path path;              // routing path
    Router router;          // router

  public:
    enum { BUSY, IDLE, BLOCKED, WAITING };

    Client(){}
    Client(int id, int stat) :
        id(id), stat(stat) {}

    void setID(int _id);
    void setCurLoc(int x, int y);
    void setDesLoc(int x, int y);
    void updateStat(int u_stat);
    bool createPath();

    void next();

    int getX(){ return c_x; }
    int getY(){ return c_y; }
    int getStat(){ return stat; }
    vector<int>::iterator begin(){ return path.vertices.begin(); };
    vector<int>::iterator end(){ return path.vertices.end(); };

};


class Server
{
  private:
    static Server* instance;
    vector<Client> clients;

  public:
    static Server* inst();
  
    void initClients(int _numClients);
    void addClient();
   
    // getter
    int numClients(){ return clients.size(); }
    Client* getClient(int id){ return &clients[id]; }

    void next();
     
};



#endif
