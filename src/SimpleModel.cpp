#include "SimpleModel.h"
#include "SceneReader.h"

using namespace IMMP;

SimpleModel::SimpleModel(void)
{
	//robots = IMMP::SceneReader::openRobotFile("DAT\\Robots\\robot00.dat");
	//obstacles = IMMP::SceneReader::openObstacleFile("DAT\\Obstacles\\map00.dat");
	bfs = NULL;
	state = STATE_INIT;
}


SimpleModel::~SimpleModel(void)
{
	delete bfs;
}

void SimpleModel::cleanBFS(){
	if(bfs != NULL){
		delete bfs;
		bfs = NULL;
	}
}

void SimpleModel::findPath(bool useCSpace){
	clearPath();
	cspacePolygon.clear();
	cleanBFS();
	if(robots.empty() || obstacles.empty()){
		return;
	}
	state = STATE_FINDING;
	bfs = new BFS(getRobots().at(0), getObstacles());
	bfs->setUseCSpace(useCSpace);
	bfs->init();
	setPath(bfs->findPath());
	setCSpacePolygon(bfs->cspacePolygon);

	if(path.empty()){
		state = STATE_NOTFOUND;
	}
	else{
		state = STATE_FOUND;
	}

}
