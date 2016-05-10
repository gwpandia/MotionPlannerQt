//#include "qtmotionplanner.h"
#include <QtWidgets/QApplication>
#include "MainFrame.h"
#include "SimpleModel.h"
#include <ctime>

int main(int argc, char *argv[])
{
	srand(time(NULL));
	QApplication a(argc, argv);
	a.setApplicationDisplayName(QString("Motion Planner with Qt5"));
	a.setApplicationName(QString("Motion Planner"));
	a.setApplicationVersion(QString("1.0"));
	a.setWindowIcon(QIcon("im-logo2.png"));
	//QtMotionPlanner w;


	//std::vector<IMMP::Robot> robots = IMMP::SceneReader::openRobotFile("DAT\\Robots\\robot05.dat");
	//std::vector<IMMP::Obstacle> obstacles = IMMP::SceneReader::openObstacleFile("DAT\\Obstacles\\map05.dat");

	//IMMP::BFS bfs(robots.at(0), obstacles);
	//long ori = std::time(NULL);
	//std::vector<IMMP::Configuration> path = bfs.findPath();
	//long fin = std::time(NULL);
	
	//std::ofstream ofs("path.txt");
	//ofs << (fin-ori) << std::endl;
	//for(int i = 0; i < path.size(); i++){
	//	ofs << path.at(i).getX() << " " << path.at(i).getY() << " " << path.at(i).getTheta() << std::endl;
	//}
	//ofs.close();
	//IMMP::MyCanvas w(robots, obstacles);

	//w.show();
	IMMP::SimpleModel sm;
	IMMP::MainFrame m(sm);
	m.setMinimumWidth(550);
	m.setMinimumHeight(450);
	m.show();
	return a.exec();
}
