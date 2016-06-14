#include "BFS.h"
#include "CoordinateTransform.h"
#include "Util.h"
#include "CollisionDetector.h"
#include "Configuration.h"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
//#include <omp.h>

using namespace IMMP;

BFS::BFS(const Robot& robot, const std::vector<Obstacle>& obstacles):robot(robot), obstacles(obstacles), 
	cstructures(robot.NControlPoints() * Constant::POTENTIALMAX + 1), c_pools(robot.NControlPoints() * Constant::POTENTIALMAX + 1)
{
	useCSpace = false;
	//cspacePolygon.reserve(Constant::ANGLESPACE);
}

BFS::~BFS(void)
{
	destroyCStructure();
}

void BFS::init(){
	initializePotentialField();
	initializeVisitedTable();
}

std::vector<Configuration> BFS::findPath(){
	Configuration conf = robot.getInitialConfiguration();
	BFSTree *config = new BFSTree(conf, NULL);
	int pfv = getPotentialValue(conf);
	insertToStructure(pfv, config);
	visited[(int)conf.getX()][(int)conf.getY()][(int)(conf.getTheta()/360.0*Constant::ANGLESPACE)] = true;
	bool SUCCESS = false;
	double x_dis[] = {1, -1, 0, 0, 0, 0};
	double y_dis[] = {0, 0, 1, -1, 0, 0};
	double a_dis[] = {0, 0, 0, 0, 1, -1};
	BFSTree* goal = NULL;
	//bruteTestCollision();

	QPolygonF boundary;
	boundary.append(QPointF(0.0, 0.0));
    boundary.append(QPointF(0.0, Constant::BITMAPSIZE));
    boundary.append(QPointF(Constant::BITMAPSIZE, Constant::BITMAPSIZE));
    boundary.append(QPointF(Constant::BITMAPSIZE, 0.0));

	while(!isEmptyStructure() && !SUCCESS){
		BFSTree *x = getFirstNode();
		BFSTree *xnew = NULL;

		for(int i = 0; i < 6; i++){
			Configuration co(x->config.getX() + x_dis[i], x->config.getY() + y_dis[i], x->config.getTheta() + (a_dis[i] * 360.0 / Constant::ANGLESPACE));
			
			bool outOfBound = false;
			
			for(int j = 0; j < robot.NPolygons(); j++){
				QPolygonF cmpPolygon(IMMP::CoordinateTransform::toPlannerPoints(robot.getPolygonF(j), co, IMMP::Constant::SWP));
				for(int k = 0; k < cmpPolygon.size(); k++){
					if(!boundary.containsPoint(cmpPolygon.at(k), Qt::OddEvenFill)){
						outOfBound = true;
						break;
					}
				}
				if(outOfBound){
					break;
				}
			}

			if(!outOfBound && co.getX() < Constant::BITMAPSIZE && co.getX() >= 0 && co.getY() < Constant::BITMAPSIZE && co.getY() >= 0){
				if(!visited[(int)co.getX()][(int)co.getY()][(int)(co.getTheta()/360.0*Constant::ANGLESPACE)] && 
					((isUseCSpace() && !cspace[(int)co.getX()][(int)co.getY()][(int)(co.getTheta()/360.0*Constant::ANGLESPACE)]) || 
					   !CollisionDetector::isRobotCollide(robot, co, obstacles))
			      ){
					xnew = new BFSTree(co, x);
					insertToStructure(getPotentialValue(co), xnew);
					visited[(int)co.getX()][(int)co.getY()][(int)(co.getTheta()/360.0*Constant::ANGLESPACE)] = true;
					if(std::abs(getPotentialValue(co) - getPotentialValue(robot.getGoalConfiguration())) <= robot.NControlPoints()){
						goal = xnew;
						SUCCESS = true;
					}
				}

			}
		}

	}

	std::vector<Configuration> path;
	if(SUCCESS){
		BFSTree *xxx = goal;
		while(xxx != NULL){
			path.push_back(xxx->config);
			xxx = xxx->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

void BFS::initializePotentialField(){
	for(int i = 0; i < robot.NControlPoints(); i++){
		ObstacleBitmap obm(obstacles);
		obm.drawObstacles();
		double x = robot.getControlPoint(i).getX();
		double y = robot.getControlPoint(i).getY();
		QPointF point = CoordinateTransform::toPlannerPoint(QPointF(x, y), robot.getGoalConfiguration(), IMMP::Constant::SWP);
		PotentialField pf(point.x(), point.y(), obm);
		potentialFields.push_back(pf);
		/*std::string fn = "a";
		for(int jjj = 0; jjj < i; jjj++){
			fn += "a";
		}
		fn += ".txt";
		std::ofstream ofs(fn);
		for(int i = 0; i < Constant::BITMAPSIZE; i++){
			for(int j = 0; j < Constant::BITMAPSIZE; j++){
				if(pf.getPotentialValue(i, j) == Constant::OBST){
					ofs << "    ";
				}
				else{
					ofs << std::setw(3) << pf.getPotentialValue(i, j) << " ";
				}
			}
			ofs << std::endl;
		}
		*/
	}
}

#ifndef USE_TBB

void BFS::initializeVisitedTable(){
	#pragma omp parallel for
	for(int k = 0; k < Constant::ANGLESPACE; k++){
		ObstacleBitmap obm(obstacles);
		if(isUseCSpace()){
//#ifdef USE_TBB
            //cspacePolygon.push_back(obm.drawCSpace(robot, Configuration(0.0, 0.0, k*360.0/Constant::ANGLESPACE)));
//#elif
            cspacePolygon.insert(std::pair<int, std::vector<QPolygonF>>(k, obm.drawCSpace(robot, Configuration(0.0, 0.0, k*360.0/Constant::ANGLESPACE))));
//#endif
		}
		for(int i = 0; i < Constant::XSPACE; i++){
			for(int j = 0; j < Constant::YSPACE; j++){
				visited[i][j][k] = false;
				cspace[i][j][k] = false;
				if(isUseCSpace()){
					if(obm.bitMapAt(i, j) == Constant::OBST){
						cspace[i][j][k] = true;
					}
				}
			}
		}
	}
}

#else

void BFS::initializeVisitedTable(){
	tbb::parallel_for(tbb::blocked_range<size_t>(0, Constant::ANGLESPACE), ApplyBuildTable(this));
}

#endif

void BFS::bruteTestCollision(){
	for(int i = 0; i < Constant::XSPACE; i++){
		for(int j = 0; j < Constant::YSPACE; j++){
			for(int k = 0; k < Constant::ANGLESPACE; k++){
				if(!visited[i][j][k]){
					Configuration config(i, j, k * 360.0 / Constant::ANGLESPACE);
					visited[i][j][k] = CollisionDetector::isRobotCollide(robot, config, obstacles);
				}
			}
		}
	}
}

void BFS::insertToStructure(int index, BFSTree* node){
	if(index >= 0 && index < cstructures.size()){
		cstructures.at(index).push_back(node);
		c_pools.at(index).push_back(node);
	}
}

void BFS::removeFromStructure(int index){
	if(index >= 0 && index < cstructures.size()){
		if(!cstructures.at(index).empty()){
			cstructures.at(index).erase(cstructures.at(index).begin());
		}
	}
}

BFSTree* BFS::getFirstNode(){
	for(int i = 0; i < cstructures.size(); i++){
		if(!cstructures.at(i).empty()){
			BFSTree* node = cstructures.at(i).at(0);
			//visited[(int)node->config.getX()][(int)node->config.getY()][(int)(node->config.getTheta()/Constant::ANGLESPACE)] = true;
			removeFromStructure(i);
			return node;
		}
	}
	return NULL;
}

int BFS::getPotentialValue(const Configuration& config){
	int value = 0;
	for(int i = 0; i < robot.NControlPoints(); i++){
		QPointF point(robot.getControlPoint(i).getX(), robot.getControlPoint(i).getY());
		QPointF transPoint = CoordinateTransform::toPlannerPoint(point, config, Constant::SWP);
		int x = transPoint.x();
		int y = transPoint.y();
		x = Util<int>::adjustRange(x, 0, Constant::BITMAPSIZE);
		y = Util<int>::adjustRange(y, 0, Constant::BITMAPSIZE);
        value += (robot.getControlPointWeight(i) * potentialFields.at(i).getPotentialValue(x, y));
	}
    return value / robot.getTotalControlPointWeights();
}

bool BFS::isEmptyStructure() const {
	for(int i = 0; i < cstructures.size(); i++){
		if(!cstructures.at(i).empty()){
			return false;
		}
	}
	return true;
}

void BFS::destroyCStructure(){
	for(int i = 0; i < c_pools.size(); i++){
		for(int j = 0; j < c_pools.at(i).size(); j++){
			delete c_pools.at(i).at(j);
		}
	}
}

void BFS::setUseCSpace(bool b){
	useCSpace = b;
}

bool BFS::isUseCSpace()const{
	return useCSpace;
}

BFSTree::BFSTree(Configuration c, BFSTree* p){
	parent = p;
	config = c;
}

BFSTree::~BFSTree(){

}
