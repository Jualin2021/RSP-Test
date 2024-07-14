#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;

bool ReadLinkData(std::vector<LINK>& Links,
	ifstream& fin) {
	LINK tl;
	int IDcount = 0;
	//int tail, head;
	//float t0, ca0;
	//float BprAlph, BprBeta;
	vector<string> fields;
	string line;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0]) < 0)
			continue;
		/*	fscanf_s(fin, "%i %i %f %f %f %f",
				&tail, &head, &t0, &ca0, &BprAlph, &BprBeta);*/
		Links.push_back(tl);
		Links.back().ID = IDcount;
		Links.back().Tail = std::stoi(fields[0]);
		Links.back().Head = std::stoi(fields[1]);
		Links.back().T0 = std::stof(fields[2]);
		Links.back().CaInput = std::stof(fields[3]);
		Links.back().CaRevise = std::stof(fields[3]);
		Links.back().AlphaBpr = std::stof(fields[4]);
		Links.back().BetaBBpr = std::stof(fields[5]);
		IDcount++;
	}
	return true;
}

GRAPH::GRAPH(){
	this->OdPairs.reserve(NumOD + 1);
	this->OriginSet.reserve(NumNodes);
	this->Nodes.reserve(NumNodes + 1);
	this->Links.reserve(NumLinks + 1);
	TotalSystemCost = 0.0;

	//int** MinPathPredLink;
	if (NumNodes==0)
	{
		cerr << "Input node should be known before construct graph" << endl;
	}
	this->MinPathPredLink
		= Create2DIntVec(NumNodes + 1, NumNodes + 1);
	for (int i = 0; i < NumNodes + 1; i++)
	{
		for (int j = 0; j < NumNodes + 1; j++)
		{
			this->MinPathPredLink[i][j] = InvaildInt;
		}
	}

};
GRAPH::~GRAPH(){
	//Free2DArrey<int>(this->MinPathPredLink, NumNodes + 1);
};

int GRAPH::FindMinCostRoutes(){

	try{
		int StatusMsg;
		std::vector<double> Lable;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			int Orig = o->Onode;
			if (Orig<0) continue;
			StatusMsg = this->SP(o->Onode, Lable);// shortest path
			assert(StatusMsg);
			//printf("Find min tree for Onode = %d \n", Orig);
			//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
			for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
			{
				int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
				if (isnan(Lable[Dest])) DEBUG("Od Pair %d,mincost is NaN", *d);
#endif	
				this->OdPairs.at((*d)->ID).MinCost = Lable[Dest];

				//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
				if (this->OdPairs.at((*d)->ID).MinCost < InvalidMinCost)
				{
					this->OdPairs.at((*d)->ID).isConnected = true;
				}
				/*if (ODPairs.at(*d).MinCost.at(ModeType) < InvalidMinCost)
					ODPairs.at(*d).isConnected = true;*/
			}
		}
		return 1;
	}

	catch (exception& e)
	{
		TRACE("FindMinCostRoutes %s", e.what());
		return 0;
	}
}

void GRAPH::CreateOriginSet()
{
	OriginBasedOD Oset;
	vector<bool> isOrign(NumNodes, false);
	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		//cout << od->Orign << endl;
		isOrign.at(od->Orign) = true;
	}
	for (int n = 0; n < NumNodes; n++) {

		this->OriginSet.push_back(Oset);
		if (isOrign.at(n))	this->OriginSet.back().Onode = n;
		//if (isOrign.at(n))
		//{
		//	Oset.Onode = n;
		//	this->OriginSet.push_back(Oset);
		//}
	}

	for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
	{
		this->OriginSet.at(od->Orign).ODset.push_back(&*od);
	}

	cout << "finish create origin set" << endl;
}

