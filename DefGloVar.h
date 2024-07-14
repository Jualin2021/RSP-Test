// define global variables
#pragma once
#ifndef DGV
#define DGV
#include <random>
#include <fstream>
//define global
double Zero;
int NumOrig;
int NumDest;
int NumLinks;
int NumNodes;
int NumOD;
double  OneDimEsp;
double  UEeps;
int	   UEmaxIter;
std::default_random_engine GenRan;
int NetworkIndex;
int MaxGAIter;
int GANumPop;
int GANumChild;
bool isWriteConverge;
int MaxNumSolEval;
int StopCriteria;
int CSACloneMethod;
double GaMutationRate;
int SeedRunCount;
std::vector<int> SeedVecVal;
bool UseMyOwnAlgo;
bool isTestSingleOperator;
int TestSingleOpIndex;


#endif
