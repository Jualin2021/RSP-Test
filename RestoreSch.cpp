#include "RestoreSchClass.h"
#include "RandomFuncs.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <assert.h>

void SortStartTime(vector<int>& st)
{
	std::sort(st.begin(), st.end());
}


int FindValIndex(const vector<int>& vec, int key)
{
	std::vector<int>::const_iterator itr = std::find(vec.begin(), vec.end(), key);
	if (itr != vec.cend()) {
		return static_cast<int>(std::distance(vec.begin(), itr));
		//std::cout << "Element present at index " << std::distance(v.begin(), itr);
	}
	else {
		cerr << "wtf: can not find the index of a key" << endl;
		return -1;
	}
}

//generate the time from order to time
void SCHCLASS::GenerateTimeFromOrder(vector<GRAPH*> VofGraph,const vector<double>& ResCap)
{
	AlignStartTime(VofGraph,ResCap);
}
void SCHCLASS::print() const
{
	for (int sec = 0; sec < 10; sec++) {
		cout << "*******Print and check one sol  sec " << sec << "******" << endl;
		for (size_t i = 0; i < Links.size(); i++)
		{
			cout << "Link = " << Links.at(i)->ID << ", Start Time = " << StartTime[sec].at(i) << ", End Time = " << EndTime[sec].at(i) << endl;
		}
		cout << "********End*************************" << endl;
	}

}

Scenario SCHCLASS::ConvertToScenario() {
	Scenario s;
	for (auto l : Links) s.LinkIds.push_back(l->ID);
	//for (auto t : StartTime) s.tau.push_back(t);
	return s;
}

//update the resource used before the pos project
//does not include the et link, which is supposed to be the current link
void SCHCLASS::updatePrecedingRes(size_t st, size_t et, int s)
{
	  for (size_t l = st; l < et; l++)
	  {
		for (size_t t = StartTime[s].at(l); t < EndTime[s].at(l); t++)
		{
			UsedRes[s].at(t) += Links.at(l)->RequiredRes;///////////////////////////1010
		}
	  }
}

void SCHCLASS::updateResFor(size_t Pos, int s)
{
	  for (int t = StartTime[s].at(Pos); t < EndTime[s].at(Pos); t++)
	  {
		UsedRes[s].at(t) += Links.at(Pos)->RequiredRes;///////////////////////////1010

	  }
}

void SCHCLASS::CheckResFor(size_t Pos, int s)
{
	  for (int t = StartTime[s].at(Pos); t < EndTime[s].at(Pos); t++)
	  {
		UsedRes[s].at(t) -= Links.at(Pos)->RequiredRes;///////////////////////////1010
	  }
}

//find the earliest a time with available resources
int SCHCLASS::findEarliestFeasibleSt(size_t l, const vector<double>& ResCap, int s) {
		for (int t = 0; t < MaxNumOfSchPeriod; t++)
		{
			if (UsedRes[s].at(t) + this->Links.at(l)->RequiredRes <= ResCap.at(t))///////////////////////////1010
			{
				return t;
			}
		}
		cout << "ERR: can not find the earliest start time for project " << l << endl;
		return -1;
}


void SCHCLASS::AlignStartTime(vector<GRAPH*> VofGraph, const vector<double>& ResCap) {
	if (UsedRes.size() > 0)
	{
	    StartTime.clear(); EndTime.clear(); UsedRes.clear();
	}
	for (int s = 0; s < 10; s++) 
	{
		vector<int> tem(Links.size(), 0);
		EndTime.push_back(tem);

		vector<double> temRes(MaxNumOfSchPeriod, 0);
		UsedRes.push_back(temRes);

		vector<int> temST(Links.size(), 0);
		StartTime.push_back(temST);
	}
		for (int s = 0; s < 10; s++)
	{
		for (size_t l = 0; l < Links.size(); l++)
		{
			if (l == 0)
			{
				StartTime[s].at(l) = 0;
				EndTime[s].at(l) = StartTime[s].at(l) + (*(VofGraph[s])).Links.at(Links[l]->ID).RecoverTime;
			}
			else
			{
				updatePrecedingRes(l - 1, l, s);
				StartTime[s].at(l) = findEarliestFeasibleSt(l, ResCap, s);
				EndTime[s].at(l) = StartTime[s].at(l) + (*(VofGraph[s])).Links.at(Links[l]->ID).RecoverTime;
			}
		}
		updateResFor(this->Links.size() - 1, s);
	}
}

void SCHCLASS::GenerateIniSch(GRAPH& g, const vector<int>& FailureLinks)
{
	assert(FailureLinks.size() > 0);
	if (this->Links.size() > 0)
	{
		Links.clear(); StartTime.clear(); EndTime.clear();
	}
	// step 1 generate ini number of links
	int NumOfFailureLinks = (int)FailureLinks.size();
	vector<bool> isSelected(FailureLinks.size(), false);
	int CountDoWhile = 0;
	do
	{
		int linkNum = GenRandomInt(FailureLinks);
		int pos = FindValIndex(FailureLinks, linkNum);
		if (!isSelected.at(pos))
		{
			this->Links.push_back(new LINK());
			Links.back() = &g.Links.at(linkNum);
			isSelected.at(pos) = true;
		}
		CountDoWhile++;
		if (CountDoWhile > 1000)
		{
			cout << "ERR: Do while count is larger than 1000" << endl;
			system("PAUSE");
		}
	} while (std::find(isSelected.begin(), isSelected.end(), false) != isSelected.end());
}

