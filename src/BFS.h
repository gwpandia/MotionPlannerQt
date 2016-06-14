#pragma once
#include "Robot.h"
#include "Obstacle.h"
#include "Constant.h"
#include "PotentialField.h"
#include <vector>
#ifdef USE_TBB
#include <tbb/tbb.h>
#endif
#include <map>

namespace IMMP{

class BFSTree;
class ApplyBuildTable;

class BFS
{
public:
	BFS(const Robot&, const std::vector<Obstacle>&);
	~BFS(void);
	std::vector<Configuration> findPath();

	void setUseCSpace(bool);
	bool isUseCSpace()const;

	void init();

	const std::vector<PotentialField>& getPotentialFields()const{
		return potentialFields;
	}

	bool isVisit(int x, int y, int a)const{
		return visited[x][y][a];
	}

	void setVisit(int x, int y, int a, int b){
		visited[x][y][a] = b;
	}

	bool isCSpace(int x, int y, int a)const{
		return cspace[x][y][a];
	}

	void setCSpace(int x, int y, int a, bool b){
		cspace[x][y][a] = b;
	}

	std::map<int, std::vector<QPolygonF> > cspacePolygon;

	const std::vector<Obstacle>& getObstacles()const{
		return obstacles;
	}

	const Robot& getRobot()const{
		return robot;
	}

private:
	void initializePotentialField();
	void initializeVisitedTable();
	void bruteTestCollision();

	void insertToStructure(int, BFSTree*);
	void removeFromStructure(int);
	BFSTree* getFirstNode();
	int getPotentialValue(const Configuration&);
	bool isEmptyStructure()const;

	void destroyCStructure();

	bool useCSpace;
	bool visited[Constant::XSPACE][Constant::YSPACE][Constant::ANGLESPACE];
	bool cspace[Constant::XSPACE][Constant::YSPACE][Constant::ANGLESPACE];
    const Robot& robot;
    const std::vector<Obstacle>& obstacles;
	std::vector< std::vector<BFSTree*> > cstructures;
	std::vector< std::vector<BFSTree*> > c_pools;
	std::vector<PotentialField> potentialFields;
};

class BFSTree{
public:
	BFSTree(Configuration, BFSTree*);
	~BFSTree();
	BFSTree* parent;
	Configuration config;
};

#ifdef USE_TBB
class ApplyBuildTable{
public:
	ApplyBuildTable(BFS *bfs):bfs(bfs){

	}

	void operator()(const tbb::blocked_range<size_t>& r) const{
		for(int k = r.begin(); k < r.end(); k++){
			ObstacleBitmap obm(bfs->getObstacles());
			if(bfs->isUseCSpace()){
				bfs->cspacePolygon[k] = obm.drawCSpace(bfs->getRobot(), Configuration(0.0, 0.0, k*360.0/Constant::ANGLESPACE));
			}
			for(int i = 0; i < Constant::XSPACE; i++){
				for(int j = 0; j < Constant::YSPACE; j++){
					bfs->setVisit(i, j, k, false);
					bfs->setCSpace(i, j, k, false);
					if(bfs->isUseCSpace()){
						if(obm.bitMapAt(i, j) == Constant::OBST){
							bfs->setCSpace(i, j, k, true);
						}
					}
				}
			}
		}
	}

private:
	BFS* bfs;
};
#endif

};
