
#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;
bool UpdateLable(const double CurrentCost, const double LinkCost, const double OldNextNodeCost, double &NewCost)
{

	NewCost = InvaildFloat;
	//printf("ModeType = %d, link Type = %d \n", ModeType, Links.at(LinkIndex).Type);
	NewCost = CurrentCost + LinkCost;
#ifdef __DEBUG__  
	if (NewCost < 0.0)
		DEBUG("Label New Cos is less 0, linkid = %d, CurrentCost= %f", LinkIndex, CurrentCost);
#endif
	if (NewCost <= OldNextNodeCost) return true;
	else return false;
}

int GRAPH::SP(const int Orig, std::vector<double> &Lable)
{
//int *PredLink, double *Labl
try
	{
		int  WasInQueue = -7;
		int now, NewNode, Return2Q_Count = 0;
		double NewCost = InvaildFloat;
		int QueueFirst, QueueLast;
		Lable.clear();
		std::vector<int> QueueNext;
		
		QueueNext.reserve(NumNodes);
	
		for (int node = 0; node < NumNodes; node++){
			QueueNext.push_back(InvaildInt);
			//Lable[node] = LargeNum;
			Lable.push_back(LargeNum);
			//PredLink[node] = InvaildInt;
			this->MinPathPredLink[Orig][node] = InvaildInt;
		}
		now = Orig;
		QueueNext.at(now) = WasInQueue;
		//PredLink[now] = InvaildInt;
		this->MinPathPredLink[Orig][now] = InvaildInt;

		Lable[now] = 0.0;
		QueueFirst = QueueLast = InvaildInt;
		while ((now != InvaildInt) && (now != WasInQueue)) {
			for (auto k =this->Nodes.at(now).OutLinks.begin();
				k != this->Nodes.at(now).OutLinks.end(); k++) {
		

				// in case the capacity is zero
				if ((*k)->CaRevise <Zero) continue;
				NewNode = (*k)->Head;
				//printf("current Cost= %f \n", Lable[now]);
				if (UpdateLable(Lable[now], (*k)->Cost, Lable[NewNode], NewCost))
				{
					//cout << "Lable[now]= " << Lable[now] << ",(*k)->Cost =" << (*k)->Cost << ",NewCost =" << NewCost << endl;
				//if (UpdateLable(Lable[now], thiLinks,  *k, Lable[NewNode], NewCost)){
					//if (Lable[NewNode] > NewCost) {
					//printf("new node is = %d, cost = %f \n", NewNode,NewCost);
					Lable[NewNode] = NewCost;
					//if (Lable[NewNode] > 1000.0f&&isPrint)
					//{
					//	DEBUG("Cost is %f, node is %i, PLink is %i", Lable[NewNode], NewNode, *k);
					//}
					//PredLink[NewNode] = *k;
					this->MinPathPredLink[Orig][NewNode] = (*k)->ID;

					if (QueueNext.at(NewNode) == WasInQueue) {
						QueueNext.at(NewNode) = QueueFirst;
						QueueFirst = NewNode;
						if (QueueLast == InvaildInt)
							QueueLast = NewNode;
						Return2Q_Count++;
					}
					else if (QueueNext.at(NewNode) == InvaildInt && NewNode != QueueLast) {
						if (QueueLast != InvaildInt) { 					/*Usually*/
							QueueNext.at(QueueLast) = NewNode;
							QueueLast = NewNode;
						}
						else {			  /* If the queue is empty, initialize it. */
							QueueFirst = QueueLast = NewNode;
							QueueNext.at(QueueLast) = InvaildInt;
						}
					}
					/* If the new node is in the queue, just leave it there. (Do nothing) */
				}//if (Lable[NewNode] > NewCost) {
			} // for incoming link cycle

			/* Get the first node out of the queue, and use it as the current node. */
			now = QueueFirst;
			if ((now == InvaildInt) || (now == WasInQueue))	break;

			QueueFirst = QueueNext[now];
			QueueNext.at(now) = WasInQueue;
			if (QueueLast == now) QueueLast = InvaildInt;
		}

		return 1;
	}//try
	catch (exception& e)
	{
		printf("MinPath: %s", e.what());
		TRACE("MinPath: %s", e.what());
		return 0;
	}

}
