#include "PotentialFieldCanvas.h"
#include "Constant.h"
#include <QtCore/QDebug>

using namespace IMMP;

PotentialFieldCanvas::PotentialFieldCanvas(const PotentialField *p)
{
	potentialField = p;
}


PotentialFieldCanvas::~PotentialFieldCanvas()
{
}

void PotentialFieldCanvas::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen thickPen(palette().foreground(), 1.5);
	QPen thinPen(palette().foreground(), 0.5);
	QColor obsColor(255, 255, 0);

    //int delta = potentialField->getMaxVal() - potentialField->getMinVal();

//	painter.setPen(thinPen);
//	painter.setBrush(niceBlue);
	painter.setPen(QPen(Qt::NoPen));
	if(potentialField != NULL){
		for(int i = 0; i < Constant::BITMAPSIZE; i++){
			for(int j = 0; j < Constant::BITMAPSIZE; j++){
				int value = potentialField->getPotentialValue(i, j);
				//qDebug()<<value<<" ";
				if(value == Constant::OBST){
					//painter.setPen(QPen(Qt::NoPen));
					painter.setBrush(obsColor);
				}
				else{
					//painter.setPen(QPen(Qt::NoPen));
					//int R = 255-(4*(value-potentialField->getMinVal())/delta);
					//int G = 255-(70*(value-potentialField->getMinVal())/delta);
					//int B = 255-(232*(value-potentialField->getMinVal())/delta);
					int R = 0;
					int G = 0;
					//int B = 255-255*((value-potentialField->getMinVal())/delta);
					int B = 255 - value;
					//qDebug() << R << " " << G << " " << B;
					painter.setBrush(QColor(R, G, B));
				}
				painter.drawRect(QRect(i*Constant::potentialFieldCanvasScale, (Constant::BITMAPSIZE-j)*Constant::potentialFieldCanvasScale, 
					Constant::potentialFieldCanvasScale, Constant::potentialFieldCanvasScale));
			}
			//qDebug() << endl;
		}
	}
}
