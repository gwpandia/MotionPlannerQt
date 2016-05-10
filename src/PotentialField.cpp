#include "PotentialField.h"
#include "Util.h"
#include "Constant.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace IMMP;

PotentialField::PotentialField(int gx, int gy, const ObstacleBitmap& ob):goalx(gx), goaly(gy), maxVal(INT_MIN), minVal(INT_MAX){
	ob.getBitMap(potentialfield);
	NF1();
	/*std::ofstream ofs("aaa.txt");
	std::cout << "Here" << std::endl;
	for(int i = 0; i < Constant::BITMAPSIZE; i++){
		for(int j = 0; j < Constant::BITMAPSIZE; j++){
			if(potentialfield[i][j] == Constant::OBST){
				ofs << "###";
			}
			else{
				ofs << potentialfield[i][j] << " ";
			}
		}
		ofs << std::endl;
	}*/
}


PotentialField::~PotentialField(void)
{
}

int PotentialField::getPotentialValue(int x, int y)const{
	return potentialfield[x][y];
}

void PotentialField::NF1(){
	int hd[] = {1, -1, 0, 0};
	int vd[] = {0, 0, 1, -1};

	std::vector<Config> explode_queue;
	int depth = Constant::GOAL;
	explode_queue.push_back(Config(goalx, goaly, depth));
	
	while(!explode_queue.empty()){
		Config f = explode_queue.front();
		potentialfield[f.x][f.y] = f.depth;
		updateMinMaxVal(f.depth);

		for(int i = 0; i < 4; i++){
			int new_x = f.x + hd[i];
			int new_y = f.y + vd[i];

			new_x = Util<int>::adjustRange(new_x, 0, Constant::BITMAPSIZE);
			new_y = Util<int>::adjustRange(new_y, 0, Constant::BITMAPSIZE);

			if(potentialfield[new_x][new_y] != Constant::OBST && potentialfield[new_x][new_y] == Constant::INIT && (f.depth + 1 < Constant::OBST)){
				explode_queue.push_back(Config(new_x, new_y, f.depth + 1));
				potentialfield[new_x][new_y] = f.depth + 1;
				updateMinMaxVal(f.depth+1);
			}
		}

		explode_queue.erase(explode_queue.begin());
	}

}

void PotentialField::updateMinMaxVal(int pv){
	if(pv < minVal){
		minVal = pv;
	}
	if(pv > maxVal){
		maxVal = pv;
	}
}