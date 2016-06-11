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
    void smoothHelper(size_t, size_t, std::vector<Configuration>&, const std::vector<Configuration>&);
    void generatePath(size_t, const Configuration&, const Configuration&, std::vector<Configuration>&);
	bool checkAll(const std::vector<Configuration>&)const;
};

}
