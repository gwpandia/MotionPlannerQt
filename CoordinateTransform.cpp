#include "CoordinateTransform.h"
#include <cmath>
#include "Constant.h"

using namespace IMMP;

QVector<QPointF> CoordinateTransform::toPlannerPoints(const QVector<QPointF>& points, const IMMP::Configuration& config, double swp){
	QVector<QPointF> transformedPoints;
	for(int i = 0; i < points.size(); i++){
		transformedPoints.push_back(toPlannerPoint(points.at(i), config, swp));
	}
	return transformedPoints;
}

QPointF CoordinateTransform::toPlannerPoint(const QPointF& point, const IMMP::Configuration& config, double swp){
	double x = (((std::cos(toRadian(config.getTheta())) * point.x() - std::sin(toRadian(config.getTheta())) * point.y()) * swp) + (config.getX() * swp));
	double y = (((std::sin(toRadian(config.getTheta())) * point.x() + std::cos(toRadian(config.getTheta())) * point.y()) * swp) + (config.getY() * swp));
	return QPointF(x, y);
}

QVector<QPoint> CoordinateTransform::toCanvasPoints(const QVector<QPointF>& points, const IMMP::Configuration& config, double swp, double spc){
	QVector<QPoint> transformedPoints;
	for(int i = 0; i < points.size(); i++){
		transformedPoints.push_back(toCanvasPoint(points.at(i), config, swp, spc));
	}
	return transformedPoints;
}

QPoint CoordinateTransform::toCanvasPoint(const QPointF& point, const IMMP::Configuration& config, double swp, double spc){
	int x = (int)(((std::cos(toRadian(config.getTheta())) * point.x() - std::sin(toRadian(config.getTheta())) * point.y()) * swp * spc) + (config.getX() * swp * spc));
	int y = (-1 * (int)((std::sin(toRadian(config.getTheta())) * point.x() + std::cos(toRadian(config.getTheta())) * point.y()) * swp * spc) + (IMMP::Constant::HC-(int)(config.getY() * swp * spc)));
	return QPoint(x, y);
}