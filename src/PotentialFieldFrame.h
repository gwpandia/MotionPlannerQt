#pragma once
#include <QtWidgets/QtWidgets>
#include "PotentialFieldCanvas.h"
#include "SimpleModel.h"

namespace IMMP{

class PotentialFieldFrame :	public QWidget
{
	Q_OBJECT

public:
	PotentialFieldFrame(const SimpleModel&);
	~PotentialFieldFrame();

private slots:

		void nextControlPoint();
		void prevControlPoint();
		void updateTitle();

private:
	int index;
	QPushButton *nextButton;
	QPushButton *prevButton;
	QHBoxLayout *bottomPanel;
	QVBoxLayout *panel;
	PotentialFieldCanvas *canvas;
	const SimpleModel& model;
};

}