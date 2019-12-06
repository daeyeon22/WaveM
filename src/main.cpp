#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>

#include "CImg.h"
#include "bitmap.h"
#include "graph.h"


using namespace cimg_library;
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

  
    // load the file. The constructor now does most of the work
    BitMap bmp(fileName.c_str()); 

    // get the vector <R,G,B> for the pixel at (1,1)
    std::vector<unsigned int> example_vector = bmp.getPixel(1,1);

    int g_width = bmp.width();
    int g_height = bmp.height();
    bool** map = new bool*[bmp.width()];

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    for(int i=0; i < bmp.width(); i++)
    {
        map[i] = new bool[bmp.height()];

        for(int j=0; j < bmp.height(); j++)
        {
            vector<unsigned> RGB = bmp.getPixel(i,j);

            if(RGB[0] == 0 && RGB[1] == 0 && RGB[2] == 0)
                map[i][j] = false;
            else
                map[i][j] = true;

        }

    }


    // initialize graph
    graph->init(g_width, g_height, map);
    cout << "[Rep] Graph Initialization Done (" << g_width << " " << g_height << ")" <<  endl;
    //
    

    static const unsigned char  yellow[] = { 255,255,0 }, white[] = { 255,255,255 },
                 green[] = { 0,255,0 }, blue[] = { 120,200,255 },
                 purple[] = { 255,100,255 }, black[] = { 0,0,0 };



    CImgDisplay disp( g_width, g_height, "WaveM Self-driving Simulator (Made by DYKim)");
    CImg<unsigned char> background( g_width, g_height, 1, 3, 255);


    // draw bitmap img
    for(int i=0; i < bmp.width(); i++)
    {
        for(int j=0; j < bmp.height(); j++)
        {
            if(map[i][j])
                background.draw_point(i, j, black, 0.9);
        }
    }

    background.display(disp);

    while (!disp.is_closed() && !disp.is_keyESC() && !disp.is_keyQ()) {
        CImgDisplay::wait(disp);
    }
    




    cout << "[Rep] Program Finished" << endl;
}
