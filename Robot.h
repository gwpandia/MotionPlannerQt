#pragma once
#include "Configuration.h"
#include "ControlPoint.h"
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

	void addControlPoint(double x, double y){
		controlPoints.push_back(ControlPoint(x, y));
	}

	ControlPoint getControlPoint(int i) const{
		return controlPoints.at(i);
	}

	size_t NControlPoints() const{
		return controlPoints.size();
	}

	void addPolygon(QPolygonF polygon){
		polygons.push_back(polygon);
	}

	QPolygonF getPolygonF(int i) const{
		return polygons.at(i);
	}

	size_t NPolygons()const{
		return polygons.size();
	}

private:

	IMMP::Configuration initialConfig;
	IMMP::Configuration goalConfig;
	std::vector<IMMP::ControlPoint> controlPoints;
	std::vector<QPolygonF> polygons;
};

};