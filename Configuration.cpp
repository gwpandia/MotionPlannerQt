#include "Configuration.h"

using namespace IMMP;

Configuration::Configuration():x(0.0), y(0.0), theta(0.0){

}

Configuration::Configuration(double x, double y, double t)
{
	this -> x = x;
	this -> y = y;
	while(t < 0.0){
		t += 360.0;
	}
	while(t > 360.0){
		t -= 360.0;
	}
	this -> theta = t;

}


Configuration::~Configuration(void)
{
}

double Configuration::getX()const{
	return x;
}

double Configuration::getY()const{
	return y;
}

double Configuration::getTheta()const{
	return theta;
}