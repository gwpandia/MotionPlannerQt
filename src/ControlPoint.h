#pragma once

namespace IMMP{
class ControlPoint
{
public:
	ControlPoint(double x, double y);
	~ControlPoint(void);
	double getX()const{
		return x;
	}
	double getY()const{
		return y;
	}

private:
	double x;
	double y;
};

};