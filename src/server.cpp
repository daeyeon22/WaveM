#include "server.h"



// Singleton 
Server* Server::instance == nullptr;

Server* Server::inst()
{
    if(instance == nullptr)
        instance = new Server();
    return instance;
}


void Server::addClient()
{
    clients.push_back(Client((int)clients.size(), Client::Status::IDLE));

}





