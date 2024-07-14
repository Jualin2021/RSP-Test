
#include "CommonHeaders.h"
#include <assert.h>
#include <math.h>       /* pow */
using namespace std;


inline double GetBPRCost(double &flow,double &Ca,double &t0,
	double &AlphBpr,double &BetaBpr){
	
	//return (t0*(1.0f + AlphBpr*pow((flow / Ca), BetaBpr)));
	//return (t0*(1.0f + AlphBpr*(flow / Ca)*(flow / Ca)*(flow / Ca)*(flow / Ca)));// by defalut Bpr = 4;
	//return (t0*(1.0f + 0.15*(flow / Ca)*(flow / Ca)*(flow / Ca)*(flow / Ca)));// by defalut Bpr = 4;
	//return (t0*(1.0f + 0.15*std::pow((flow / Ca),4)));// by defalut Bpr = 4;
	return (t0*(1.0f + AlphBpr*std::pow((flow / Ca),BetaBpr)));// by defalut Bpr = 4;
};

double LinksDirectionDerivative(vector<LINK> &XLink, vector<double> &FlowDif, double &Alpha)
{
	assert(XLink.size() == NumLinks);
	vector<double> Flow(NumLinks,0.0);
	double LinkCostSum = 0.0f;;

	for (unsigned int i = 0; i < XLink.size();i++)
	{
		Flow.at(i) = XLink.at(i).Flow + Alpha*FlowDif.at(i);
	}

	for (unsigned int i = 0; i < XLink.size();i++)
	{
		//double GetBPRCost(double &flow, double &Ca, double &t0,
		//	double &AlphBpr, double &BetaBpr){
		LinkCostSum = LinkCostSum + GetBPRCost(Flow.at(i), XLink.at(i).CaRevise, XLink.at(i).T0, XLink.at(i).AlphaBpr, XLink.at(i).BetaBBpr)*FlowDif.at(i);
	}

	return LinkCostSum;
}


double LinksSDLineSearch(vector<LINK> &XLink, vector<LINK> &YLink)
{
	//cout << "start update alpha:line search" << endl;
	assert(XLink.size() == NumLinks);
	vector<double> FlowDif(NumLinks, 0.0);
	double Grade;
	double Alpha = 0.0f;
	double LastAlpha = 0.0f;
	double LeftAlpha, RightAlpha;
	int itercounter = 0;
	for (unsigned int i = 0; i < XLink.size(); i++)
	{
		FlowDif.at(i) = YLink.at(i).Flow - XLink.at(i).Flow;
		//cout << "df," << FlowDif.at(i) << endl;
	}

	
	Grade = LinksDirectionDerivative(XLink, FlowDif, Alpha);
	//cout << Grade <<"," << Alpha<<endl;
	if (Grade >= 0)
	{
		LastAlpha = 0.0;
		return LastAlpha;
	}
	Alpha = 1.0;
	Grade = LinksDirectionDerivative(XLink, FlowDif, Alpha); 
	//cout << Grade << "," << Alpha << endl;
	if (Grade <= 0)
	{
		LastAlpha = 1.0;
		return LastAlpha;
	}
	LeftAlpha = 0.0f; RightAlpha = 1.0f; Alpha = 0.5f;
Loop:
	itercounter++;
	Grade = LinksDirectionDerivative(XLink, FlowDif, Alpha);
	//cout << Grade << "," << Alpha << endl;
	if (Grade <= 0.0f)
		LeftAlpha = Alpha;
	else
		RightAlpha = Alpha;

	Alpha = 0.5f*(LeftAlpha + RightAlpha);

	if (std::abs(LeftAlpha - RightAlpha) > OneDimEsp)
	{
		//cout << abs(LeftAlpha - RightAlpha) << endl;
		goto Loop;
	}
	LastAlpha = 0.5f*(LeftAlpha + RightAlpha);
	//cout << "end update alpha:line search" << endl;

	return LastAlpha;

}
