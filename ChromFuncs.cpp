#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;

CHROME::CHROME(){
	this->ID = InvaildInt;
	this->Nodes.reserve(NumNodes);
	this->NodeDof.reserve(NumNodes);
	this->NodeDofProb.reserve(NumNodes);

	this->Links.reserve(NumLinks);
	this->LinkDof.reserve(NumLinks);
	this->LinkDofProb.reserve(NumLinks);

	SolProb = InvaildFloat;
	TotalCost = InvaildFloat;
	UNPM = InvaildFloat;
	ImpactValue = InvaildFloat;
	Fitness = InvaildFloat;
}
CHROME::~CHROME(){
	this->Nodes.clear();
	this->NodeDof.clear();
	this->NodeDofProb.clear();

	this->Links.clear();
	this->LinkDof.clear();
	this->LinkDofProb.clear();

};
void CHROME::ReviseCap(GRAPH &Graph){

	// revised on 13 - Sep - 2018 based on the new capacity function 
	for (unsigned int i = 0; i < this->Nodes.size(); i++)
	{
		int n = this->Nodes.at(i);
		// incoming link
		for (auto l = Graph.Nodes.at(n).InLinks.begin();
			l != Graph.Nodes.at(n).InLinks.end(); l++)
		{

			//Graph.Links.at((*l)->ID).CaRevise =
				//Graph.Links.at((*l)->ID).CaRevise*(1.0f - this->NodeDof.at(i));

			Graph.Links.at((*l)->ID).CaRevise
				= Graph.Links.at((*l)->ID).CaInput
				* min(Graph.Links.at((*l)->ID).CaRevise / Graph.Links.at((*l)->ID).CaInput,
					1.0f - this->NodeDof.at(i));

		}
		// out going link

		for (auto l = Graph.Nodes.at(n).OutLinks.begin();
			l != Graph.Nodes.at(n).OutLinks.end(); l++)
		{
			//Graph.Links.at((*l)->ID).CaRevise
			//	= Graph.Links.at((*l)->ID).CaRevise*(1.0f - this->NodeDof.at(i));
			Graph.Links.at((*l)->ID).CaRevise
				= Graph.Links.at((*l)->ID).CaInput
				*min(Graph.Links.at((*l)->ID).CaRevise / Graph.Links.at((*l)->ID).CaInput,
					1.0f - this->NodeDof.at(i));
		}
	}

	for (unsigned int i = 0; i < this->Links.size(); i++)
	{
		//Graph.Links.at(i).CaRevise
			//= Graph.Links.at(i).CaRevise*(1.0f - this->LinkDof.at(i));


		Graph.Links.at(i).CaRevise
			= Graph.Links.at(i).CaInput *
			min(Graph.Links.at(i).CaRevise / Graph.Links.at(i).CaInput, 1.0f - this->LinkDof.at(i));

	}
}

void CHROME::IniCap(GRAPH &ScenarioGraph){
	for (unsigned int i = 0; i < this->Nodes.size(); i++)
	{
		int n = this->Nodes.at(i);
		// incoming link
		for (auto l = ScenarioGraph.Nodes.at(n).InLinks.begin();
			l != ScenarioGraph.Nodes.at(n).InLinks.end(); l++)
		{
			ScenarioGraph.Links.at((*l)->ID).IniCap();
		}
		// out going link

		for (auto l = ScenarioGraph.Nodes.at(n).OutLinks.begin();
			l != ScenarioGraph.Nodes.at(n).OutLinks.end(); l++)
		{
			ScenarioGraph.Links.at((*l)->ID).IniCap();
		}
	}

	for (unsigned int i = 0; i < this->Links.size(); i++)
	{
		 ScenarioGraph.Links.at(i).IniCap();
	}
};


