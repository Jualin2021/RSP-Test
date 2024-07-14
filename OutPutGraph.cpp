#include "CommonHeaders.h"
#include <string>
using namespace std;

void OutPutGraph(GRAPH &Graph){
	std::ofstream  fout;
	fout.open("..//OutPut//LinkSummary.txt", ios::app);
	Graph.PrintLinks(fout);
	fout.close();
	fout.open("..//OutPut//ODsummary.txt", ios::app);
	Graph.PrintOD(fout);
	fout.close();
}

void OutputSummary(vector<CHROME> &BestSol,GRAPH &Graph,vector<double> &CpuTime){
	std::ofstream  foutLink,foutOD,fSummay,fsol;
	fSummay.open("..//OutPut//Summary.txt", ios::app);
	//double BaseUNPM;
	Graph.EvaluteGraph();
	//BaseUNPM = Graph.UNPM;
	
	foutLink.open("..//OutPut//BestSolLink.txt", ios::app);
	foutOD.open("..//OutPut//BestSolOD.txt", ios::app);

	fSummay << "UNPM" << ",";
	fSummay << "ImpactValue" << ",";
	fSummay << "Fitness" << ",";
	fSummay << "TotalCost" << ",";
	fSummay << "CPU time" << ",";
	fSummay << "SolProb" << ",";
	fSummay << "ExpectedImpact" << endl;
	//fSummay << endl;
	//fout << "SolProb"<<"\t"<<this->SolProb << endl;
	//fout <<"ExpectedImpact"<<"\t"<<this->ImpactValue* this->SolProb << "\t" << endl;

	for (unsigned int i = 0; i < BestSol.size();i++)
	{
		fSummay << BestSol.at(i).UNPM << ",";
		fSummay << BestSol.at(i).ImpactValue << ",";
		fSummay << BestSol.at(i).Fitness << ","; //expected impact
		fSummay << BestSol.at(i).TotalCost << ",";
		fSummay << CpuTime.at(i) << ",";
		fSummay << BestSol.at(i).SolProb << ",";
		fSummay << BestSol.at(i).SolProb*BestSol.at(i).ImpactValue << ",";

		/*******print link and od pairs**********************/
		BestSol.at(i).ReviseCap(Graph);
		Graph.EvaluteGraph();
		Graph.PrintLinks(foutLink);
		Graph.PrintOD(foutOD);
		BestSol.at(i).IniCap(Graph); // after revising the capacity and change it back
		/*****************************************/
		fSummay << endl;
	}
	

	//fSummay << "BaseUnpm" << "," << Graph.UNPM << endl;
	fsol.open("..//OutPut//PrintSols.txt", ios::app);
	for (size_t i = 0; i < BestSol.size(); i++)
	{
		BestSol.at(i).PrintSol(fsol);
	}
	foutLink.close();
	foutOD.close(); 
	fSummay.close(); 
	fsol.close();
}
