#pragma once
#include <QtWidgets/QtWidgets>
#include <vector>
#include "PotentialField.h"

namespace IMMP{
class PotentialFieldCanvas : public QWidget
{
	Q_OBJECT

public:
	PotentialFieldCanvas(const PotentialField*);
	~PotentialFieldCanvas();
	void setPotentialField(const PotentialField *p){
		potentialField = p;
	}

protected:
	void paintEvent(QPaintEvent *);


private:
	const PotentialField *potentialField;
};

}