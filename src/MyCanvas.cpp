#include "MyCanvas.h"
#include "Constant.h"
#include "CoordinateTransform.h"
#include "CollisionDetector.h"
#include <iostream>
#include <sstream>
#include <QtCore/Qt>
#include <cmath>

using namespace IMMP;

MyCanvas::MyCanvas(std::vector<Robot>& robots, std::vector<Obstacle>& obstacles):robots(robots), obstacles(obstacles)
    , type(Constant::NONE), lock(false), tempI(-1), path(NULL), drawPath(false)
  , showAnimation(false), animateConfig(NULL), lockMouseOperation(false), currentControlPoint(SIZE_MAX)
{
	setMouseTracking(false);
	mb = Qt::NoButton;
	setMinimumHeight(450);
	setMinimumWidth(400);
}


MyCanvas::~MyCanvas(void)
{
}

void MyCanvas::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen thickPen(palette().foreground(), 1.5);
	QPen thinPen(palette().foreground(), 0.5);
	QColor niceBlue(150, 150, 200);

	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(200, 200, 200));
	painter.drawRect(0, 0, Constant::HC, Constant::HC);

	painter.setPen(thinPen);
	painter.setBrush(niceBlue);

	for(int i = 0; i < obstacles.size(); i++){
		for(int j = 0; j < obstacles.at(i).NPolygons(); j++){
			QPolygon transPolygon(CoordinateTransform::toCanvasPoints(obstacles.at(i).getPolygonF(j), obstacles.at(i).getInitialConfiguration(), Constant::SWP, Constant::SPC));
			painter.drawPolygon(transPolygon);
			
		}
		//std::stringstream ss;
		//ss << obstacles.at(i).getInitialConfiguration().getX() << " " << obstacles.at(i).getInitialConfiguration().getY() << " " << obstacles.at(i).getInitialConfiguration().getTheta();
		//painter.drawText(CoordinateTransform::toCanvasPoint(obstacles.at(i).getPolygonF(0), obstacles.at(i).getInitialConfiguration(), Constant::SWP, Constant::SPC).at(0), QString(ss.str().c_str()));
	}



    for(size_t i = 0; i < robots.size(); i++){
        for(size_t j = 0; j < robots.at(i).NPolygons(); j++){
			if(drawPath && (path != NULL)){
                for(size_t k = 0; k < path -> size(); k++){
					QPolygon transPolygon3(CoordinateTransform::toCanvasPoints(robots.at(i).getPolygonF(j), path -> at(k), Constant::SWP, Constant::SPC));
					painter.setBrush(QColor(255, 255, 255));
					painter.drawPolygon(transPolygon3);
				}
			}

			if(showAnimation && (animateConfig != NULL)){
				QPolygon transPolygon3(CoordinateTransform::toCanvasPoints(robots.at(i).getPolygonF(j), *animateConfig, Constant::SWP, Constant::SPC));
				painter.setBrush(QColor(255, 255, 255));
				painter.drawPolygon(transPolygon3);
			}

			QPolygon transPolygon1(CoordinateTransform::toCanvasPoints(robots.at(i).getPolygonF(j), robots.at(i).getInitialConfiguration(), Constant::SWP, Constant::SPC));
            painter.setBrush(QColor(255, 255, 0));
			painter.drawPolygon(transPolygon1);
			QPolygon transPolygon2(CoordinateTransform::toCanvasPoints(robots.at(i).getPolygonF(j), robots.at(i).getGoalConfiguration(), Constant::SWP, Constant::SPC));
            painter.setBrush(QColor(255, 0, 0));
			painter.drawPolygon(transPolygon2);
		}

        for(size_t j = 0; j < robots.at(i).NControlPoints(); ++j){
            QPointF cpF(robots.at(i).getControlPoint(j).getX(), robots.at(i).getControlPoint(j).getY());
            QPoint initCP(CoordinateTransform::toCanvasPoint(cpF, robots.at(i).getInitialConfiguration(), Constant::SWP, Constant::SPC));
            QPoint goalCP(CoordinateTransform::toCanvasPoint(cpF, robots.at(i).getGoalConfiguration(), Constant::SWP, Constant::SPC));

            if(currentControlPoint == j){
                painter.setBrush(QColor(255, 255, 255));
                painter.drawEllipse(initCP, 4, 4);
                painter.drawEllipse(goalCP, 4, 4);
            }

            painter.setBrush(QColor(0, 0, 0));
            painter.drawEllipse(initCP, 2, 2);
            painter.drawEllipse(goalCP, 2, 2);
        }
	}

	painter.setBrush(QColor(0, 0, 0));
	painter.drawText(0, 420, message);
}

void MyCanvas::mousePressEvent(QMouseEvent *event){
	mb = event->button();
	clickedPoint.setX(event->x());
	clickedPoint.setY(event->y());
	tempI = clickedOnWhichRobots(event);
	if(tempI < 0){
		tempI = clickedOnWhichObstacles(event);
	}
	update();
}

