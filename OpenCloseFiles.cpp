// check 6Dec2022
#include "CommonHeaders.h"
using namespace std;
void LogMsg(const char *format, ...);
std::ofstream Log;
void OpenAndCleanFiles(){
	Log.open("..//OutPut//Log.txt", ios::trunc | ios::app);
	if (Log.is_open()) printf("Log File Not Open\n");
	ofstream  OutFile;
	OutFile.open("..//OutPut//Summary.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//ModelPara.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//Summary.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//PrintSols.txt", ios::trunc);
	OutFile << "Seed,Scenario,Link,St,Et" << endl;
	OutFile.close();
	OutFile.open("..//OutPut//PrintPeriod.txt", ios::trunc);
	OutFile << "Seed,Scenario,Period,Cost,UNPM" << endl;
	OutFile.close();
	OutFile.open("..//OutPut//LinkData.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//DemandData.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//GAConverge.txt", ios::trunc);
	OutFile.close();
	OutFile.open("..//OutPut//OperatorsMeasure.txt", ios::trunc);
	OutFile << "Seed,Id,Good,Bad,Sum,Gp,Bp,Prob,Score,Weight" << endl;
	OutFile.close();
	OutFile.open("..//OutPut/LinkEI.txt", ios::trunc);
	OutFile << "LinkId,EI" << endl;
	OutFile.close();
	OutFile.open("..//OutPut//PrintPatternScore.txt", ios::trunc);
	OutFile << "Seed,First,Second,Score,Prob" << endl;
	OutFile.close();

}
void CloseFiles(){
	Log.close();
}
