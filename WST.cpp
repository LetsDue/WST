#include "WST.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>
#include <time.h>
//Autor: Karol Br¹galski 235533
WST::WST(std::string nazwa)  
{
	srand(time(NULL));
	std::cout << nazwa;
	std::string a;
	std::ifstream wejscie;
	wejscie.open(nazwa);
	if (wejscie.is_open() == false)
	{
		throw 1;
	}
	std::string firstline;
	wejscie >> firstline;
	wejscie >> numberoftasks;
	tasktime = new int[numberoftasks];
	penaltyfactor = new int[numberoftasks];
	expectedfinishtime = new int[numberoftasks];
	swappen = new int[numberoftasks*numberoftasks / 2];
	swapa = new int[numberoftasks*numberoftasks / 2];
	swapb = new int[numberoftasks*numberoftasks / 2];
	for (int i = 0; i < numberoftasks; i++)
	{
		wejscie >> tasktime[i];
		wejscie >> penaltyfactor[i];
		wejscie >> expectedfinishtime[i];
	}
	tabulist = new int*[numberoftasks];
	for (int i = 0; i <numberoftasks; i++)
	{
		tabulist[i] = new int[numberoftasks];
		for (int j = 0; j < numberoftasks; j++)
		{
			tabulist[i][j] = 0;
		}
	}
	result = new int[numberoftasks];
	starttime = new int[numberoftasks];
	tempresult = new int[numberoftasks];
	wejscie.close();

}
void WST::WypiszDane()
{
	std::cout << "NR. Czas wykonania; Wspolczynnik kary; Termin zakonczenia " << std::endl;
	for (int i = 0; i < numberoftasks; i++)
	{
		std::cout << std::setw(3) << i + 1 << " " << std::setw(10) << tasktime[i] << "     " << std::setw(10) << penaltyfactor[i] << "       " << std::setw(10) << expectedfinishtime[i] << std::endl;
	}
}


