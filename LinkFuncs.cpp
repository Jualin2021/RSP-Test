#include "CommonHeaders.h"
#include <assert.h>
#include <algorithm>    // std::max
#include <math.h>       /* pow */
using namespace std;

double LINK::CleanLinkFlow(){
	return 0.0f;
}
double LINK::IniCost(){
	return this->T0;
}
double LINK::BPRCost(){
	//assert(this->CaRevise > Zero);
	//return (this->T0*(1.0f + (this->AlphaBpr)*std::pow((this->Flow /std::max(this->CaRevise, Zero)), this->BetaBBpr)));
	//return (this->T0*(1.0f + 0.15*std::pow((this->Flow /std::max(this->CaRevise, Zero)), 4.0)));
	//if (this->ID == 0)
	//{
	//	cout << "linkid = 0" << ",";
	//	cout << "flow = " << Flow << ",";
	//	cout << "AlphaBpr = " << AlphaBpr << ",";
	//	cout << "BetaBBpr = " << BetaBBpr << ",";
	//	cout << "CaRevise = " << CaRevise << ",";
	//	cout << "Cost = " << (this->T0 * (1.0f + AlphaBpr * std::pow((this->Flow / std::max(this->CaRevise, Zero)), BetaBBpr))) << endl;
	//}
	double BPRvalue = this->T0 * (1.0f + AlphaBpr * std::pow((this->Flow / std::max(this->CaRevise, Zero)), BetaBBpr));
	if (this->CaRevise < 0.01)
	{
		return 100 + BPRvalue;
	}
	else
	{
		return BPRvalue;
	}
	//return (this->T0*(1.0f + AlphaBpr*std::pow((this->Flow /std::max(this->CaRevise, Zero)),BetaBBpr)));
};

LINK::LINK(const LINK& OtherLink){
	this->ID = OtherLink.ID;
	this->Tail = OtherLink.Tail;
	this->Head = OtherLink.Head;
	this->Flow = OtherLink.Flow;
	this->T0 = OtherLink.T0;
	this->CaRevise = OtherLink.CaRevise;
	this->CaInput = OtherLink.CaInput;
	this->Cost = OtherLink.Cost;
	this->RecoverTime = OtherLink.RecoverTime;
	this->RequiredRes = OtherLink.RequiredRes;
	this->EI = 0.0;
	/*this->AlphaBpr = 0.15;
	this->BetaBBpr = 4;*/
	this->AlphaBpr = OtherLink.AlphaBpr;
	this->BetaBBpr = OtherLink.BetaBBpr;

}

LINK::LINK(){
	RecoverTime = 2; // take two period
	RequiredRes = 1; // only occupy on resource unit
	ID = Tail = Head = InvaildInt;
	Flow = InvaildFloat;
	Cost = InvaildFloat;
	CaRevise = InvaildFloat;
	CaInput = InvaildFloat;
	T0 = InvaildFloat;
	EI = 0;
}
LINK::~LINK(){
	//do sth
}

void LINK::IniCap(){
	this->CaRevise = this->CaInput;
}



