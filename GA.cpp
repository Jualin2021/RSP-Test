#include "CommonHeaders.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iostream>
#include "RandomFuncs.h"
#include "GA.h"
#include <assert.h>     /* assert */
#include <string>
using namespace std;


bool ReadSeedVec(std::vector<int>& SeedVec,
	FILE* fin) {
	int SeedValue;
	SeedVec.clear();
	if (nullptr == fin) {
		perror("Read Seed File Fails \n");
		return false;
	}
	else
	{
		while (!feof(fin))
		{
			fscanf_s(fin, "%i",
				&SeedValue);
			if (SeedValue != EOF) {
				SeedVec.push_back(SeedValue);
			}
		}
	}

	ofstream fout;
	fout.open("..//OutPut//CheckSeed.txt");
	for (int i = 0; i < SeedVec.size(); i++)
	{
		fout << SeedVec.at(i) << endl;
	}

	fout.close();

	return true;
}


void GAAlgorithms::GAMain()
{
	ofstream ConvergeFile;
	ConvergeFile.open("..//OutPut//GAConverge.txt", ios::app);
	vector<double> CpuTimes;
	clock_t St, Et;
	for (int s = 0; s < SeedVecVal.size(); s++)
	{
		St = clock();
		GenRan.seed((unsigned)SeedVecVal.at(s));
		// start the process of one seed operation
		ConvergeMeasure.clear();
		GlobalBest.Fitness = std::numeric_limits<double>::max();
		GenerateIni();
		int NumPop = GANumPop;
		int Father, Mother;
		for (int i = 0; i < MaxGAIter; i++)
		{
#ifdef _DEBUG
			cout << "---------------------GA iter = " << i << "--------------" << endl;
#endif 
			for (int i = 0; i < GANumChild; i = i + 2)
			{

				GAselectParent(Father, Mother, NumPop);
				int BigBro = NumPop + i;
				int CuteSis = NumPop + i + 1;
				// step 1 crossover and generate offspring 
				GACrossOver(this->Chroms.at(Father), this->Chroms.at(Mother), this->Chroms.at(BigBro), this->Chroms.at(CuteSis));
				// step 2 mutation on the offspring
				GAMutation();
				// step 3 evaluate the offspring
				Sols.back().Evaluate(VofGraph, setOfFailureLinks);
			}

#ifdef _DEBUG
			cout << "---------------------GA iter" << i << " complete on scout phase" << endl;
#endif	
			ConvergeMeasure.push_back(GlobalBest.Fitness);
			if (isWriteConverge) ConvergeFile << s << "," << i << "," << fixed << setprecision(2) << ConvergeMeasure.back() << endl;
		}
		Et = clock();
		CpuTimes.push_back((double)((Et - St)/CLOCKS_PER_SEC)); // time unit is second
		PrintFinal(s); 

	}
	ofstream CpuTimeFile;
	CpuTimeFile.open("..//OutPut//CpuTime.txt", ios::trunc);
	CpuTimeFile << "Seed,Time" << endl;
	for (int s= 0;s< SeedVecVal.size();s++)
	{
		CpuTimeFile << s << "," << fixed << setprecision(2) <<CpuTimes[s]<< endl;
	}
	CpuTimeFile.close();

#ifdef _DEBUG
	cout << "*************************GA completes**************************" << endl;
	cout << "Print Global Best Sol" << endl;
	GlobalBest.print();
	cout << "Total Cost = " << GlobalBest.Fitness << endl;
	cout << "*************************Done**************************" << endl;
#endif // _DEBUG
}

void GAAlgorithms::GenerateIni()
{
	// improve the solutions
	for (int i = 0; i < GANumPop; i++)
	{
#ifdef _DEBUG
		cout << "-----------------------Generate Ini Sol= " << i << "-------------------" << endl;
#endif // _DEBUG
		Sols.push_back(SCHCLASS(i));
		Sols.back().GenerateIniSch(*(VofGraph[0]), setOfFailureLinks);
		Sols.back().AlignStartTime(VofGraph, setResourceCap);
#ifdef _DEBUG
		cout << "----------Print solution after solution alignment--------" << endl;
		Sols.back().print();
		cout << "----------End print solution after solution alignment--------" << endl;
#endif // _DEBUG
		
		Sols.back().Evaluate(VofGraph, setOfFailureLinks);
		if (Sols.back().Fitness < GlobalBest.Fitness)
		{
			GlobalBest = Sols.back();
		}
	}
}


