#include "VisitedCanvas.h"
#include "CoordinateTransform.h"
using namespace IMMP;

VisitedCanvas::VisitedCanvas(int angle, SimpleModel& model) : angle(angle), model(model)
{

}


VisitedCanvas::~VisitedCanvas()
{

}



void VisitedCanvas::paintEvent(QPaintEvent * event){
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen thickPen(palette().foreground(), 1.5);
	QPen thinPen(palette().foreground(), 0.5);
	QColor obsColor(255, 255, 0);
	QColor nmlColor(0, 0, 255);
	painter.setPen(QPen(Qt::NoPen));

	if(!model.getCSpacePolygon().empty()){
		painter.setBrush(QColor(0, 0, 0));
		painter.drawRect(0, 0, 400, 400);
		painter.setBrush(QColor(255, 0, 0));
		for(size_t i = 0; i < model.getCSpacePolygon().at(angle).size(); i++){
			QPolygon transPolygon(CoordinateTransform::toCanvasPoints(model.getCSpacePolygon().at(angle).at(i), Configuration(0, 0, 0), Constant::SWP, Constant::SPC));
			painter.drawPolygon(transPolygon);
		}

		painter.setBrush(QColor(255, 255, 0));

		for(size_t i = 0; i < model.getObstacles().size(); i++){
			for(size_t j = 0; j < model.getObstacles().at(i).NPolygons(); j++){
				QPolygon transPolygon(CoordinateTransform::toCanvasPoints(model.getObstacles().at(i).getPolygonF(j), model.getObstacles().at(i).getInitialConfiguration(), Constant::SWP, Constant::SPC));
				painter.drawPolygon(transPolygon);

			}
		}
	}

	int offset = 0;
	if(!model.getCSpacePolygon().empty()){
		offset = 500;
	}

	for(int i = 0; i < Constant::XSPACE; i++){
		for(int j = 0; j < Constant::YSPACE; j++){

			if(model.isVisit(i, j, angle)){
				painter.setBrush(QColor(0, 255, 0));
			}
			else{
				painter.setBrush(nmlColor);
			}

			painter.drawRect(QRect(offset+i*Constant::potentialFieldCanvasScale, (Constant::YSPACE-j)*Constant::potentialFieldCanvasScale, 
				Constant::potentialFieldCanvasScale, Constant::potentialFieldCanvasScale));
		}
	}

	if(!model.getCSpacePolygon().empty()){
		for(int i = 0; i < Constant::XSPACE; i++){
			for(int j = 0; j < Constant::YSPACE; j++){

				if(model.isCSpace(i, j, angle)){
					painter.setBrush(obsColor);
				}
				else{
					painter.setBrush(nmlColor);
				}

				painter.drawRect(offset+300+i*Constant::potentialFieldCanvasScale, (Constant::YSPACE-j)*Constant::potentialFieldCanvasScale, 
					Constant::potentialFieldCanvasScale, Constant::potentialFieldCanvasScale);
			}
		}
	}
}
