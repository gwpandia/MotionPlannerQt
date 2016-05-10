#include "FindPathRunner.h"
#include "Util.h"
#include "MainFrame.h"

using namespace IMMP;

FindPathRunner::FindPathRunner(){
}


FindPathRunner::~FindPathRunner(void)
{
}

void FindPathRunner::run(){
	QTime time;
	time.start();
	model->findPath(useCSpace);
	int delta = time.elapsed();
	if(!model->getPath().empty()){
		QString msg(tr("Path Found in "));
		msg.append(tr(Util<int>::toString(delta).c_str()));
		msg.append(tr(" milliseconds."));
		canvas->setMessage(msg);
		canvas->setDrawPath(true);
		canvas->setPath(&model->getPath());
	}
	else{
		canvas->setMessage(QString(tr("Path Not Found.")));
	}
	frame->setUIState();
	canvas->update();
}