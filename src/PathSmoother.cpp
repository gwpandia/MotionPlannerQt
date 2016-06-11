#include "PathSmoother.h"
#include "CollisionDetector.h"
#include "Util.h"
#include "Constant.h"
#include <cmath>


using namespace IMMP;

PathSmoother::PathSmoother(SimpleModel& model) : model(model)
{
}


PathSmoother::~PathSmoother()
{
}

std::vector<Configuration> PathSmoother::smooth(const std::vector<Configuration>& originPath){
	std::vector<Configuration> path;
	smoothHelper(0, originPath.size() - 1, path, originPath);
	return path;
}

void PathSmoother::smoothHelper(size_t start, size_t end, std::vector<Configuration>& path, const std::vector<Configuration>& originPath){
	if(start == end){
		path.push_back(originPath.at(end));
		return;
	}
	else{
		std::vector<Configuration> tmpPath;
		generatePath(end-start+1, originPath.at(start), originPath.at(end), tmpPath);
		//tmpPath.push_back(originPath.at(end));
		if(!checkAll(tmpPath)){
			int bp = 2;// + rand() % 2;
            size_t mid = (start+end)/bp;
			smoothHelper(start, mid, path, originPath);
			smoothHelper(mid+1, end, path, originPath);
		}
		else{
			for(int i = 0; i < tmpPath.size(); i++){
				path.push_back(tmpPath.at(i));
			}
		}
	}
}

void PathSmoother::generatePath(size_t steps, const Configuration& start, const Configuration& end, std::vector<Configuration>& path){
	double dx = (end.getX() - start.getX()) / (double)steps;
	double dy = (end.getY() - start.getY()) / (double)steps;
	double da = (end.getTheta() - start.getTheta()) / (double)steps;

	for(int i = 0; i <= steps; i++){
		double xi = start.getX() + i * dx;
		double yi = start.getY() + i * dy;
		double ai = start.getTheta() + i * da;
		path.push_back(Configuration(xi, yi, ai));
	}
	//path.push_back(end);
}

bool PathSmoother::checkAll(const std::vector<Configuration>& path)const{
	for(int i = 0; i < path.size(); i++){
		if(!model.getCSpacePolygon().empty()){
			int x = (int)(path.at(i).getX() + 0.5);
			int y = (int)(path.at(i).getY() + 0.5);
			int a = (int)(path.at(i).getTheta() + 0.5);
			x = Util<int>::adjustRange(x, 0, Constant::XSPACE);
			y = Util<int>::adjustRange(y, 0, Constant::YSPACE);
			a = Util<int>::adjustRange(a, 0, Constant::ANGLESPACE);
			if(model.isCSpace(x, y, a)){
				return false;
			}
		}
		else{
			if(CollisionDetector::isRobotCollide(model.getRobots().at(0), path.at(i), model.getObstacles())){
				return false;
			}
		}
	}
	return true;
}
