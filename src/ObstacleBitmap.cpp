#include "ObstacleBitmap.h"
#include "CoordinateTransform.h"
#include "Util.h"
#include <cmath>
#include <cfloat>

using namespace IMMP;

ObstacleBitmap::ObstacleBitmap(const std::vector<Obstacle>& o):obstacles(o)
{
	initialBitMap();
	//drawObstacles();
}


ObstacleBitmap::~ObstacleBitmap(void){
}

void ObstacleBitmap::getBitMap(int (*bm)[Constant::BITMAPSIZE])const{
	for (int i = 0; i < Constant::BITMAPSIZE; i++){
		for(int j = 0; j < Constant::BITMAPSIZE; j++){
			bm[i][j] = obsbitmap[i][j];
		}
	}
}

int ObstacleBitmap::bitMapAt(int i, int j){
	if(i >= 0 && i < Constant::BITMAPSIZE && j >= 0 && j < Constant::BITMAPSIZE){
		return obsbitmap[i][j];
	}
	return -1;
}

void ObstacleBitmap::initialBitMap(){
	for(int i = 0; i < Constant::BITMAPSIZE; i++){
		for(int j = 0; j < Constant::BITMAPSIZE; j++){
			obsbitmap[i][j] = Constant::INIT;
		}
	}
}

std::vector<QPolygonF> ObstacleBitmap::drawCSpace(const Robot& robot, const Configuration& config){
	std::vector<QPolygonF> ret;
	for (int i = 0; i < obstacles.size(); i++){
		for(int j = 0; j < obstacles.at(i).NPolygons(); j++){

			QPolygonF transObstacle(CoordinateTransform::toPlannerPoints(obstacles.at(i).getPolygonF(j), obstacles.at(i).getInitialConfiguration(), Constant::SWP));
			
			for(int k = 0; k < robot.NPolygons(); k++){
				QPolygonF transRobot(minusPolygon(QPolygonF(CoordinateTransform::toPlannerPoints(robot.getPolygonF(k), config, Constant::SWP))));
				//qDebug() << "robot: " << transRobot;
				//qDebug() << "Obstacle: " << transObstacle;
				QPolygonF result = minkowskiSums(transObstacle, transRobot);
				//qDebug() << "result: " << result;
				ret.push_back(result);
				drawObstacle(result);
			}
		}
	}

	QPolygonF top;
	top.push_back(QPointF(0, -1));
	top.push_back(QPointF(0, -2));
	top.push_back(QPointF(Constant::BITMAPSIZE, -2));
	top.push_back(QPointF(Constant::BITMAPSIZE, -1));
	QPolygonF bottom;
	bottom.push_back(QPointF(0, Constant::BITMAPSIZE+1));
	bottom.push_back(QPointF(0, Constant::BITMAPSIZE));
	bottom.push_back(QPointF(Constant::BITMAPSIZE, Constant::BITMAPSIZE));
	bottom.push_back(QPointF(Constant::BITMAPSIZE, Constant::BITMAPSIZE+1));
	QPolygonF left;
	left.push_back(QPointF(-2, 0));
	left.push_back(QPointF(-1, 0));
	left.push_back(QPointF(-1, Constant::BITMAPSIZE));
	left.push_back(QPointF(-2, Constant::BITMAPSIZE));
	QPolygonF right;
	right.push_back(QPointF(Constant::BITMAPSIZE, 0));
	right.push_back(QPointF(Constant::BITMAPSIZE+1, 0));
	right.push_back(QPointF(Constant::BITMAPSIZE+1, Constant::BITMAPSIZE));
	right.push_back(QPointF(Constant::BITMAPSIZE, Constant::BITMAPSIZE));

	for(int k = 0; k < robot.NPolygons(); k++){
		QPolygonF transRobot(CoordinateTransform::toPlannerPoints(robot.getPolygonF(k), config, Constant::SWP));
		QPolygonF result1 = minkowskiSums(top, transRobot);
		QPolygonF result2 = minkowskiSums(bottom, transRobot);
		QPolygonF result3 = minkowskiSums(left, transRobot);
		QPolygonF result4 = minkowskiSums(right, transRobot);
		ret.push_back(result1);
		drawObstacle(result1);
		ret.push_back(result2);
		drawObstacle(result2);
		ret.push_back(result3);
		drawObstacle(result3);
		ret.push_back(result4);
		drawObstacle(result4);
	}

	return ret;
}

QPolygonF ObstacleBitmap::minusPolygon(const QPolygonF& polygon)const{
	QPolygonF ret;
	for (int i = 0; i < polygon.size(); i++)
	{
		ret.push_back(polygon.at(i) * -1.0);
	}
	return ret;
}