void GRAPH::CreateNodes(){
	cout <<"Read Number of Nodes in the network is"<<this->Links.size() << endl;
	assert(this->Links.size() > 0);
	NODE tnode;
	for (int i = 0; i < NumNodes; i++)
	{
		this->Nodes.push_back(tnode);
		this->Nodes.back().ID = i;
	}
	for (auto l = this->Links.begin(); l != this->Links.end(); l++)
	{
		this->Nodes.at(l->Tail).OutLinks.push_back(&*l);
		this->Nodes.at(l->Head).InLinks.push_back(&*l);
	}
}

bool ReadDemandData(vector<OD>& ODPairs,
	ifstream& fin) {

	int IDcount = 0;
	//int or , de;
	//float dd;
	vector<string> fields;
	string line;
	OD tod;

	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (std::stoi(fields[0]) < 0)
			continue;

		ODPairs.push_back(tod);
		ODPairs.back().ID = IDcount;
		ODPairs.back().Orign = std::stoi(fields[0]);
		ODPairs.back().Dest = std::stoi(fields[1]);
		ODPairs.back().Demand = std::stof(fields[2]);
		IDcount++;
	}

	return true;
}


void GRAPH::ReadGraphData()
{

	//FILE * fin;
	std::ifstream fin;
	// Read Demand data
	if (NetworkIndex == 1) // Scan
	{
		fin.open("..//Input//MediumNetwork//DeamdData.txt");
	}
	else if (NetworkIndex == 2)
	{
		fin.open("..//Input//2009Network//DeamdData.txt");
	}
	else if (NetworkIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//DeamdData.txt");
	}
	else if (NetworkIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//DeamdData.txt");
	}
	else if (NetworkIndex ==5)
	{
		fin.open("..//Input//WangNetwork//DeamdData.txt");
	}
	else if (NetworkIndex == 6)
	{
		fin.open("..//Input//Nguyue-Dupuisnetwork//DeamdData.txt");
	}
	else
	{
		cout << "Mode Index is not specified" << endl;
		system("PAUSE");
		fin.open("..//Input//DeamdData.txt");
	}

	if (!ReadDemandData(OdPairs, fin))
	{
		cerr << "read demand data fails" << endl;
		system("PAUSE");
	}

	fin.close();

	// Read link data
	if (NetworkIndex == 1) // Scan
	{
		fin.open("..//Input//MediumNetwork//LinkData.txt");
	}
	else if (NetworkIndex == 2)
	{
		fin.open("..//Input//2009Network//LinkData.txt");
	}
	else if (NetworkIndex == 3)
	{
		fin.open("..//Input//SiouxFallsNetwork//LinkData.txt");
	}
	else if (NetworkIndex == 4)
	{
		fin.open("..//Input//ParadoxNet//LinkData.txt");
		//fopen_s(&fin, "..//Input//SiouxFallsNetwork//LinkData.txt", "r");
	}
	else if (NetworkIndex == 5)
	{
		fin.open("..//Input//WangNetwork//LinkData.txt");
	}
	else if (NetworkIndex == 6)
	{
		fin.open("..//Input//Nguyue-Dupuisnetwork//LinkData.txt");
	}
	else
	{
		fin.open("..//Input//LinkData.txt");
	}

	if (!fin.is_open())
		cout << "link data file is not open" << endl;

	if (!ReadLinkData(Links, fin))
	{
		cerr << "read link data fails" << endl;
		system("PAUSE");
	}
	fin.close();
	CreateOriginSet();
	CreateNodes();
}


