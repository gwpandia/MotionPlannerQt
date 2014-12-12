#pragma once

#include <QtGui/QtGui>
#include "Configuration.h"

namespace IMMP{

class CoordinateTransform
{
public:
	static QVector<QPointF> toPlannerPoints(const QVector<QPointF>&, const IMMP::Configuration&, double);
	static QPointF toPlannerPoint(const QPointF&, const IMMP::Configuration&, double);
	static QVector<QPoint> toCanvasPoints(const QVector<QPointF>&, const IMMP::Configuration&, double, double);
	static QPoint toCanvasPoint(const QPointF&, const IMMP::Configuration&, double, double);
	static double toRadian(double a){
		return (a/180.0)*M_PI;
	}
	static double toDegree(double a){
		return (a/M_PI)*180.0;
	}
	static double toPositiveDegree(double a){
		double ret = toDegree(a);
		while(ret < 0.0){
			ret += 360.0;
		}
		return ret;
	}
};

};