#pragma once
#ifndef GAclass
#define GAclass
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
#include "RestoreSchClass.h"
using namespace std;

class OperatorClass
{
public:
	int id;
	int TotalCounterGood;  // number of calls improve
	int TotalCounterBad; // number of counters drops
	int TotalCounterSum; // Total number of counters 
	double Score;
	double Prob;
	double Weight;
};


class GAAlgorithms // to begin with use GA algorithm
{
public:
	double BaseUNPM;
	vector<GRAPH*> VofGraph;//////////////////////////////////////////////////1017
	GRAPH* Graph;
	double MaxFitValue;
	double MinFitValue;
	vector<CHROME> Chroms;
	int GANumPop;
	int GANumChild;
	int MaxGAIter;
	std::vector<double> setResourceCap;  // Capacity of the resources
	std::vector<int> setOfFailureLinks;
	SCHCLASS GlobalBest;
	vector<double> ConvergeMeasure;
	vector<PatternClass> Pattern;
	SelectOperatorType SelectOp;
	void Ini();
	GAAlgorithms() {
		MaxFitValue = -99999999999999; MinFitValue = 999999999999999;
		MaxGAIter = -1; VofGraph.clear();
		setResourceCap.reserve(100); setOfFailureLinks.reserve(100);
		Graph = new GRAPH;
		SelectOp = SelectOperatorType::None;
	};
	~GAAlgorithms() { //Graph = nullptr;
	};
	std::vector<SCHCLASS> Sols;
	void ReadSolAndEvaluate(vector<int>& vec);
	void GenerateIni();
	void GAMain();
	void GAselectParent(int& Father, int& Mother, const int NumPop);
	void GACrossOver(CHROME& Father, CHROME& Mother, CHROME& BigBro, CHROME& CuteSis);
	void GAMutation();
	void ReadData();
	void PrintFinal(int sd);
};


#endif
