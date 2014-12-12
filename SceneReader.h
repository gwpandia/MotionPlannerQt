#pragma once
#include <string>
#include <vector>
#include "Robot.h"
#include "Obstacle.h"

namespace IMMP{

class SceneReader
{
public:

	static std::vector<IMMP::Robot> openRobotFile(std::string);
	static std::vector<IMMP::Obstacle> openObstacleFile(std::string);
	static bool getNextValidLine(std::ifstream&, std::string&, std::stringstream&);

private:

};

};