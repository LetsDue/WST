#pragma once
#include <string>
//Autor: Karol Br¹galski 235533
class WST
{
public:
	int numberoftasks;
	int *starttime;
	int *tasktime;
	int *penaltyfactor;
	int *expectedfinishtime;

	int *swappen;
	int *swapa;
	int *swapb;
	int *result;
	int *tempresult;
	int temppenalty;
	int resultpenalty;
	int **tabulist;

	int *resultSA;



	WST(std::string nazwa);
	void WypiszDane();
	int CountPenaltySum();
	void SolveTabu(int itCount, int tabuTime, int maxIt, int masterNumber);
	void Print();
	void Restart();
	void SimulatedAnnealing(double T,int it,int is,int LS,double mult);
	~WST();
};