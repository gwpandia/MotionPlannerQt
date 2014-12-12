#include "VisitedFrame.h"
#include "VisitedCanvas.h"
#include "Util.h"
#include <sstream>

using namespace IMMP;

VisitedFrame::VisitedFrame(SimpleModel& model) : model(model)
{
	nextButton = new QPushButton(tr("Next"));
	prevButton = new QPushButton(tr("Previous"));
	bottomPanel = new QHBoxLayout;
	panel = new QVBoxLayout;

	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextAngle()));
	QObject::connect(prevButton, SIGNAL(clicked()), this, SLOT(prevAngle()));
	canvas = new VisitedCanvas(0, model);
	bottomPanel->addWidget(prevButton);
	bottomPanel->addWidget(nextButton);
	panel->addWidget(canvas);
	panel->addLayout(bottomPanel);
	setLayout(panel);
	
	if(model.getCSpacePolygon().empty()){
		setMinimumHeight(300);
		setMinimumWidth(300);
	}
	else{
		setMinimumHeight(500);
		setMinimumWidth(1200);
	}
	updateTitle();
	show();
}


VisitedFrame::~VisitedFrame()
{
	delete nextButton;
	delete prevButton;
	delete bottomPanel;
	delete panel;
	delete canvas;
}

void VisitedFrame::updateTitle(){
	std::string title;
	if(model.getCSpacePolygon().empty()){
		title += "Visited Table, angle : ";
		title += Util<int>::toString(canvas->getAngle() * (360/Constant::ANGLESPACE)) + " degree";
		//title += Util<int>::toString(Constant::ANGLESPACE);
		setWindowTitle(tr(title.c_str()));
	}
	else{
		title += "CSpace and Visited Table, angle : ";
		title += Util<int>::toString(canvas->getAngle() * (360/Constant::ANGLESPACE)) + " degree";
		//title += Util<int>::toString(Constant::ANGLESPACE);
		setWindowTitle(tr(title.c_str()));
	}
}

void VisitedFrame::nextAngle(){
	canvas->incrementAngle();
	canvas->update();
	updateTitle();
}

void VisitedFrame::prevAngle(){
	canvas->decrementAngle();
	canvas->update();
	updateTitle();
}