vector<size_t> SCHCLASS::getNewReadyLinks(int tau, int s)
{

	vector<size_t> results;
	if (tau == 0) return results;
	else
	{
		for (size_t l = 0; l < Links.size(); l++)
		{
			if (EndTime[s].at(l) == tau)///////////////////////////1010
			{
				results.push_back(l);
			}
		}
	}

	return results;
}


void SCHCLASS::Evaluate(vector<GRAPH*> VofGraph, const vector<int>& FailureLinks)
{
	//vector<GRAPH>& VofGraph
	//const vector<double> Opt = {854272.0,638806.7,738470.8,1057711.6,667371.6,784594.4,973766.6,665888.2,764686.7,686645.1};//ND,1.0D,0.01,R2£¬UE500
	const vector<double> Opt = { 4098.7,5218.6,4188.0,3928.8,4745.1,5218.6,4257.4,5253.4,5247.9,3547.2 };//d=6,NEW
	Fitness = 0.0;
	vector<double> VecRegTime;
	if(TravelTime.size() > 0)
	{
		TravelTime.clear(); UNPM.clear();
	}
	for (int s = 0; s < 10; s++)
	{
		vector<double> temTT(GetLastPeriod(s), 0);
		TravelTime.push_back(temTT);

		vector<double> temUN(GetLastPeriod(s), 0);
		UNPM.push_back(temUN);
	}
	for (int s = 0; s < 10; s++)
	{
#ifdef _DEBUG
		for (auto l : (*(VofGraph[s])).Links)
		{
			if (l.CaRevise < l.CaInput - 1)
			{
				cout << "Warning: the capacity is not set to initial value before the evalute procedure" << endl;
				system("PAUSE");
			}
		}
#endif
		vector<size_t> CumulativeReadyLinks;
		TravelTime[s].assign(GetLastPeriod(s), 0);
		UNPM[s].assign(GetLastPeriod(s), 0);
		// step 1: set all the capacity of failure links to be 0.0 and evalute the total cost in tau = 0
		for (size_t l = 0; l < Links.size(); l++)
		{
			Links.at(l)->CaRevise = Zero;
		
		}
		for (size_t l = 0; l < FailureLinks.size(); l++)
		{
			(*(VofGraph[s])).Links.at(FailureLinks.at(l)).CaRevise = Zero;
		}
		(*(VofGraph[s])).EvaluteGraph();

		// step 2: compute the time in each period
		for (int t = 0; t < GetLastPeriod(s) + 1; t++)
		{
			vector<size_t> NewReady = getNewReadyLinks(t,s);
			// case 1: at the begin, there is not other links
			if (NewReady.size() == 0)
			{
				if (t == 0)
				{
					TravelTime[s].at(t) = (*(VofGraph[s])).TotalSystemCost;
					UNPM[s].at(t) = (*(VofGraph[s])).UNPM;
				}
				else
				{
					TravelTime[s].at(t) = TravelTime[s].at(t - 1);
					UNPM[s].at(t) = UNPM[s].at(t - 1);
				}
#ifdef _DEBUG
				cout << "---Period = " << t << ", no link is added" << endl;
#endif // _DEBUG
			}
			else
			{
				for (auto l : NewReady) Links.at(l)->IniCap();
				for (auto l : NewReady)
				{
					for (int i = 0; i < (*(VofGraph[s])).Links.size(); i++)
					{
						if ((*(VofGraph[s])).Links[i].ID == Links.at(l)->ID)
						{
							(*(VofGraph[s])).Links[i].IniCap();
							break;
						}
					}
				}

				(*(VofGraph[s])).EvaluteGraph();
#ifdef _DEBUG
				cout << "---Period = " << t << "," << NewReady.size() << " link is added" << endl;
				cout << "---Period = " << t << "," << "min cost = " << (*(VofGraph[s])).OdPairs.at(0).MinCost << "unpm=" << (*(VofGraph[s])).UNPM << endl;;
#endif // _DEBUG
				CumulativeReadyLinks.insert(CumulativeReadyLinks.end(), NewReady.begin(), NewReady.end());
				if (t == GetLastPeriod(s))
				{
					//TravelTime.push_back(0);
				}
				else
				{
					TravelTime[s].at(t) = (*(VofGraph[s])).TotalSystemCost;
					UNPM[s].at(t) = (*(VofGraph[s])).UNPM;
				}
			}
		}
		VecRegTime.push_back(std::accumulate(TravelTime[s].begin(), TravelTime[s].end(), 0.0) - Opt[s]);
	}
	sort(VecRegTime.begin(), VecRegTime.end());
	this->Fitness = (VecRegTime[8]+VecRegTime[9])/2;////0.8
#ifdef _DEBUG

	for (int s = 0; s < 10; s++) {
		cout << "-------summary of travel time----------" << endl;
		cout << "S = " << s << endl;
		cout << "Last end time = " << GetLastPeriod(s) << endl;
		for (int t = 0; t < TravelTime[s].size(); t++)
		{
			cout << "t = " << t << ", TravelTime = " << TravelTime[s].at(t) << ",";
			cout << "UNPM = " << UNPM[s].at(t) << endl;
		}
		//
	}
	cout << "VaR = " << this->Fitness << endl;// regrettime!
#endif // _DEBUG
}

