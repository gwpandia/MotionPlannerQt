#pragma once
#include <vector>
#include "Robot.h"
#include "Obstacle.h"
#include "Constant.h"
#include "Configuration.h"

namespace IMMP{
	using namespace IMMP;
class ObstacleBitmap
{
public:
	ObstacleBitmap(const std::vector<Obstacle>&);
	~ObstacleBitmap(void);
	void drawObstacles();
	std::vector<QPolygonF> drawCSpace(const Robot&, const Configuration&);
	void getBitMap(int(*)[Constant::BITMAPSIZE])const;
	int bitMapAt(int, int);
private:
	void initialBitMap();
	void drawObstacle(const QPolygonF&);
	QPolygonF minkowskiSums(const QPolygonF&, const QPolygonF&);
	QPolygonF minusPolygon(const QPolygonF&)const;
	std::vector<QPointF> initPolygonVertex(int, const QPolygonF&)const;
	int findFirstVertex(const QPolygonF&)const;
	//int adjustRange(int, int, int);
	//const std::vector<Robot>& robots;
	const std::vector<Obstacle>& obstacles;
	int obsbitmap[Constant::BITMAPSIZE][Constant::BITMAPSIZE];
};

};