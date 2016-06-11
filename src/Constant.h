#pragma once
namespace IMMP{
class Constant
{
public:
	static const int HC = 400;
	static const int BITMAPSIZE = 128;
	static const int OBST = BITMAPSIZE * 2 - 1;
	static const int INIT = OBST - 1;
	static const int GOAL = 0;
	static const int SWP = 1;
	static const int MAXCONFIG = 300;
	static const int MAXPOINT = 300;
	static const int POINTED = 253;
	static const int ANGLESPACE = 36;
	static const int XSPACE = BITMAPSIZE;
	static const int YSPACE = BITMAPSIZE;
	static const int POTENTIALMAX = OBST;
	static const int PERSTRUCT = 8000;
	static const int OCCUPIED = -1;
	static const int NONE = 0, ROBOT_I = 1, ROBOT_G = 2, OBS = 3, FILES = 8;
	static const int OUTOFBOUND = 2, COLLISION = 1, FREE = 0;
	static const bool NOTVISIT = false, VISIT = true;
	static const double SPC;// = 800.0/128.0;
	static const int potentialFieldCanvasScale = 2;
    //static const double PI;
    static const double CONTROLPOINT_WEIGHT_MAX;
};



};
