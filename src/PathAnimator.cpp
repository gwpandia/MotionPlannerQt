#include "PathAnimator.h"

using namespace IMMP;

PathAnimator::PathAnimator()
{
	currentConfig = NULL;
	path = NULL;
	canvas = NULL;
	stopped = false;
}


PathAnimator::~PathAnimator(void)
{
}

void PathAnimator::run(){
	if(canvas == NULL || path == NULL){
		return;
	}

	for(size_t i = 0; i < path->size() && !stopped; i++){
		canvas->setAnimateConfig(&path->at(i));
		canvas->update();
		QThread::msleep(10);
	}
	stopped = false;
	canvas->setAnimateConfig(NULL);
	canvas->setShowAnimation(false);
	canvas->update();
}