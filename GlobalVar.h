#pragma once
#ifndef GLOBALVAR
#define GLOBALVAR
#include <fstream>
#include <random>
//Constant
extern bool isTestSingleOperator;
extern int TestSingleOpIndex;
extern double Zero;
extern int NumOrig;
extern int NumDest;
extern int NumLinks;
extern int NumNodes;
extern int NumOD;
//extern std::ofstream TestStepSize/*;*/
extern std::ofstream Log;
extern std::ofstream AssertLog;
extern double OneDimEsp;
extern double UEeps;
extern int UEmaxIter;
extern std::default_random_engine GenRan;
extern int NetworkIndex;
extern int MaxGAIter;
extern int GANumPop;
extern int GANumChild;
//extern int MaxNumSolEval;
extern bool isWriteConverge;
//extern int StopCriteria;  
extern double GaMutationRate;
extern int SeedRunCount;
extern std::vector<int> SeedVecVal;
extern bool UseMyOwnAlgo;
#endif