int GRAPH::PrintLinks(std::ofstream &fout){

	try{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "ID" << ",";
		fout << left << setw(12) << "Tail" << ",";
		fout << left << setw(12) << "Head" << ",";
		fout << left << setw(12) << "T0" << ",";
		fout << left << setw(12) << "Ca0" << ",";
		fout << left << setw(12) << "CaRev" << ",";
		fout << left << setw(12) << "Cost" << ",";
		fout << left << setw(12) << "Flow" <<endl;
		//fout << endl;
		for (auto l = this->Links.begin(); l != this->Links.end(); l++)
		{
			fout << l->ID << ",";
			fout << l->Tail << ",";
			fout << l->Head << ",";
			fout << l->T0 << ",";
			fout << l->CaInput << ",";
			fout << l->CaRevise << ",";
			fout << l->Cost << ",";
			fout << l->Flow <<endl;
			//fout << endl;
		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}
}

int GRAPH::PrintLinks_onscreen() {
	try {
		cout.setf(ios::fixed);
		cout << left << setw(6) << "ID" << ",";
		cout << left << setw(12) << "Tail" << ",";
		cout << left << setw(12) << "Head" << ",";
		cout << left << setw(12) << "T0" << ",";
		cout << left << setw(12) << "Ca0" << ",";
		cout << left << setw(12) << "CaRev" << ",";
		cout << left << setw(12) << "Cost" << ",";
		cout << left << setw(12) << "Flow" << endl;
		//fout << endl;
		for (auto l = this->Links.begin(); l != this->Links.end(); l++)
		{
			cout << l->ID << ",";
			cout << l->Tail << ",";
			cout << l->Head << ",";
			cout << l->T0 << ",";
			cout << l->CaInput << ",";
			cout << l->CaRevise << ",";
			cout << l->Cost << ",";
			cout << l->Flow << endl;
			//fout << endl;
		}
		cout << " Total Cost = " << TotalSystemCost << endl;
		return 1;
	}
	catch (exception& e)
	{
		TRACE("%s", e);
		return 0;
	}

}

int GRAPH::PrintOD(std::ofstream &fout)
{
	try
	{
		assert(fout.good());
		fout.setf(ios::fixed);
		fout << left << setw(6) << "ID" << ",";
		fout << left << setw(12) << "Origin" << ",";
		fout << left << setw(12) << "Dest" << ",";
		fout << left << setw(12) << "Demand" << ",";
		fout << left << setw(12) << "MinCost" << endl;
		//fout << endl;

		for (auto od = this->OdPairs.begin(); od != this->OdPairs.end(); od++)
		{
			fout << od->ID << ",";
			fout << od->Orign << ",";
			fout << od->Dest << ",";
			fout << od->Demand << ",";
			fout << od->MinCost << endl;
			//fout << endl;

		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}

}

int GRAPH::PrintSp(int Orign, int Dest, std::ofstream &fout)
{
	try{

		vector<int> Path;
		for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
		{
			for (auto od = o->ODset.begin(); od != o->ODset.end(); od++)
			{
				if ((*od)->Orign == Orign && Dest == (*od)->Dest)
				{
					Path.clear();
					int CurrentNode = (*od)->Dest;
					while (CurrentNode != (*od)->Orign)
					{
						int k = this->MinPathPredLink[(*od)->Orign][CurrentNode];
						Path.push_back(k);
						CurrentNode = this->Links.at(k).Tail;
					}
					for (auto i = Path.rbegin(); i != Path.rend(); i++)
					{
						fout << *i << ",";
					}
					fout << endl;
				}
			}
		}
		return 1;
	}
	catch (exception &e)
	{
		TRACE("%s", e);
		return 0;
	}
}

void GRAPH::EvalutateFailureScenarios(const Scenario &s)
{
	if (s.tau.size() != s.LinkIds.size())
	{
		cout << "err: the size of tau and linked should be equal" << endl;
		system("pause");
	}

	cout << "------Eva Link Cap Fail-----------------------" << endl;
	for (int i = 0; i < 1; i++)  // loop all the period
	{
		cout << "Remark: Only Evaluate one period for test case" << endl;
		for (size_t j = 0; j < s.tau.size(); j++)
		{
			if (i == s.tau.at(j))
			{
				this->Links.at(s.LinkIds.at(j)).CaRevise = Zero;
				//Links.at(l)->CaRevise = Zero;
			}
		}
		this->EvaluteGraph();
		cout << "------Summary for period = " << i << "-------" << endl;
		this->PrintLinks_onscreen();
		cout << "------End of summary for period = " << i << "-------" << endl;
	}
	cout << "------End of Eva Link Cap Fail-----------------------" << endl;
}

void GRAPH::RevertFailureScenarios(const Scenario& s)
{
	if (s.tau.size() != s.LinkIds.size())
	{
		cout << "err: the size of tau and linked should be equal" << endl;
		system("pause");
	}

	cout << "------Restore Link Cap-----------------------" << endl;
	for (int i = 0; i < MaxNumOfSchPeriod; i++)  // loop all the period
	{
		for (size_t j = 0; j < s.tau.size(); j++)
		{
			if (i == s.tau.at(j))
			{
				this->Links.at(s.LinkIds.at(j)).IniCap();
			}
		}
		this->EvaluteGraph();
		cout << "------Summary for period = " << i << "-------" << endl;
		this->PrintLinks_onscreen();
		cout << "------End of summary for period = " << i << "-------" << endl;
	}
	cout << "------End Restore Link Cap-----------------------" << endl;
}

/// <summary>
/// compute the relative changes for the graph with and without the link
/// </summary>
/// <param name="link"></param>
double GRAPH::CalRelSpChange(int LinkID)
{
	// Step 0: Compute the Sp labels before remove the links
	int StatusMsg;
	for (int i = 0; i < Links.size(); i++)
	{
		Links.at(i).Cost= Links.at(i).IniCost();
	}
	std::vector<double> Lable;
	for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	{
		int Orig = o->Onode;
		if (Orig < 0) continue;
		StatusMsg = this->SP(o->Onode, Lable);// shortest path
		assert(StatusMsg);
		//printf("Find min tree for Onode = %d \n", Orig);
		//Minpath(Orig, MinPathPredLink[Orig], Lable, Nodes, Links, ModeType);
		for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		{
			int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d)->ID).BeforeRemoveSpDist = Lable[Dest];
			//ODPairs.at(*d).MinCost.at(ModeType) = Lable[Dest];
			if (this->OdPairs.at((*d)->ID).BeforeRemoveSpDist < InvalidMinCost)
			{
				this->OdPairs.at((*d)->ID).BeforeRemoveSpDist = true;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Step 1: Remove the link in the current network
	this->Links.at(LinkID).Cost = RemoveLinkCost;
	//////////////////////////////////////////////////////////////////////////
	for (auto o = this->OriginSet.begin(); o != this->OriginSet.end(); o++)
	{
		int Orig = o->Onode;
		if (Orig < 0) continue;
		StatusMsg = this->SP(o->Onode, Lable);// shortest path
		assert(StatusMsg);
		for (auto d = o->ODset.begin(); d != o->ODset.end(); d++)
		{
			int Dest = (*d)->Dest;
#ifdef __DEBUG__ 
			if (isnan(Lable[Dest])) DEBUG("Od Pair %d,min cost is NaN", *d);
#endif	
			this->OdPairs.at((*d)->ID).AfterRemoveSpDist = Lable[Dest];
			if (this->OdPairs.at((*d)->ID).AfterRemoveSpDist < InvalidMinCost)
			{
				this->OdPairs.at((*d)->ID).isConnected = true;
			}
		}
	}
	
	double EIvalue = 0.0;
	for (auto o:OdPairs)
	{
#ifdef _DEBUG
		assert(o.AfterRemoveSpDist > 0);
		assert(o.BeforeRemoveSpDist > 0);
#endif // _DEBUG
		EIvalue += (1/ o.BeforeRemoveSpDist- 1 / o.AfterRemoveSpDist);
	}

	//Final step add back the link
	this->Links.at(LinkID).Cost = this->Links.at(LinkID).IniCost();

	return EIvalue;
}