QPolygonF ObstacleBitmap::minkowskiSums(const QPolygonF& polygonV, const QPolygonF& polygonW){
	
	std::vector<QPointF> vertexV = initPolygonVertex(findFirstVertex(polygonV), polygonV);
	std::vector<QPointF> vertexW = initPolygonVertex(findFirstVertex(polygonW), polygonW);

	QPolygonF ret;

	int i = 0;
	int j = 0;

    size_t vSize = vertexV.size();
    size_t wSize = vertexW.size();

	do{
		qreal x = vertexV.at(i%vSize).x() + vertexW.at(j%wSize).x();
		qreal y = vertexV.at(i%vSize).y() + vertexW.at(j%wSize).y();
		ret.push_back(QPointF(x, y));
		QPointF V = vertexV.at((i+1)%vSize) - vertexV.at(i%vSize);
		QPointF W = vertexW.at((j+1)%wSize) - vertexW.at(j%wSize);
		double angle1 = CoordinateTransform::toPositiveDegree(std::atan2(V.y(), V.x()));
		double angle2 = CoordinateTransform::toPositiveDegree(std::atan2(W.y(), W.x()));

		if(i == polygonV.size()){
			//if(angle1 == 0.0){
				angle1 = 360.0;
			//}
		}

		if(j == polygonW.size()){
			//if(angle2 == 0.0){
				angle2 = 360.0;
			//}
		}

		if(angle1 < angle2){
			++i;
		}
		else if(angle1 > angle2){
			++j;
		}
		else{
			++i;
			++j;
		}

	}while((i < polygonV.size()) || (j < polygonW.size()));

	return ret;
}

std::vector<QPointF> ObstacleBitmap::initPolygonVertex(int start, const QPolygonF& polygon)const{
	std::vector<QPointF> ret;

	for(int i = 0; i < polygon.size(); i++){
		ret.push_back(polygon.at((i+start) % polygon.size()));
	}

	/*if(ret.at(0).x() > ret.at(1).x()){
		std::vector<QPointF> tmp;
		tmp.push_back(ret.at(0));

		for(int i = ret.size() - 1; i >= 1; i--){
			tmp.push_back(ret.at(i));
		}
		//ret = tmp;
		return tmp;
	}*/

	//ret.push_back(polygon.at(start));
	//ret.push_back(polygon.at(start+1));
	return ret;
}

int ObstacleBitmap::findFirstVertex(const QPolygonF& polygon)const{
	std::vector<int> tmp;
	qreal minY = DBL_MAX;
	for(int i = 0; i < polygon.size(); i++){
		if(polygon.at(i).y() < minY){
			tmp.clear();
			minY = polygon.at(i).y();
			tmp.push_back(i);
		}
		else if(minY == polygon.at(i).y()){
			tmp.push_back(i);
		}
	}

	if(tmp.size() == 1){
		return tmp.at(0);
	}

	qreal minX = DBL_MAX;
	int ret = -1;

	for(int i = 0; i < tmp.size(); i++){
		if(polygon.at(tmp.at(i)).x() < minX){
			minX = polygon.at(tmp.at(i)).x();
			ret = tmp.at(i);
		}
	}

	return ret;
}

void ObstacleBitmap::drawObstacles(){
	for (int i = 0; i < obstacles.size(); i++){
		for(int j = 0; j < obstacles.at(i).NPolygons(); j++){
			QPolygonF transPolygon(CoordinateTransform::toPlannerPoints(obstacles.at(i).getPolygonF(j), obstacles.at(i).getInitialConfiguration(), Constant::SWP));
			drawObstacle(transPolygon);
		}
	}
}

void ObstacleBitmap::drawObstacle(const QPolygonF& transPolygon){
	
			
	int xMaxInt = (int)(transPolygon.boundingRect().right()+0.5);
	int xMinInt = (int)(transPolygon.boundingRect().left()+0.5);
	xMaxInt = Util<int>::adjustRange(xMaxInt, 0, Constant::BITMAPSIZE);
	xMinInt = Util<int>::adjustRange(xMinInt, 0, Constant::BITMAPSIZE);
	int size = xMaxInt - xMinInt;
	if(size == 0){
		size = 1;
	}
	
	int* YMIN = new int[size];
	int* YMAX = new int[size];

	for(int a = 0; a < size;a++){
		YMAX[a] = -1;
		YMIN[a] = 1000;
	}

	for(int k = 0; k < transPolygon.size(); k++){
		int s = ( k + 1 ) % transPolygon.size();
		double d = std::max(std::abs(transPolygon.at(s).x()-transPolygon.at(k).x()), 
							std::abs(transPolygon.at(s).y()-transPolygon.at(k).y()));
		double dx = ((transPolygon.at(s).x()-transPolygon.at(k).x())/d);
		double dy = ((transPolygon.at(s).y()-transPolygon.at(k).y())/d);
		for(int l = 0; l < d; l++){
			int xi = (int)(transPolygon.at(k).x()+(l*dx));
			int yi = (int)(transPolygon.at(k).y()+(l*dy));;
			xi = Util<int>::adjustRange(xi, xMinInt, xMaxInt);
			yi = Util<int>::adjustRange(yi, 0, Constant::BITMAPSIZE);
			int xPos = xi - xMinInt;
			xPos = Util<int>::adjustRange(xPos, 0, size);
			if(yi >= YMAX[xPos]){
				YMAX[xPos] = yi;
			}
			if(yi <= YMIN[xPos]){
				YMIN[xPos] = yi;
			}
		}
	}

	for(int a = xMinInt; a < xMaxInt; a++){
		for(int b = YMIN[a-xMinInt]; b <= YMAX[a-xMinInt]; b++){
			obsbitmap[a][b] = Constant::OBST;
		}
	}

	delete [] YMIN;
	delete [] YMAX;

}
/*
int ObstacleBitmap::adjustRange(int test, int min, int max){
	if(test >= max){
		test = max - 1;
	}
	if(test < min){
		test = min;
	}
	return test;
}
*/
