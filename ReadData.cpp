#include "CommonHeaders.h"
#include <string>
using namespace std;

bool PrintModelParas(){

	ofstream fout;
	fout.open("..//OutPut//ModelPara.txt");

	fout << "NumNodes" << "," << NumNodes << endl;
	fout << "NumOD" << "," << NumOD << endl;
	fout << "NumLinks" << "," << NumLinks << endl;
	//fout << "StopCriteria" << "," << StopCriteria << endl;
	//fout << "MaxNumSol" << "," << MaxNumSolEval << endl;

	fout << "OneDimEsp" << "," << OneDimEsp << endl;
	fout << "UEmaxIter" << "," << UEmaxIter << endl;
	fout << "CsaNumPo" << "," << CsaNumPop << endl;
	fout << "CsaCloneBeta" << "," << CsaCloneBeta << endl;
	fout << "CsaRepRatio" << "," << CsaRepRatio << endl;

	fout << "MaxCsaIter" << "," << MaxCsaIter << endl;
	fout << "GANumPop" << "," << GANumPop << endl;
	fout << "GANumChild" << "," << GANumChild << endl;
	fout << "MaxGAIter" << "," << MaxGAIter << endl;
	fout << "GaMutationRate" << "," << GaMutationRate << endl;
	fout.close();



	return true;
}

bool ReadModelParas(){
	ifstream fin,fGA;
	if (NetworkIndex == 1)
	{
		fin.open("..//InPut//MediumNetwork//Para.txt");
		fGA.open("..//InPut//MediumNetwork//GAPara.txt");
	}
	else if (NetworkIndex == 2)
	{
		fin.open("..//InPut//Nguyue-Dupuisnetwork//Para.txt");
	}
	else if (NetworkIndex == 3)
	{
		fin.open("..//InPut//SiouxFallsNetwork//Para.txt");
		fGA.open("..//InPut//SiouxFallsNetwork//GAPara.txt");
	}
	else if (NetworkIndex == 4)
	{
		fin.open("..//InPut//ParadoxNet//Para.txt");
		fGA.open("..//InPut//ParadoxNet//GAPara.txt");
	}
	else
	{
		cout << "C++ Warning: Model Index is not specified" << endl;
		system("Pause");
	}

	string line;
	vector<string> fields;
	while (getline(fin, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
	}
	cout << "OneDimEsp = " << OneDimEsp << endl;
	fin.close();


	return PrintModelParas();
}

