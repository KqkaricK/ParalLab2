#include <iostream>
#include "omp.h"
#include <cstdlib>
int THREADS = 1;
int DIM = 1000;
bool VIVOD = false;
using namespace std;

void Gauss_parallel(int n, int m){

	srand(4541);

	float **matrix = new float *[n];
	for (int i = 0; i < n; i++)
		matrix[i] = new float[m];

	for (int i = 0; i < n; i++)

	for (int j = 0; j < m; j++)
	{
		matrix[i][j] = rand();
	}
	// Выводим на экран
	if (VIVOD == true)
	{
		cout << "Матрица: " << endl;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
		cout << endl;
	}

	float  tmp;
	float* xx = new float [DIM];
	int i, j, k;
	double dt, timein, timeout;

	// Метод Гаусса, прямой ход
	timein = omp_get_wtime();
	omp_set_num_threads(THREADS);
	for (i = 0; i < n; i++)
	{
		tmp = matrix[i][i];
		for (j = n; j >= i; j--)
			matrix[i][j] /= tmp;

#pragma omp parallel for private (j, k, tmp)
		for (j = i + 1; j < n; j++)
		{
			tmp = matrix[j][i];
			for (k = n; k >= i; k--)
				matrix[j][k] -= tmp * matrix[i][k];
		}
	}

	//Обратный ход
	xx[n - 1] = matrix[n - 1][n];
	for (i = n - 2; i >= 0; i--)
	{
		xx[i] = matrix[i][n];
#pragma omp for private (j)
		for (j = i + 1; j < n; j++)
			xx[i] -= matrix[i][j] * xx[j];
	}
	timeout = omp_get_wtime();
	dt = timeout - timein;
	cout << "Время: " << dt << " секунд" << endl;

	if (VIVOD == true)
	{
		for (int i = 0; i < n; i++)
		{
			cout << floor(xx[i]) << " ";
			cout << endl;
		}
	}
	
	delete[] matrix;
	delete[] xx;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "Потоков: ";
	cin >> THREADS;
	cout << "Матрица: ";
	cin >> DIM;
	cout << "Выводим?: ";
	cin >> VIVOD;
	Gauss_parallel(DIM, DIM);
	system("pause");
}