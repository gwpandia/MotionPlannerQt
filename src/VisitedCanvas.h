#pragma once
#include <QtWidgets/QtWidgets>
#include "SimpleModel.h"

namespace IMMP{
class VisitedCanvas :
	public QWidget
{
	Q_OBJECT

public:
	VisitedCanvas(int, SimpleModel&);
	~VisitedCanvas();
	void incrementAngle(){
		++angle;
		angle %= Constant::ANGLESPACE;
	}

	void decrementAngle(){
		--angle;
		if(angle < 0){
			angle = Constant::ANGLESPACE - 1;
		}
	}
	int getAngle()const{
		return angle;
	}

protected:
	void paintEvent(QPaintEvent *);

private:

	int angle;
	SimpleModel& model;
};

}