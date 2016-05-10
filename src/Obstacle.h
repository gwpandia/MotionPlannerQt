#pragma once
#include "Configuration.h"
#include "ControlPoint.h"
#include <QtGui/QtGui>
#include <vector>

namespace IMMP{
class Obstacle
{
public:
	Obstacle(void);
	~Obstacle(void);

	void setInitialConfiguration(Configuration conf){
		initialConfig = conf;
	}

	Configuration getInitialConfiguration() const{
		return initialConfig;
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
	std::vector<QPolygonF> polygons;
};

};