int CHROME::PrintSol(ofstream &fout){

	//fout << "OutPut Fail Nodes Prob******" << endl;
	vector<bool> isFailNode(NumNodes, false);
	for (auto i = this->Nodes.begin(); i != this->Nodes.end();i++)
	{
		isFailNode[*i] = true;
	}
	for (int i = 0; i < NumNodes;i++)
	{
		if (!isFailNode[i]) continue;
		for (unsigned int j = 0; j < this->Nodes.size();j++)
		{
			if (this->Nodes.at(j) == i)
			{
				fout << this->NodeDof.at(j) << ",";
			}
		}
	}
	fout << endl;

	if ( this->Links.size()>0)
	{
		vector<bool> isFailLink(NumLinks, false);
		for (auto i = this->Links.begin(); i != this->Links.end(); i++)
		{
			isFailLink[*i] = true;
		}
		for (int i = 0; i < NumLinks; i++)
		{
			if (!isFailLink[i]) continue;
			for (unsigned int j = 0; j < this->Links.size(); j++)
			{
				if (this->Links.at(j) == i)
				{
					fout << this->LinkDof.at(j) << ",";
				}
			}
		}
		fout << endl;
	}

	//fout << "SolProb"<<"\t"<<this->SolProb << endl;
	//fout << "UNPM\t"<<this->UNPM << endl;
	//fout << "ImpactValue"<<"\t"<<this->ImpactValue<< endl;
	//fout << "TotalCoat" << "\t" << this->TotalCost << endl;
	//fout <<"ExpectedImpact"<<"\t"<<this->ImpactValue* this->SolProb << "\t" << endl;
	return 1;
}

// copy constructor 
void CHROME::Copy(const CHROME& FromSource){
	this->ID = FromSource.ID;
	this->SolProb = FromSource.SolProb;
	this->UNPM = FromSource.UNPM;
	this->ImpactValue = FromSource.ImpactValue;
	this->TotalCost = FromSource.TotalCost;
	this->Fitness = FromSource.Fitness;
	this->Nodes.clear(); this->NodeDof.clear(); this->NodeDofProb.clear();
	this->Links.clear(); this->LinkDof.clear(); this->LinkDofProb.clear();

	if (FromSource.Nodes.size() != FromSource.NodeDof.size()
		|| FromSource.Nodes.size() != FromSource.NodeDofProb.size()
		|| FromSource.NodeDof.size() != FromSource.NodeDofProb.size())
	{
		TRACE("From source nodes size inconsistant \n");
	}
	for (unsigned int i = 0; i < FromSource.Nodes.size();i++)
	{
		this->Nodes.push_back(FromSource.Nodes.at(i));
		this->NodeDof.push_back(FromSource.NodeDof.at(i));
		this->NodeDofProb.push_back(FromSource.NodeDofProb.at(i));
	}

	if (FromSource.Links.size() != FromSource.LinkDof.size()
		|| FromSource.Links.size() != FromSource.LinkDofProb.size()
		|| FromSource.LinkDof.size() != FromSource.LinkDofProb.size())
	{
		TRACE("From source link size inconsistant \n");
	}
	for (unsigned int i = 0; i < FromSource.Links.size();i++)
	{
		this->Links.push_back(FromSource.Links.at(i));
		this->LinkDof.push_back(FromSource.LinkDof.at(i));
		this->LinkDofProb.push_back(FromSource.LinkDofProb.at(i));
	}
};

void CHROME::clear(){
	this->ID = 0;
	this->Nodes.clear();
	this->NodeDof.clear();
	this->NodeDofProb.clear();
	this->Links.clear();
	this->LinkDof.clear();
	this->LinkDofProb.clear();
	this->SolProb = InvaildFloat;
	this->TotalCost = InvaildFloat;
	this->UNPM = InvaildFloat;
	this->Fitness = InvaildFloat;
	this->ImpactValue = InvaildFloat;
}

bool CHROME::isSame(const CHROME &CompareSource)
{
	bool isSame = true;
	for (unsigned int i = 0; i < this->NodeDof.size();i++)
	{
		for (unsigned int j = 0; j < CompareSource.Nodes.size();j++)
		{
			if (this->Nodes.at(i)==CompareSource.Nodes.at(j))
			{
				if (!isEqual(this->NodeDof.at(i),CompareSource.NodeDof.at(j)))
				{
					isSame = false;
					return isSame;
				}
			}
		}
	}

	return isSame;
			
	
}

