#pragma once
#include <QtCore/QThread>
#include <vector>
#include "Configuration.h"
#include "MyCanvas.h"

namespace IMMP{
class PathAnimator :
	public QThread
{
public:
	PathAnimator();
	~PathAnimator();

	const Configuration *getCurrentConfig()const{
		return currentConfig;
	}

	void setPath(const std::vector<IMMP::Configuration>* p){
		path = p;
	}

	void setCanvas(MyCanvas* c){
		canvas = c;
	}

	void stop(){
		stopped = true;
	}

protected:
	void run();

private:
	const std::vector<IMMP::Configuration>* path;
	const Configuration *currentConfig;
	MyCanvas* canvas;
	bool stopped;
};

}