void GAAlgorithms::GAselectParent(int& Father, int& Mother, const int NumPop)
{
	//pseudo-code//steps
	//step 1: Select the best solution from the population according to the fitness value.
	//step 2: Place the selected solution directly into the new generation population.
	//step 3: Select two solutions as parent individuals via tournament selection strategy 
}

void GAAlgorithms::GACrossOver(CHROME& Father, CHROME& Mother, CHROME& BigBro, CHROME& CuteSis)
{
	//pseudo-code//steps
	//step 1: Randomly Select two parent individuals P1 and P2.
	//step 2: Randomly locate the positions of two genes on the individual P1.
	//step 3: Generate offspring individuals using order crossover operator.
}

void GAAlgorithms::GAMutation()
{
	//pseudo-code//steps
	//step 1: Randomly select a mutation operator among the swap operator and the right shift operator.
	//step 2: Randomly locate the positions of two genes on the individual.
	//step 3: Generate new individuals using the selected mutation operator.
}

void GAAlgorithms::Ini()
{
	ReadData();
	for (int l = 0; l < setOfFailureLinks.size(); l++)
	{
		Pattern.push_back(PatternClass());
		Pattern.back().id = l;
		Pattern.back().LinkId = setOfFailureLinks.at(l);
		for (int k =0;k<setOfFailureLinks.size();k++)
		{
			Pattern.back().next.push_back(setOfFailureLinks.at(k));
		}
		Pattern.back().Prob.assign(setOfFailureLinks.size(), 0.0);
		Pattern.back().Score.assign(setOfFailureLinks.size(), 1.0);
	}
	for (auto& p : Pattern)
	{
		p.updateProb();
	}
}

