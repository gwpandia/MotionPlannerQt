#pragma once
#include "Constant.h"
#include "Robot.h"
#include "ObstacleBitmap.h"

namespace IMMP{
class PotentialField
{
public:
	PotentialField(int, int, const ObstacleBitmap&);
	~PotentialField(void);
	int getPotentialValue(int, int)const;

	int getMaxVal()const{
		return maxVal;
	}

	int getMinVal()const{
		return minVal;
	}

private:
	void NF1();
	void updateMinMaxVal(int);
	int goalx;
	int goaly;
	int maxVal;
	int minVal;
	int potentialfield[Constant::BITMAPSIZE][Constant::BITMAPSIZE];
};

class Config{
public:
	Config(int x, int y, int d):x(x), y(y), depth(d){

	}

	int x;
	int y;
	int depth;
};


};