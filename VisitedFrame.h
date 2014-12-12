#pragma once
#include <QtWidgets/QtWidgets>
#include <vector>
#include "SimpleModel.h"
#include "VisitedCanvas.h"

namespace IMMP{
class VisitedFrame : public QWidget
{
	Q_OBJECT

public:
	VisitedFrame(SimpleModel&);
	~VisitedFrame();

private slots:

	void nextAngle();
	void prevAngle();

private:
	void updateTitle();
	QPushButton *nextButton;
	QPushButton *prevButton;
	QHBoxLayout *bottomPanel;
	QVBoxLayout *panel;
	SimpleModel& model;
	VisitedCanvas *canvas;
};

}