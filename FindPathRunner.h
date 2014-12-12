#pragma once
#include <QtCore/QThread>
#include "SimpleModel.h"
#include "MyCanvas.h"


namespace IMMP{

class MainFrame;

class FindPathRunner : public QThread
{
public:
	FindPathRunner();
	~FindPathRunner(void);

	void setModel(SimpleModel *m){
		model = m;
	}

	void setCanvas(MyCanvas *c){
		canvas = c;
	}

	void setUseCSpace(bool b){
		useCSpace = b;
	}

	void setFrame(MainFrame *m){
		frame = m;
	}

protected:
	void run();

private:
	bool useCSpace;
	SimpleModel *model;
	MyCanvas *canvas;
	MainFrame *frame;
};

}