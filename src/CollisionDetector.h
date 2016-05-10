#pragma once
#include <QtGui/QtGui>
#include <vector>
#include "Robot.h"
#include "Obstacle.h"
//#include "tbb/tbb.h"
//#include "Constant.h"
//#include "CoordinateTransform.h"

namespace IMMP{
class CollisionDetector
{
public:
	static bool isCollide(const QPolygonF&, const QPolygonF&);
	static bool isRobotCollide(const Robot&, const Configuration&, const std::vector<Obstacle>&);
	static bool isAllCollide(const std::vector<Robot>&, const std::vector<Obstacle>&);
};
/*
class ApplyCollisionDetection{
public:
	int my_result;
	const Obstacle& obstacle;
	const QPolygonF& curPolygon;
	ApplyCollisionDetection(const Obstacle& o, const QPolygonF& p):my_result(0), obstacle(o), curPolygon(p){
		
	}

	ApplyCollisionDetection(ApplyCollisionDetection& acd, tbb::split) : my_result(0), obstacle(acd.obstacle), curPolygon(acd.curPolygon){

	}

	void operator()( const tbb::blocked_range<size_t>& r ){
		int result = my_result;
		//result = (result + my_result);

		for(int k = r.begin(); k < r.end(); k++){
			QPolygonF testPolygon(CoordinateTransform::toPlannerPoints(obstacle.getPolygonF(k), obstacle.getInitialConfiguration(), Constant::SWP));
			if(CollisionDetector::isCollide(curPolygon, testPolygon)){
				result += 1;
			}
		}
	
		my_result = result;
	}

	void join(const ApplyCollisionDetection& acd){
		my_result += acd.my_result;
	}

};
*/
};