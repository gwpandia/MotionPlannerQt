#pragma once
#include <sstream>
#include <string>

namespace IMMP{
template<class T>
class Util
{
public:
	static T adjustRange(T test, T min, T max){
		if(test >= max){
			test = max - 1;
		}
		if(test < min){
			test = min;
		}
		return test;
	}

	static std::string toString(T input){
		std::stringstream ss;
		ss << input;
		return ss.str();
	}

};

};
