#pragma once
#include <QtWidgets/QtWidgets>
#include <vector>
#include "Robot.h"
#include "Obstacle.h"
#include <string>

namespace IMMP{
class MyCanvas : public QWidget
{
	Q_OBJECT
public:
	MyCanvas(std::vector<Robot>&, std::vector<Obstacle>&);
	~MyCanvas(void);
	void setDrawPath(bool t){
		drawPath = t;
	}
	bool isDrawPath()const{
		return drawPath;
	}
	void setShowAnimation(bool t){
		showAnimation = t;
	}
	bool isShowAnimation()const{
		return showAnimation;
	}
	void setLockMouseOperation(bool u){
		lockMouseOperation = u;
	}
	bool isLockMouseOperation()const{
		return lockMouseOperation;
	}
	void setPath(const std::vector<IMMP::Configuration>* p){
		path = p;
	};

	void setAnimateConfig(const Configuration* c){
		animateConfig = c;
	}

	void setMessage(const QString& str){
		message = str;
	}

    void setSelectedControlPoint(size_t nCP){
        currentControlPoint = nCP;
    }

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);



private:
	int clickedOnWhichRobots(QMouseEvent *event);
	int clickedOnWhichObstacles(QMouseEvent *event);
	IMMP::Configuration foundConfig;

	int type;
	int tempI;
	bool lock;
	std::vector<Robot>& robots;
	std::vector<Obstacle>& obstacles;
	const std::vector<IMMP::Configuration>* path;
	QPoint clickedPoint;
	Qt::MouseButton mb;
	bool lockMouseOperation;
	bool drawPath;
	bool showAnimation;
	const Configuration *animateConfig;
    size_t currentControlPoint;
	QString message;
};

};
