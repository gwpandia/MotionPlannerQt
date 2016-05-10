#pragma once

namespace IMMP{

class Configuration
{
public:
	Configuration();
	Configuration(double x, double y, double z);
	~Configuration(void);
	double getX()const;
	double getY()const;
	double getTheta()const;

private:
	double x;
	double y;
	double theta;
};

};