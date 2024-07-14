// uniform_int_distribution
#include "CommonHeaders.h"
#include <iostream>
#include <random>
#include <vector>
int GenRandomInt(const std::vector<int> &Vec)
{
	std::uniform_int_distribution<unsigned int> distribution(0,(unsigned int) (Vec.size()-1));
	unsigned int Num = distribution(GenRan);
	return Vec.at(Num);
}
int GenRandomInt(int MinVal, int MaxVal)
{
	std::uniform_int_distribution<unsigned int> distribution(MinVal,MaxVal);
	return static_cast<int>(distribution(GenRan));
}



double GenRandomFloat(const std::vector<double> &Vec)
{
	std::uniform_int_distribution<unsigned int> distribution(0, (unsigned int)(Vec.size()-1));
	unsigned int Num = distribution(GenRan);
	return Vec.at(Num);
}

int GenRandomPos(unsigned int siz)
{
	std::uniform_int_distribution<unsigned int> distribution(0, siz-1);
	return distribution(GenRan);
}
double GenRandomReal()
{
	std::uniform_real_distribution<double> distribution(0.0f,1.0f);
	return distribution(GenRan);
}
