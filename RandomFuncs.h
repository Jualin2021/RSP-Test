#ifndef RANFUNS
#define RANFUNS
#include <vector>
double GenRandomReal();
int GenRandomInt(const std::vector<int> &Vec);
int GenRandomInt(int MinVal, int MaxVal);
double GenRandomFloat(const std::vector<double> &Vec);
int GenRandomPos(unsigned int siz);
#endif // !RANFUNS
