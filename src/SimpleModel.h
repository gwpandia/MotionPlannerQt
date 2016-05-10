#pragma once
#include <tbb/concurrent_hash_map.h>
#include <vector>
#include "Robot.h"
#include "Obstacle.h"
#include "BFS.h"

namespace IMMP{
class SimpleModel
{
public:
	SimpleModel(void);
	~SimpleModel(void);

	static const int STATE_INIT = 0;
	static const int STATE_FINDING = 1;
	static const int STATE_FOUND = 2;
	static const int STATE_NOTFOUND = 3;

	void setRobots(std::vector<IMMP::Robot> r){
		robots = r;
	}
	void setObstacles(std::vector<IMMP::Obstacle> o){
		obstacles = o;
	}

	std::vector<IMMP::Robot>& getRobots(){
		return robots;
	}

	std::vector<IMMP::Obstacle>& getObstacles(){
		return obstacles;
	}

	bool isFoundPath()const{
		return !path.empty();
	}

	const std::vector<IMMP::Configuration>& getPath()const{
		return path;
	}

	void setPath(const std::vector<IMMP::Configuration>& p){
		path = p;
	}

	void clearPath(){
		path.clear();
	}

	void findPath(bool);

	const std::vector<PotentialField>& getPotentialFields()const{
		//if(bfs != NULL){
			return bfs->getPotentialFields();
		//}
		//return std::vector<PotentialField>();
	}

	bool isVisit(int x, int y, int a)const{
		return bfs->isVisit(x, y, a);
	}

	bool isCSpace(int x, int y, int a)const{
		return bfs->isCSpace(x, y, a);
	}

	void setCSpacePolygon(const std::map<int, std::vector<QPolygonF> >& polygons){
		cspacePolygon.clear();
		if(!polygons.empty()){
			for(int i = 0; i < Constant::ANGLESPACE; i++){
				cspacePolygon.push_back(polygons.at(i));
			}
		}
	}

	const std::vector< std::vector<QPolygonF> >& getCSpacePolygon()const{
		return cspacePolygon;
	}

	BFS* getBFS()const{
		return bfs;
	}

	void cleanBFS();

	int getState()const{
		return state;
	}

	void setState(int s){
		state = s;
	}

	void reset(){
		clearPath();
		state = STATE_INIT;
	}

private:
	std::vector<IMMP::Robot> robots;
	std::vector<IMMP::Obstacle> obstacles;
	std::vector<IMMP::Configuration> path;
	std::vector< std::vector<QPolygonF> > cspacePolygon;
	IMMP::BFS *bfs;
	int state;
};

};