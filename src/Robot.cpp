#include "Robot.h"

using namespace IMMP;

Robot::Robot(void)
    :totalControlPointWeights(0.0)
{
}


Robot::~Robot(void)
{
}

void Robot::setControlPointWeight(size_t i, double weight)
{
    if(i >= controlPointWeights.size()){
        return;
    }

    if(weight < 0.0 || weight > Constant::CONTROLPOINT_WEIGHT_MAX){
        return;
    }

    controlPointWeights.at(i) = weight;

    totalControlPointWeights = 0.0;

    for(size_t j = 0; j < controlPointWeights.size(); ++j){
        totalControlPointWeights += controlPointWeights.at(j);
    }
}
