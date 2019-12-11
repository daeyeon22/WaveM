#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

#include "bitmap.h"
#include "bitmap_image.hpp"
#include "graph.h"
#include "router.h"
#include "server.h"
#include "simulator.h"

using namespace std;

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv) {

    
    string fileName;
    
    if( cmdOptionExists(argv, argv+argc, "-map"))
    {        
        fileName =  getCmdOption(argv, argv+argc, "-map");
        cout << "[Arg] -map :"  << fileName << endl;
    }
    else
    {
        cout << "-map option must be required" << endl;
        exit(0);
    }

    ///////////have to see
    
    /*
    // load the file. The constructor now does most of the work
    BitMap bmp(fileName.c_str());
    */
    
    bitmap_image bmp(fileName.c_str());

    int g_width = bmp.width();
    int g_height = bmp.height();

    bool** map = new bool*[g_width];
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i=0; i < g_width; i++)
    {
        map[i] = new bool[g_height];

        for(int j=0; j < g_height; j++)
        {
            //vector<unsigned> RGB = bmp.getPixel(i,j);
            rgb_t RGB = bmp.get_pixel(i,j);

            //if(RGB[0] == 0 && RGB[1] == 0 && RGB[2] == 0)
            if(RGB.red == 0 && RGB.green == 0 && RGB.blue == 0)
                map[i][j] = false;
            else
                map[i][j] = true;

        }

    }
    //////////end here

    // initialize graph
    WMGraph->init(g_width, g_height, map);
    cout << "[Rep] Graph Initialization Done (" << g_width << " " << g_height << ")" <<  endl;
  
    WMServer->initTasks(100);
    WMServer->initClients(20);
    cout << "[Rep] Server Initialization Done" << endl;
    
    WMSim->initDisp(g_width, g_height, map);
    cout << "[Rep] Simulator Initialization Done" << endl;

    WMSim->simFramePerSec();
    cout << "[Rep] Program Finished" << endl;
}
