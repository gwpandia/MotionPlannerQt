#pragma once
#include "SimpleModel.h"
#include "Configuration.h"
#include <vector>

namespace IMMP{

class PathSmoother
{
public:
	PathSmoother(SimpleModel&);
	~PathSmoother();
	std::vector<Configuration> smooth(const std::vector<Configuration>&);

private:
	SimpleModel& model;
	void smoothHelper(int, int, std::vector<Configuration>&, const std::vector<Configuration>&);
	void generatePath(int, const Configuration&, const Configuration&, std::vector<Configuration>&);
	bool checkAll(const std::vector<Configuration>&)const;
};

}