void GAAlgorithms::ReadData()
{
	ifstream f_GApara, f_FailLinks;
	FILE* fseedin;
	if (NetworkIndex == 4)
	{
		string name;

		name = "..//InPut//ParadoxNet//FailureLinks-S";
		name += to_string(1) + ".txt";
		f_FailLinks.open(name);
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
		string line;
		vector<string> fields;
		std::cout << "Start to read failure S1 link data" << endl;
		while (getline(f_FailLinks, line))//////////////////////////////////////Faillinks
		{
			splitf(fields, line, ",");
			if (fields.size() == 3)
			{
				int linkId = stoi(fields[0]);
				setOfFailureLinks.push_back(linkId);
			}
			else if (fields.size() == 2)
			{
				if (fields[0]._Equal("res"))
				{
					setResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
				}
				else
					std::cout << "ERR: Read failure links warning" << endl;
			}
			else
				std::cout << "ERR: Read failure links warning" << endl;
		}
		f_FailLinks.close();
		std::cout << "compete read failure link data" << endl;

		for (int sceID = 0; sceID < 10;sceID++) 
		{
			//setOfFailureLinks.clear();/////////////////////101
			name = "..//InPut//ParadoxNet//FailureLinks-S";
			name += to_string(sceID) + ".txt";
			f_FailLinks.open(name);
			fopen_s(&fseedin, "..//Input//Seed.txt", "r");
			if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");

			std::cout << "Start to read failure S"<< sceID <<" link data" << endl;
			while (getline(f_FailLinks, line))//////////////////////////////////////Faillinks
			{
				splitf(fields, line, ",");
				if (fields.size() == 3)
				{
					int linkId = stoi(fields[0]);
					int length = stoi(fields[1]);
					int res = stoi(fields[2]);
					(* (this->VofGraph[sceID])).Links.at(linkId).RecoverTime = length;
					(*(this->VofGraph[sceID])).Links.at(linkId).RequiredRes = res;
				}
				else if (fields.size() == 2)
				{
					if (fields[0]._Equal("res"))
					{
						setResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
					}
					else
						std::cout << "ERR: Read failure links warning" << endl;
				}
				else
					std::cout << "ERR: Read failure links warning" << endl;

			}
			f_FailLinks.close();

			std::cout << "compete read failure link data" << endl;
		}
		f_GApara.open("..//InPut//ParadoxNet//GAPara.txt");
		
	}
	else if (NetworkIndex == 6)
	{
	string name;

	name = "..//InPut//Nguyue-Dupuisnetwork//FailureLinks-S";
	name += to_string(1) + ".txt";
	f_FailLinks.open(name);
	fopen_s(&fseedin, "..//Input//Seed.txt", "r");
	if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");
	string line;
	vector<string> fields;
	std::cout << "Start to read failure S1 link data" << endl;
	while (getline(f_FailLinks, line))
	{
		splitf(fields, line, ",");
		if (fields.size() == 3)
		{
			int linkId = stoi(fields[0]);
			setOfFailureLinks.push_back(linkId);
		}
		else if (fields.size() == 2)
		{
			if (fields[0]._Equal("res"))
			{
				setResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
			}
			else
				std::cout << "ERR: Read failure links warning" << endl;
		}
		else
			std::cout << "ERR: Read failure links warning" << endl;
	}
	f_FailLinks.close();
	std::cout << "compete read failure link data" << endl;

	for (int sceID = 0; sceID < 10; sceID++)
	{
		name = "..//InPut//Nguyue-Dupuisnetwork//FailureLinks-S";
		name += to_string(sceID) + ".txt";
		f_FailLinks.open(name);
		fopen_s(&fseedin, "..//Input//Seed.txt", "r");
		if (!ReadSeedVec(SeedVecVal, fseedin)) TRACE("Read Seed File Fails \n");

		std::cout << "Start to read failure S" << sceID << " link data" << endl;
		while (getline(f_FailLinks, line))
		{
			splitf(fields, line, ",");
			if (fields.size() == 3)
			{
				int linkId = stoi(fields[0]);
				int length = stoi(fields[1]);
				int res = stoi(fields[2]);
				(*(this->VofGraph[sceID])).Links.at(linkId).RecoverTime = length;
				(*(this->VofGraph[sceID])).Links.at(linkId).RequiredRes = res;
			}
			else if (fields.size() == 2)
			{
				if (fields[0]._Equal("res"))
				{
					setResourceCap.assign(MaxNumOfSchPeriod, stoi(fields[1]));
				}
				else
					std::cout << "ERR: Read failure links warning" << endl;
			}
			else
				std::cout << "ERR: Read failure links warning" << endl;

		}
		f_FailLinks.close();

		std::cout << "compete read failure link data" << endl;
	}
	f_GApara.open("..//InPut//Nguyue-Dupuisnetwork//GAPara.txt");

	}
	else
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
	}
	std::cout << "Complete open files" << endl;
	std::cout << "Start to read para" << endl;
	string line;
	vector<string> fields;
	while (getline(f_GApara, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "GANumPop")	GANumPop = stoi(fields[1]);
		if (fields[0] == "GANumChild")	GANumChild = stoi(fields[1]);
		if (fields[0] == "MaxGAIter")	MaxGAIter = stoi(fields[1]);
		if (fields[0] == "GaMutationRate") GaMutationRate = stof(fields[1]);
	}
	f_GApara.close();
	std::cout << "complete read GA para" << endl;
	

	/// <summary>
	/// print and check the model parameters input
	/// </summary>
	ofstream fout;
	fout.open("..//OutPut//InputPara.txt");
	fout << "NumNodes" << "," << NumNodes << endl;
	fout << "NumOD" << "," << NumOD << endl;
	fout << "NumLinks" << "," << NumLinks << endl;
	fout << "OneDimEsp" << "," << OneDimEsp << endl;
	fout << "UEmaxIter" << "," << UEmaxIter << endl;
	fout << "MaxGAIter" << "," << MaxGAIter << endl;
	fout.close();
}


void GAAlgorithms::PrintFinal(int sd)
{
	ofstream sf;
	sf.open("..//OutPut//PrintSols.txt", ios::app);
	for (int s = 0; s < 10; s++) ///////////////////////////////1017
	{
	  for (size_t t = 0; t < GlobalBest.Links.size(); t++)
	  {
		sf << sd << "," << s << "," << GlobalBest.Links.at(t)->ID << "," << GlobalBest.StartTime[s].at(t) << "," <<
			GlobalBest.EndTime[s].at(t) << endl;
	  }
	}
	sf.close();

	sf.open("..//OutPut//PrintPeriod.txt", ios::app);
	for (int s = 0; s < 10; s++) ///////////////////////////////1017
	{ 
	  for (size_t t = 0; t < GlobalBest.TravelTime[s].size(); t++)
	  {
		sf << sd << "," << s << "," << t << "," << GlobalBest.TravelTime[s].at(t) << "," << GlobalBest.UNPM[s].at(t) << endl;
	  }
	}
	sf.close();
}




