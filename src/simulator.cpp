#include "simulator.h"
#include "graph.h"
#include "server.h"


Simulator* Simulator::instance = nullptr;

Simulator* Simulator::inst()
{
    if(instance == nullptr)
        instance = new Simulator();
    return instance;
}

void Simulator::initDisp(int width, int height, bool **map)
{
    disp = CImgDisplay(width, height, "WaveM Self-driving Simulator (Made by DYKim)");

    background = CImg<unsigned char>(width, height, 1, 3, 0);
    for(int i=0; i < width; i++)
        for(int j=0; j < height; j++)
            if(map[i][j]) background.draw_point(i, j, white, 0.9);
}


void Simulator::showCurState()
{

    CImg<unsigned char> img(background);
    for(int i=0; i < WMServer->numClients(); i++)
    {
        Client* client = WMServer->getClient(i);


        int x1 = client->getX()-1;
        int x2 = client->getX()+1;
        int y1 = client->getY()-1;
        int y2 = client->getY()+1;

        img.draw_rectangle( x1, y1, x2, y2, colors[i%5], 1 );

        for(auto it=client->begin(); it != client->end(); it++)
        {
            int x = WMGraph->getVertex(*it)->x;
            int y = WMGraph->getVertex(*it)->y;

            img.draw_point(x, y, colors[i%5], 1);
        }
    }

    img.display(disp);

    while (!disp.is_closed() && !disp.is_keyESC() && !disp.is_keyQ()) {
        CImgDisplay::wait(disp);
    }
}


void Simulator::simFramePerSec()
{

    CImg<unsigned char> img0(background), img;

    while(!disp.is_closed() && !disp.is_keyQ() && !disp.is_keyESC()) 
    {
        img = img0;

        for(int i=0; i < WMServer->numClients(); i++)
        {
            Client* client = WMServer->getClient(i);


            int x1 = client->getX()-4;
            int x2 = client->getX()+4;
            int y1 = client->getY()-4;
            int y2 = client->getY()+4;

            img.draw_rectangle( x1, y1, x2, y2, colors[i%9], 1 );

            for(auto it=client->begin(); it != client->end(); it++)
            {
                int x = WMGraph->getVertex(*it)->x;
                int y = WMGraph->getVertex(*it)->y;

                img.draw_point(x, y, colors[i%9], 1);
            }
        }


        img0.resize(disp.display(img).resize(false).wait(1));

        // Allow the user to toggle fullscreen mode, by pressing CTRL+F.
        if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(WMGraph->getWidth()*2,WMGraph->getHeight()*2,false).toggle_fullscreen(false);

        WMServer->taskManage();
        WMServer->next();
    }







}

