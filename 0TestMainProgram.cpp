#include "CommonHeaders.h"
#include "DefGloVar.h"
#include "RestoreSchClass.h"
#include "GA.h"
#include "Classes.h"

using namespace std;

void OpenAndCleanFiles();
bool ReadModelParas();
void CloseFiles();
void ReadModelPara()
{
	ifstream fin_Model,fin_Net;
	string line;
	vector<string> fields;
	fin_Model.open("..//InPut//ModelPara.txt");
	while (getline(fin_Model, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "OneDimEsp")	OneDimEsp = stof(fields[1]);
		if (fields[0] == "UEeps")	UEeps = stof(fields[1]);
		if (fields[0] == "UEmaxIter")	UEmaxIter = stoi(fields[1]);
		if (fields[0] == "NetworkIndex")	NetworkIndex = stoi(fields[1]);
		if (fields[0] == "SingleOpIndex") TestSingleOpIndex = stoi(fields[1]);
		if (fields[0] == "isTestSingleOp")
		{
			if (fields[1]._Equal("True"))
			{
				isTestSingleOperator = true;
			}
			else if (fields[1]._Equal("False"))
			{
				isTestSingleOperator = false;
			}
			else
			{
				cout << "C++: Warning:isTestSingleOperator is not read properly" << endl;
			}
		}
		if (fields[0] == "UseMyOwn")
		{
			if (fields[1] == "True")
			{
				UseMyOwnAlgo = true;
			}
			else if (fields[1] == "False")
			{
				UseMyOwnAlgo = false;
			}
		}
	}
	fin_Model.close();

	if (NetworkIndex == 1)
	{
		std::cout << "Model Index is not specified" << endl; 
		system("Pause"); 
		fin_Net.open("..//InPut//MediumNetwork//NetPara.txt"); 
	}
	else if (NetworkIndex == 2)
	{
		cout << "Model Index is not specified" << endl;
		system("Pause");
	}
	else if (NetworkIndex == 3)
	{
		fin_Net.open("..//InPut//SiouxFallsNetwork//NetPara.txt");
	}
	else if (NetworkIndex == 4)
	{
		fin_Net.open("..//InPut//ParadoxNet//NetPara.txt");
	}
	else if (NetworkIndex == 5)
	{
		fin_Net.open("..//InPut//WangNetwork//NetPara.txt");
	}
	else if (NetworkIndex == 6)
	{
		fin_Net.open("..//InPut//Nguyue-Dupuisnetwork//NetPara.txt");
	}
	else
	{
		std::cout << "Model Index is not specified" << endl;
		system("Pause");
	}

	while (getline(fin_Net, line))
	{
		splitf(fields, line, ",");
		if (fields.size() != 2) continue;
		if (fields[0] == "NumNodes")	NumNodes = stoi(fields[1]);
		if (fields[0] == "NumOD")	NumOD = stoi(fields[1]);
		if (fields[0] == "NumLinks")	NumLinks = stoi(fields[1]);
	}
	fin_Net.close();
}


int main(int argc, char* argv[])
{
	//TOOD: Change NetworkIndex to NetIndex
	/* remarks on the network model index
	* 
	  NetworkIndex = 4; //Paradox network
	*/
	cout << "Remarks: Maximum Restore Periods is " << MaxNumOfSchPeriod << endl;
	isWriteConverge = true;
	OpenAndCleanFiles();
	ReadModelPara();
#ifdef _DEBUG
	cout << "c++:Read Model Para Complete" << endl;
#endif // _DEBUG
	GAAlgorithms MainAlgo;
	std::cout << "# start to read graph data" << endl;
	for(int i=0;i<10;i++)
	{
		GRAPH* gr = new GRAPH();
		(* gr).ReadDataMain();
		MainAlgo.VofGraph.push_back(gr);
	}
	std::cout << "# complete read graph data" << endl;
	std::cout << "# start to read algorithm data" << endl;
	MainAlgo.Ini(); //contains all the ini procedure for the algorithm
	std::cout << "# complete read graph data" << endl;
	if (!UseMyOwnAlgo)
	{
		vector<int> InputVec;
		if (argc > 1)
		{
			for (int i = 1; i < argc; i++)
			{
				InputVec.push_back(MainAlgo.setOfFailureLinks.at(atoi(argv[i])));
			}
		}
		if (UseMyOwnAlgo)
		{
			std::cout << "c++: Warning: Use My Own Algorithm is not well defined" << endl;
		}
		MainAlgo.ReadSolAndEvaluate(InputVec);
	}
	else
	{
		Zero = 1.0e-6f;
		MainAlgo.GAMain();
	}
	CloseFiles();
	return 0;
}