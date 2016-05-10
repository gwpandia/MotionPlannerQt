#include "SceneReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QtGui/QtGui>
#include "IMStrTok.h"

using namespace IMMP;

bool SceneReader::getNextValidLine(std::ifstream& ifs, std::string& line, std::stringstream& ss){
	while(getline(ifs, line)){
		StrTok tokenizer(line, " \r\t\n");
		if(!tokenizer.tokened.empty() && tokenizer.tokened.at(0).at(0) != '#'){
			ss.clear();
			ss << line;
			return true;	
		}
	}
	return false;
}

std::vector<Robot> SceneReader::openRobotFile(std::string path){
	std::vector<Robot> robots;
	std::ifstream reader(path.c_str());
	std::string line;
	std::stringstream ss;

	if(reader.fail()){
		return robots;
	}

	getNextValidLine(reader, line, ss);
	int nRobot = 0;
	ss >> nRobot;
	for(int i = 0; i < nRobot; i++){
		Robot robot;
		getNextValidLine(reader, line, ss);
		int nPolygon = 0;
		ss >> nPolygon;
		for(int j = 0; j < nPolygon; j++){
			getNextValidLine(reader, line, ss);
			int nVertice = 0;
			ss >> nVertice;
			QPolygonF polygon;
			for(int k = 0; k < nVertice; k++){
				getNextValidLine(reader, line, ss);
				double x = 0.0, y = 0.0;
				ss >> x >> y;
				polygon.append(QPointF(x, y));
			}
			robot.addPolygon(polygon);
		}

		getNextValidLine(reader, line, ss);
		double x = 0.0, y = 0.0, theta = 0.0;
		ss >> x >> y >> theta;
		robot.setInitialConfiguration(Configuration(x, y, theta));
		getNextValidLine(reader, line, ss);
		ss >> x >> y >> theta;
		robot.setGoalConfiguration(Configuration(x, y, theta));
			
		getNextValidLine(reader, line, ss);
		int nControlPoints = 0;
		ss >> nControlPoints;
		for(int j = 0; j < nControlPoints; j++){
			getNextValidLine(reader, line, ss);
			double x = 0.0, y = 0.0;
			ss >> x >> y;
			robot.addControlPoint(x, y);
		}
		robots.push_back(robot);
	}
	return robots;
}

std::vector<Obstacle> SceneReader::openObstacleFile(std::string path){
	std::vector<Obstacle> obstacles;
	std::ifstream reader(path.c_str());
	std::string line;
	std::stringstream ss;

	if(reader.fail()){
		return obstacles;
	}

	getNextValidLine(reader, line, ss);
	int nObstacle = 0;
	ss >> nObstacle;
	for(int i = 0; i < nObstacle; i++){
		Obstacle obstacle;
		getNextValidLine(reader, line, ss);
		int nPolygon = 0;
		ss >> nPolygon;
		for(int j = 0; j < nPolygon; j++){
			getNextValidLine(reader, line, ss);
			int nVertice = 0;
			ss >> nVertice;
			QPolygonF polygon;
			for(int k = 0; k < nVertice; k++){
				getNextValidLine(reader, line, ss);
				double x = 0.0, y = 0.0;
				ss >> x >> y;
				polygon.append(QPointF(x, y));
			}
			obstacle.addPolygon(polygon);
		}

		getNextValidLine(reader, line, ss);
		double x = 0.0, y = 0.0, theta = 0.0;
		ss >> x >> y >> theta;
		obstacle.setInitialConfiguration(Configuration(x, y, theta));
		obstacles.push_back(obstacle);
	}
	return obstacles;

}