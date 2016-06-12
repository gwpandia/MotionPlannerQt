#pragma once
#include "Configuration.h"
#include "ControlPoint.h"
#include "Constant.h"
#include <QtGui/QtGui>
#include <vector>

namespace IMMP{


class Robot
{
public:
	Robot(void);
	~Robot(void);

	void setInitialConfiguration(Configuration conf){
		initialConfig = conf;
	}

	Configuration getInitialConfiguration() const{
		return initialConfig;
	}

	void setGoalConfiguration(Configuration conf){
		goalConfig = conf;
	}

	Configuration getGoalConfiguration() const{
		return goalConfig;
	}

    void addControlPoint(double x, double y, double weight = 1.0){
		controlPoints.push_back(ControlPoint(x, y));
        controlPointWeights.push_back(weight);
        totalControlPointWeights += weight;
	}

    ControlPoint getControlPoint(size_t i) const{
		return controlPoints.at(i);
	}

    double getTotalControlPointWeights() const{
        return totalControlPointWeights;
    }

    double getControlPointWeight(size_t i) const{
        if(i >= controlPointWeights.size()){
            return -1;
        }

        return controlPointWeights.at(i);
    }

    void setControlPointWeight(size_t i, double weight);

	size_t NControlPoints() const{
		return controlPoints.size();
	}

	void addPolygon(QPolygonF polygon){
		polygons.push_back(polygon);
	}

    QPolygonF getPolygonF(size_t i) const{
		return polygons.at(i);
	}

	size_t NPolygons()const{
		return polygons.size();
	}

private:

	IMMP::Configuration initialConfig;
	IMMP::Configuration goalConfig;
	std::vector<IMMP::ControlPoint> controlPoints;
    std::vector<double> controlPointWeights;
    std::vector<QPolygonF> polygons;
    double totalControlPointWeights;
};

};
