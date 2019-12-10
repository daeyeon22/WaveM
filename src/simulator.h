#ifndef __SIMULATOR__
#define __SIMULATOR__

#include "CImg.h"
#include "server.h"

#define WMSim Simulator::inst()

using namespace cimg_library;
using namespace std;

static const unsigned char  yellow[] = { 255,255,0 }, white[] = { 255,255,255 },
                            green[] = { 0,255,0 }, blue[] = { 120,200,255 },
                            purple[] = { 255,100,255 }, black[] = { 0,0,0 },
                            red[] = { 255, 0, 0 };

class Simulator
{
  private:
    unsigned char colors[9][3] = {{255,255,0}, {0,255,0}, {255,100,255}, {255,0,0}, {120,200,255}, 
                                  {255,127,14}, {255,152,150}, {197,176,213}, {114,158,206}};
    static Simulator* instance;
    CImgDisplay disp;
    CImg<unsigned char> background;

  public:
    static Simulator* inst();

    void initDisp(int width, int height, bool** map);
    void showCurState();
    void simFramePerSec();

};



#endif