int WST::CountPenaltySum()
{
	int time = 0;
	for (int i = 0; i <numberoftasks; i++)
	{
		int it = tempresult[i];
		starttime[it] = time;
		time += tasktime[it];
	}
	int Pensum = 0;
	for (int i = 0; i < numberoftasks; i++)
	{
		int it = tempresult[i];
		int delay = starttime[it] + tasktime[it] - expectedfinishtime[it];
		if (delay>0)
		{
			Pensum += delay*penaltyfactor[it];
			
		}
	}
	return Pensum;
}
void WST::Restart() 
{
	std::random_shuffle(&tempresult[0], &tempresult[numberoftasks - 1]);
	temppenalty = CountPenaltySum();
}
void WST::SolveTabu(int itCount, int tabuTime, int maxIt, int masterNumber)
{
	int *masterTabPen = new int[masterNumber];
	int *masterTabA = new int[masterNumber];
	int *masterTabB = new int[masterNumber];
	int prog=0;
	int masterIt = 0;
	for (int i = 0; i < masterNumber; i++)
	{
		masterTabPen[i] = INT_MAX;
		masterTabA[i] = INT_MAX;
		masterTabB[i] = INT_MAX;
	}
	for (int i = 0; i < numberoftasks; i++)
	{
		tempresult[i] = i;
	}
	std::random_shuffle(&tempresult[0], &tempresult[numberoftasks - 1]);

	temppenalty = CountPenaltySum();
	resultpenalty = temppenalty;
	for (int i = 0; i < numberoftasks; i++)
	{
		result[i] = tempresult[i];
	}

	
	int itwithoutprogress = 0;
	for (int i = 0; i < itCount; i++)
	{
		//std::cout << i << " / " << itCount << std::endl;
		int iterator = 0;
		//tworzenie kolejki
		for (int j = 0; j < numberoftasks; j++)
		{
			for (int k = j + 1; k < numberoftasks; k++)
			{
				int swapbuffer;
				swapbuffer = tempresult[j];
				tempresult[j] = tempresult[k];
				tempresult[k] = swapbuffer;

				int temppenaltynew = CountPenaltySum();

				tempresult[k] = tempresult[j];
				tempresult[j] = swapbuffer;

				swappen[iterator] = temppenaltynew;
				swapa[iterator] = j;
				swapb[iterator] = k;

				iterator++;
			}
		}

		int minPen = swappen[0];
		int mina = 0;
		int minb = 0;
		
		//szukanie minimum i sprawdzanie czy nie jest na liscie tabu, lub czy jest lepsze od ostatecznego wyniku (wtedy gdy nie pozostalo nic w tabeli master)
		if ((prog < masterTabPen[masterIt]) ||( masterIt == masterNumber))
		{
			
			for (int o = 0; o < masterNumber; o++)
			{
				
				masterTabPen[o] = INT_MAX;
				masterTabA[o] =0;
				masterTabB[o] = 0;
				
				
			}
			for (int j = 0; j < (numberoftasks - 1)*(numberoftasks - 1) / 2; j++)
			{

				if (swappen[j] < minPen)
				{
					if ((tabulist[swapa[j]][swapb[j]] == 0) || (swappen[j] < resultpenalty))
					{
						minPen = swappen[j];
						mina = swapa[j];
						minb = swapb[j];
						

						for (int k = 0; k < masterNumber; k++)
						{
							if (minPen <= masterTabPen[k])
							{
								int buff = masterTabPen[k];
								masterTabPen[k] = minPen;
								minPen = buff;

								buff = masterTabA[k];
								masterTabA[k] = mina;
								mina = buff;

								buff = masterTabB[k];
								masterTabB[k] = minb;
								minb = buff;
								
							}
						}
						prog = masterTabPen[masterNumber - 1];
					}
				}
			}
			masterIt = 0;
		}
		else 
		{

			//ponowne przeliczenie opoznien dla zmian z tabeli master (uszeregowanie od konca, od najgorszego)
			
			for (int j = masterIt; j < masterNumber; j++)
			{
			
				if (masterTabA[j] == masterTabA[masterIt - 1]) masterTabA[j] = masterTabB[masterIt - 1];
				if (masterTabA[j] == masterTabB[masterIt - 1]) masterTabA[j] = masterTabA[masterIt - 1];
				if (masterTabB[j] == masterTabB[masterIt - 1]) masterTabB[j] = masterTabA[masterIt - 1];
				if (masterTabB[j] == masterTabA[masterIt - 1]) masterTabB[j] = masterTabB[masterIt - 1];
				
				int swapbuffer;
				swapbuffer = tempresult[masterTabA[j]];
				tempresult[masterTabA[j]] = tempresult[masterTabB[j]];
				tempresult[masterTabB[j]] = swapbuffer;

				masterTabPen[masterIt] = CountPenaltySum();

				tempresult[masterTabB[j]] = tempresult[masterTabA[j]];
				tempresult[masterTabA[j]] = swapbuffer;		
			}
			//posortowanie tabeli
			for (int o = masterIt-1; o< masterNumber; o++)
			{		
				for (int j = masterIt-1; j < masterNumber - 1; j++)
				{
					if (masterTabPen[j] > masterTabPen[j + 1])
					{
						int buff = masterTabPen[j];
						masterTabPen[j] = masterTabPen[j+1];
						masterTabPen[j + 1] = buff;

						buff = masterTabA[j];
						masterTabA[j] = masterTabA[j + 1];
						masterTabA[j + 1] = buff;

						buff = masterTabB[j];
						masterTabB[j] = masterTabB[j + 1];
						masterTabB[j + 1] = buff;
					}
				}
			}

		}
		minPen = masterTabPen[masterIt];
	//	masterTabPen[masterIt] = INT_MAX;
		mina = masterTabA[masterIt];
		minb = masterTabB[masterIt];
	//	std::cout << "A " << mina << " B " << minb << " PEN " << minPen << std::endl;
		masterIt++;
		//zmniejszenie kadencji na liscie tabu
		for (int j = 0; j < numberoftasks; j++)
		{
			for (int k = j + 1; k < numberoftasks; k++)
			{
				if (tabulist[j][k] != 0)
				{
					tabulist[j][k]--;
				}
			}
		}
	
		int swapbuffer = tempresult[mina];
		tempresult[mina] = tempresult[minb];
		tempresult[minb] = swapbuffer;
		tabulist[mina][minb] = tabuTime ;
		//sprawdzenie czy uzyskano lepszy wynik
		//temppenalty = CountPenaltySum();
		if (minPen < resultpenalty)
		{
			for (int o = 0; o < numberoftasks; o++)
			{
				result[o] = tempresult[o];
			}
			resultpenalty = minPen;
			if (resultpenalty == 0)return;
		}
		else
		{
			itwithoutprogress++;
		}
		if (itwithoutprogress == maxIt)
		{
			
			//std::cout <<std::endl<< "RESTARTTTTTTT:" << std::endl;
			masterIt = 0;
			//std::cout <<tempresult[0] <<std::endl;
			itwithoutprogress = 0;
			WST::Restart();
			//std::cout << tempresult[0] << std::endl;
		}
		//std::cout << " PENALTY :" << resultpenalty << std::endl;
	}
}
void WST::Print() 
{
	std::cout << "Wynik algorytmu tabu:" << std::endl;
	for (int i = 0; i < numberoftasks; i++)
	{
		std::cout << "Zadanie: " << result[i]<<std::endl;
	}
	std::cout << "Kara: " << resultpenalty;
}