void MyCanvas::mouseMoveEvent(QMouseEvent *event){
	QPoint curPoint(event->x(), event->y());
	if((mb == Qt::LeftButton) && tempI != -1){
		QPointF vec( ((double)(curPoint.x() - clickedPoint.x()))/Constant::SPC, ((double)(clickedPoint.y() - curPoint.y())/Constant::SPC));
		if(lock && type == Constant::ROBOT_I){
			robots.at(tempI).setInitialConfiguration(Configuration(foundConfig.getX()+vec.x(), foundConfig.getY()+vec.y(), foundConfig.getTheta()));
		}
		else if(lock && type == Constant::ROBOT_G){
			robots.at(tempI).setGoalConfiguration(Configuration(foundConfig.getX()+vec.x(), foundConfig.getY()+vec.y(), foundConfig.getTheta()));
		}
		else if(lock && type == Constant::OBS){
			obstacles.at(tempI).setInitialConfiguration(Configuration(foundConfig.getX()+vec.x(), foundConfig.getY()+vec.y(), foundConfig.getTheta()));
		}
	}
	else if((mb == Qt::RightButton) && tempI != -1){
		QPointF p1(((double)clickedPoint.x())/Constant::SPC - foundConfig.getX(), ((double)(Constant::HC - clickedPoint.y()))/Constant::SPC - foundConfig.getY());
		QPointF p2(((double)curPoint.x())/Constant::SPC - foundConfig.getX(), ((double)(Constant::HC - curPoint.y()))/Constant::SPC - foundConfig.getY());
		double theta = std::atan2(p2.y(), p2.x()) - std::atan2(p1.y(), p1.x());
		theta = CoordinateTransform::toDegree(theta);
		while(theta < 0){
			theta += 360.0;
		}
		if(lock && type == Constant::ROBOT_I){
			robots.at(tempI).setInitialConfiguration(Configuration(foundConfig.getX(), foundConfig.getY(), foundConfig.getTheta() + theta));
		}
		else if(lock && type == Constant::ROBOT_G){
			robots.at(tempI).setGoalConfiguration(Configuration(foundConfig.getX(), foundConfig.getY(), foundConfig.getTheta() + theta));
		}
		else if(lock && type == Constant::OBS){
			obstacles.at(tempI).setInitialConfiguration(Configuration(foundConfig.getX(), foundConfig.getY(), foundConfig.getTheta() + theta));
		}
	}
	if(CollisionDetector::isRobotCollide(robots.at(0), robots.at(0).getInitialConfiguration(), obstacles)){
		std::cout << "Init Collide" << std::endl;
	}
	if(CollisionDetector::isRobotCollide(robots.at(0), robots.at(0).getGoalConfiguration(), obstacles)){
		std::cout << "Goal Collide" << std::endl;
	}
	update();
}

void MyCanvas::mouseReleaseEvent(QMouseEvent *event){
	lock = false;
	tempI = -1;
	type = Constant::NONE;
	mb = Qt::NoButton;
	update();
}

int MyCanvas::clickedOnWhichRobots(QMouseEvent *event){
	for(int i = 0; i < robots.size(); i++){
		for(int j = 0; j < robots.at(i).NPolygons(); j++){
			QPolygonF transPolygon1(CoordinateTransform::toPlannerPoints(robots.at(i).getPolygonF(j), robots.at(i).getInitialConfiguration(), Constant::SWP));
			QPolygonF transPolygon2(CoordinateTransform::toPlannerPoints(robots.at(i).getPolygonF(j), robots.at(i).getGoalConfiguration(), Constant::SWP));

			if(transPolygon1.containsPoint(QPointF(((double)event->x())/Constant::SPC, (((double)Constant::HC-event->y()))/Constant::SPC), Qt::OddEvenFill)){
				foundConfig = robots.at(i).getInitialConfiguration();
				lock = true;
				type = Constant::ROBOT_I;
				return i;
			}
			else if(transPolygon2.containsPoint(QPointF(((double)event->x())/Constant::SPC, (((double)Constant::HC-event->y()))/Constant::SPC), Qt::OddEvenFill)){
				foundConfig = robots.at(i).getGoalConfiguration();
				lock = true;
				type = Constant::ROBOT_G;
				return i;
			}
		}
	}
	return -1;
}

int MyCanvas::clickedOnWhichObstacles(QMouseEvent *event){
	for(int i = 0; i < obstacles.size(); i++){
		for(int j = 0; j < obstacles.at(i).NPolygons(); j++){
			QPolygonF transPolygon1(CoordinateTransform::toPlannerPoints(obstacles.at(i).getPolygonF(j), obstacles.at(i).getInitialConfiguration(), Constant::SWP));
			if(transPolygon1.containsPoint(QPointF(((double)event->x())/Constant::SPC, (((double)Constant::HC-event->y()))/Constant::SPC), Qt::OddEvenFill)){
				foundConfig = obstacles.at(i).getInitialConfiguration();
				lock = true;
				type = Constant::OBS;
				return i;
			}
		}
	}
	return -1;
}
