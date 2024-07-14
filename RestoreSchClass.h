#pragma once

#ifndef RestoreClass
#define RestoreClass
#include <iostream>
#include <vector>
#include "Parameters.h"
#include "TempleteFunc.h"
#include "GlobalVar.h"
using namespace std;

class PatternClass
{
public:
	int id;
	int LinkId;
	vector<double> Score;
	vector<double> Prob;
	vector<int> next;
	PatternClass() { id = -1; LinkId = -1; Score.reserve(100); Prob.reserve(100); }
	~PatternClass() { Score.clear(); Prob.clear(); id = -1; LinkId = -1; }
	void updateProb();
};

class SCHCLASS   // class for the schedule 
{
public:
	int ID;
	//std::vector<int> LIN;
	std::vector<LINK*> Links;
	std::vector< vector<int> >StartTime;
	std::vector<vector<int> > EndTime;
	std::vector<vector<double> > UsedRes; /// resource consumption 
	std::vector<vector<double> > TravelTime;
	std::vector<vector<double> > UNPM;
	double Fitness;
	double RegretTime;//////////////////////////919
	SCHCLASS() {
		ID = -1; Fitness = -1; Links.reserve(100); StartTime.reserve(100); 

	};
	SCHCLASS(int _id) {
		ID = _id; Fitness = -1;
		Links.reserve(100); StartTime.reserve(100); 
	}
	/// <summary>
	/// It is important to write this copy constructor
	/// </summary>
	/// <param name="obj"></param>
	SCHCLASS(const SCHCLASS& obj) {
		for (int l = 0; l < obj.Links.size(); l++) this->Links.push_back(obj.Links.at(l));
		this->StartTime.assign(obj.StartTime.begin(), obj.StartTime.end());
		this->EndTime = obj.EndTime;
		this->UsedRes = obj.UsedRes;
		this->TravelTime = obj.TravelTime;
		this->UNPM = obj.UNPM;
		this->ID = obj.ID;
		this->Fitness = obj.Fitness;

	};  // copy constructor
	~SCHCLASS()
	{
		if (Links.size() > 0)
		{
			Links.clear();
		}
		StartTime.clear(); EndTime.clear(); UsedRes.clear(); TravelTime.clear(); UNPM.clear();
	}
	// functions
	Scenario ConvertToScenario(); // convert sch class to scenario and return 
	void AlignStartTime(vector<GRAPH*> VofGraph, const vector<double> &ResCap); // improve the solution itself
	bool isFeasible(const vector<double> &res, size_t l, int s); 
	//bool isEarliestFeasible(const vector<double>& res, int s);/////////////////////////////////
	void GenerateIniSch(GRAPH& g, const vector<int>& FailureLinkSet);
	void GenerateIniBasedOnPattern(GRAPH& g, const vector<int>& FailureLinkSet,
		const vector<PatternClass> &pat);
	void print() const;
	void getRes();
	void updatePrecedingRes(size_t st,size_t et, int s);
	void updateResFor(size_t Pos, int s);
	void CheckResFor(size_t Pos, int s);//919
	int findEarliestFeasibleSt(size_t l, const vector<double>& ResCap, int s);
	void Evaluate(vector<GRAPH*> VofGraph, const vector<int>& failureLinks);
	int GetLastPeriod(int sceID)
	{
		return *max_element(EndTime[sceID].begin(), EndTime[sceID].end()); ;
	}
	vector<size_t> getNewReadyLinks(int tau, int s);

	void GenerateTimeFromOrder(vector<GRAPH*> VofGraph,const vector<double>& ResCap);

	SCHCLASS& operator=(const SCHCLASS& rhs) {
		ID = rhs.ID; 
		StartTime.assign(rhs.StartTime.begin(), rhs.StartTime.end());
		EndTime = rhs.EndTime;
		UsedRes = rhs.UsedRes;
		Fitness = rhs.Fitness;
		Links.assign(rhs.Links.begin(), rhs.Links.end());
		TravelTime =rhs.TravelTime;
		UNPM = rhs.UNPM;
		return* this;
	}
};

#endif
