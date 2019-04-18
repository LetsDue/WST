#include <iostream>
#include <windows.h>

#include "WST.h"
//Autor: Karol Br¹galski 235533
using namespace std;
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}


int main()
{

	//SA parametry
	double LiczbaIteracji[4] = { 10,20, 50, 100 };
	double DlugoscStopnia[4] = { 1000, 500, 200, 100 };
	double TempStart[4] = { 500, 1000, 2000, 5000 };
	double czas;
	WST *tes = NULL;
	char ster = 'x';
	while (ster != '0')
	{

		//system("cls");
		cout << endl;
		cout << "1. Wczytaj z pliku." << endl;
		cout << "2. Wyswietlanie aktualnych danych." << endl;
		cout << "3. Rozwiaz metada tabu" << endl;
		cout << "4. Wypisz wynik" << endl;
		cout << "5. Rozwiaz metada symulowanego wyzarzania" << endl;
		cout << "6. Test automatyczny metady symulowanego wyzarzania" << endl;
		cout << "0. Wyjdz" << endl;


		cin >> ster;
		string a = "data10.txt";
		switch (ster)
		{
		case '1':
			system("cls");
			std::cout << "Podaj nazwe pliku: ";
			std::cin >> a;

			if (tes != NULL) delete tes;
			try {
				tes = new WST(a);
			}
			catch (int param) 
			{
				cout << "Nie znaleziono pliku!" << endl;
			}
			break;
		case '2':
			system("cls");
			if (tes != NULL)
			{
				tes->WypiszDane();
			}
			else
			{
				std::cout << "Brak zaladowanych danych" << std::endl;
			}
			break;
		case '3':
			system("cls");
			if (tes != NULL)
			{
				

				double itTaby, tabuTime, maxit, master;
				std::cout << "Podaj liczbe iteracji";
				cin >> itTaby;
				std::cout << "Podaj dlugosc kadencji";
				cin >> tabuTime;
				std::cout << "Podaj maksymalna liczbe iteracji bez poprawy";
				cin >> maxit;
				std::cout << "Podaj dlugosc listy kandydatow";
				cin >> master;


				StartCounter();
				tes->SolveTabu(itTaby, tabuTime, maxit, master);

				
				czas = GetCounter();
				std::cout << "Czas operacji:" << czas << std::endl;
			}
			else 
			{
				std::cout << "Najpierw wczytaj plik."  << std::endl;
			}
			
			break;
		case '4':
			system("cls");
			if (tes != NULL)
			{
				tes->Print();
			}
			else
			{
				std::cout << "Najpierw wczytaj plik." << std::endl;
			}
			break;
		case '5':
			system("cls");
			if (tes != NULL)
			{
				double tempS,IT,LS;
				std::cout << "Podaj temperature startu";
				cin >> tempS;
				std::cout << "Podaj liczbe poziomow temperatury";
				cin >> IT;
				std::cout << "Podaj dlugosc pojedynczego poziomu";
				cin >> LS;

				StartCounter();
				tes->SimulatedAnnealing(tempS, IT, LS * 0.1, LS, pow((1 / tempS), 1/IT));
				czas = GetCounter();
				std::cout << "Czas operacji:" << czas << std::endl;
			}
			else
			{
				std::cout << "Najpierw wczytaj plik." << std::endl;
			}

			break;
		case '6':
			system("cls");


			if (tes != NULL)
			{
				for(int i=0;i<4;i++)
				{
					for (int j = 0; j < 4; j++)
					{
						for (int k = 0; k < 3; k++)
						{

							StartCounter();
							tes->SimulatedAnnealing(TempStart[i], LiczbaIteracji[j], DlugoscStopnia[j]*0.1, DlugoscStopnia[j], pow((1/ TempStart[i]),1/LiczbaIteracji[j]));
							czas = GetCounter();
							std::cout << "Temperatura startowa: " << TempStart[i] << ":Liczba iteracji: " << LiczbaIteracji[j] << ":Dlugosc stopnia: " << DlugoscStopnia[j] << ":Mnoznik: " << pow((1 / TempStart[i]), 1/LiczbaIteracji[j]) <<std::endl;
							std::cout << "Czas operacji:" << czas << std::endl;
						}
					}
				}

			}
			else
			{
				std::cout << "Najpierw wczytaj plik." << std::endl;
			}

			break;
		case '7':
			system("cls");
			if (tes != NULL)
			{
				for (int o = 1; o < 5; o++)
				{
					for (int i = 1; i < 5; i++)
					{
						for (int j = 1; j <3; j++)
						{
							for (int k = 0; k < 3; k++)
							{
								StartCounter();
								tes->SolveTabu(o*50, i * 10, j*20, 1);
								czas = GetCounter();
								std::cout << "Liczba iteracji:" << o * 50 << ":Dlugosc kadencji:" << i * 10 << ":Liczba iteracji bez poprawy:" << j * 20 << std::endl;
								std::cout << "Wynik:" << tes->resultpenalty << std::endl;
								std::cout << "Czas operacji:" << czas << std::endl;
							}
						}
					}
				}

			}
			else
			{
				std::cout << "Najpierw wczytaj plik." << std::endl;
			}
			break;
		case '0':
			exit(0);

			break;

		}

	}




	return 0;
}