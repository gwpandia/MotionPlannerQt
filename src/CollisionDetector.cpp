#include "CollisionDetector.h"
#include "CoordinateTransform.h"
#include "Constant.h"
//#include "tbb/tbb.h"

using namespace IMMP;

bool CollisionDetector::isCollide(const QPolygonF& cur, const QPolygonF& test){
	if(!cur.boundingRect().contains(test.boundingRect()) &&
		!test.boundingRect().contains(cur.boundingRect()) &&
		!cur.boundingRect().intersects(test.boundingRect())){
			return false;
	}

	for(int i = 0; i < cur.size(); i++){
		int s = (i+1) % cur.size();
		QLineF c_line(cur.at(i), cur.at(s));
		if(test.containsPoint(cur.at(i), Qt::OddEvenFill)){
			return true;
		}

		for(int j = 0; j < test.size(); j++){
			int t = (j+1) % test.size();
			QLineF t_line(test.at(j), test.at(t));
			if(cur.containsPoint(test.at(j), Qt::OddEvenFill)){
				return true;
			}
			if(c_line.intersect(t_line, NULL) == QLineF::BoundedIntersection){
				return true;
			}
		}
	}
	return false;
}

bool CollisionDetector::isRobotCollide(const Robot& robot, const Configuration& config, const std::vector<Obstacle>& obstacles){
	//int sum = 0;
	for(int i = 0; i < robot.NPolygons(); i++){
		QPolygonF curPolygon(CoordinateTransform::toPlannerPoints(robot.getPolygonF(i), config, Constant::SWP));
		for(int j = 0; j < obstacles.size(); j++){
			for(int k = 0; k < obstacles.at(j).NPolygons(); k++){
				QPolygonF testPolygon(CoordinateTransform::toPlannerPoints(obstacles.at(j).getPolygonF(k), obstacles.at(j).getInitialConfiguration(), Constant::SWP));
				if(isCollide(curPolygon, testPolygon)){
					return true;
					//sum += 1;
				}
			}
			/*ApplyCollisionDetection acd(obstacles.at(j), curPolygon);
			tbb::parallel_reduce(tbb::blocked_range<size_t>(0, obstacles.at(j).NPolygons()), acd);
			sum += acd.my_result;*/
		}
	}
	return false;
	//return sum != 0;
}

bool CollisionDetector::isAllCollide(const std::vector<Robot>& robots, const std::vector<Obstacle>& obstacles){
	for(int i = 0; i < robots.size(); i++){
		if(isRobotCollide(robots.at(i), robots.at(i).getInitialConfiguration(), obstacles)){
			return true;
		}
		if(isRobotCollide(robots.at(i), robots.at(i).getGoalConfiguration(), obstacles)){
			return true;
		}
	}
	return false;
}