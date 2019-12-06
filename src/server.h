

#ifndef __SERVER__
#define __SERVER__
#include "router.h"

class Client
{
    enum Status { BUSY, IDLE, BLOCKED };

  private:
    int id;
    int stat;               // current status
    int c_x, c_y;           // current position
    int d_x, d_y;           // destination, those values exist if only if status is BUSY or BLOCKED
    Path path;              // routing path
    Router router;          // router

  public:

    Client(int id, int stat) :
        id(id), stat(stat) {}

    void setCurLoc(int x, int y);
    void setDesLoc(int x, int y);
    void updateStat(int u_stat);
    
    bool createPath();
};


class Server
{
  private:
    static Sercver* instance;
    vector<Client> clients;

  public:
    static Server* inst();
  
    void initClients(int numClients);
    void addClient();
};



#endif