void WST::SimulatedAnnealing(double Ts, int it,int is, int LS,double mult)
{
	
	int rp=0;
	int *bestResult = new int[numberoftasks];
	double T = Ts;
	
	for (int i = 0; i < numberoftasks; i++)
	{
		tempresult[i] = i;
	}
	std::random_shuffle(&tempresult[0], &tempresult[numberoftasks - 1]);
	temppenalty = CountPenaltySum();
	resultpenalty = temppenalty;
	for (int i = 0; i < numberoftasks; i++)
	{
		result[i] = tempresult[i];
	}

	
	int swapA, swapB;
	for (int i = 0; i < it; i++)
	{
		int suc=0;
		for (int j = 0; j < LS; j++)
		{
			swapA =  rand() % numberoftasks;
			do 
			{
				swapB =rand() % numberoftasks;
			} while (swapA == swapB);

			int swapbuffer= tempresult[swapA];
			tempresult[swapA] = tempresult[swapB];
			tempresult[swapB] = swapbuffer;

			temppenalty = CountPenaltySum();
			tempresult[swapB] = tempresult[swapA];
			tempresult[swapA] = swapbuffer;
			if (temppenalty < resultpenalty)
			{
				suc++;
				resultpenalty = temppenalty;
				for (int k = 0; k < numberoftasks; k++)
				{
					result[k] = tempresult[k];
				}
				rp = temppenalty;
				for (int k = 0; k < numberoftasks; k++)
				{
					bestResult[k] = tempresult[k];
				}
				swapbuffer = tempresult[swapA];
				tempresult[swapA] = tempresult[swapB];
				tempresult[swapB] = swapbuffer;
			}
			else
			{
				double r = ((double)rand() / (RAND_MAX));
				double param = (( resultpenalty- temppenalty) / (double)T);
				double kr = exp(param);

				//std::cout << "kr " << kr << " R: " << r << std::endl   << std::endl;
				if (r <kr)
				{
					swapbuffer = tempresult[swapA];
					tempresult[swapA] = tempresult[swapB];
					tempresult[swapB] = swapbuffer;					
					suc++;
					if (temppenalty < resultpenalty)
					{
						rp = temppenalty;
						for (int k = 0; k < numberoftasks; k++)
						{
							bestResult[k] = tempresult[k];
						}
					}
					
					resultpenalty = temppenalty;
					for (int k = 0; k < numberoftasks; k++)
					{
						result[k] = tempresult[k];
					}
				}
			}
			if (suc == is)
			{
				
				break;
				
			}
		}
		T *= mult;
		//std::cout <<"????"<< T << std::endl;
	}
	std::cout << "NAJLEPSZY:" << rp << std::endl;
}

WST::~WST()
{

	delete[] result;
	delete[] tempresult;
	for (int i = 0; i < numberoftasks; i++)
	{
		delete[] tabulist[i];
	}
	delete[] tabulist;
	delete[] tasktime;
	delete[] penaltyfactor;
	delete[] expectedfinishtime;
	delete[] starttime;
}
