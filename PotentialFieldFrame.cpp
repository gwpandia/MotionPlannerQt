#include "PotentialFieldFrame.h"
#include "Util.h"

using namespace IMMP;

PotentialFieldFrame::PotentialFieldFrame(const SimpleModel& model) : model(model)
{
	nextButton = new QPushButton(tr("Next"));
	prevButton = new QPushButton(tr("Previous"));
	bottomPanel = new QHBoxLayout;
	panel = new QVBoxLayout;

	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextControlPoint()));
	QObject::connect(prevButton, SIGNAL(clicked()), this, SLOT(prevControlPoint()));
	canvas = new PotentialFieldCanvas(&model.getPotentialFields().at(0));
	bottomPanel->addWidget(prevButton);
	bottomPanel->addWidget(nextButton);
	panel->addWidget(canvas);
	panel->addLayout(bottomPanel);
	setLayout(panel);
	setMinimumHeight(300);
	setMinimumWidth(300);
	index = 0;
	updateTitle();
	show();
}


PotentialFieldFrame::~PotentialFieldFrame(void)
{
	delete nextButton;
	delete prevButton;
	delete bottomPanel;
	delete panel;
	delete canvas;
}

void PotentialFieldFrame::updateTitle(){
	std::string title = "Potential Field, CP: ";
	title += Util<int>::toString(index+1) + "/";
	title += Util<int>::toString(model.getPotentialFields().size());
	setWindowTitle(tr(title.c_str()));
}

void PotentialFieldFrame::nextControlPoint(){
	++index;
	index %= model.getPotentialFields().size();
	canvas->setPotentialField(&model.getPotentialFields().at(index));
	updateTitle();
	canvas->update();
}

void PotentialFieldFrame::prevControlPoint(){
	--index;
	if(index < 0){
		index = model.getPotentialFields().size() - 1;
	}
	index %= model.getPotentialFields().size();
	canvas->setPotentialField(&model.getPotentialFields().at(index));
	updateTitle();
	canvas->update();
}
