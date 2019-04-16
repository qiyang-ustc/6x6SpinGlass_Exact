#include <iostream>
#include <Eigen/Dense>
#include "gray.h"

int N = 6;
Eigen::VectorXi block(36);
Eigen::MatrixXi neighbour(36, 4);
Eigen::MatrixXi coupling(36, 4);

long long Energy_Spectrum[150];
long long total_configuration = 1024 * 1024;


void set_model()
{
	total_configuration = total_configuration * 1024 * 64;
	//total_configuration = 2^36
	int i;
	for (i = 0;i < 36;i++)
	{
		block(i) = -1;
	}
	for (i = 0;i < 150;i++)
	{
		Energy_Spectrum[i] = 0;
	}

	int j;
	for (i = 0;i < N;i++)
	{
		for (j = 0;j < N;j++)
		{
			neighbour(i*N + j, 2) = ((i + N - 1) % N)*N + j;
			neighbour(i*N + j, 1) = ((i + N + 1) % N)*N + j;
			neighbour(i*N + j, 0) = i*N + (j + N - 1) % N;
			neighbour(i*N + j, 3) = i*N + (j + N + 1) % N;
		}
	}
	//std::cout << neighbour;
	coupling << 0, -1, 0, -1,
		-1, -1, 0, +1,
		+1, +1, 0, -1,
		-1, -1, 0, +1,
		+1, +1, 0, +1,
		+1, -1, 0, 0,
		0, +1, -1, +1,
		+1, +1, -1, -1,
		-1, +1, +1, -1,
		-1, -1, -1, +1,
		+1, +1, +1, +1,
		+1, -1, -1, 0,
		0, +1, +1, -1,
		-1, -1, +1, -1,
		-1, +1, +1, -1,
		-1, +1, -1, -1,
		-1, +1, +1, +1,
		+1, -1, -1, 0,
		0, -1, +1, -1,
		-1, -1, -1, +1,
		+1, +1, +1, +1,
		+1, -1, +1, +1,
		+1, -1, +1, +1,
		+1, -1, -1, 0,
		0, +1, -1, +1,
		+1, -1, -1, +1,
		+1, -1, +1, +1,
		+1, -1, -1, -1,
		-1, -1, -1, +1,
		+1, +1, -1, 0,
		0, 0, +1, +1,
		+1, 0, -1, -1,
		-1, 0, -1, -1,
		-1, 0, -1, -1,
		-1, 0, -1, +1,
		+1, 0, +1, 0;
	/*for (int i = 0;i < 36;i++)
		for (int j = 0;j < 4;j++)
			coupling(i, j) = -1;*/
}

int turn_spin_cost(int n)
{
	int s = block(n);
	int dE;
	block(n) = -block(n);
	
	dE = -2 * s * (coupling(n, 0)*block(neighbour(n, 0)) +
		coupling(n, 1)*block(neighbour(n, 1)) +
		coupling(n, 2)*block(neighbour(n, 2)) +
		coupling(n, 3)*block(neighbour(n, 3)));
	//std::cout << dE << std::endl;
	return dE;
}

int cal_energy()
{
	int sum = 0;
	for (int n = 0;n < N*N;n++)
	{
		sum += block(n) * (coupling(n, 0)*block(neighbour(n, 0)) +
			coupling(n, 1)*block(neighbour(n, 1)) +
			coupling(n, 2)*block(neighbour(n, 2)) +
			coupling(n, 3)*block(neighbour(n, 3)));
	}
	return sum / 2;
}
int main()
{
	set_model();
	grayCodeGenerator counter(N*N-1);
	//std::cout <<neighbour;
	//
	//grayCodeGenerator test(35);
	//int test_num=-1;
	//int iternum = 0;
	//for (int t = 0;;t++)
	//{
	//	test_num = test.next_grayCode();
	//	if (test_num == -1)break;
	//	iternum++;
	//	//std::cout << test_num<<std::endl;
	//}
	//std::cout << "INTERNUMBER=" << iternum;
	//
	int dE;
	int k = 0;
	int Energy = cal_energy();
	
	while(1)
	{
		Energy_Spectrum[Energy+72]++;	
		k = counter.next_grayCode()-1;
		if (k == -2)break;
		dE = turn_spin_cost(k);
		Energy = Energy + dE;
	}

	FILE *fp = NULL;
	fopen_s(&fp, "Spectrum.dat", "a");
	fprintf(fp, "\n\n[");
	for(int i=0;i<150;i++)
	{	
		if (Energy_Spectrum[i] == 0)
		{
			continue;
		}
		fprintf(fp, "[%d,%lld],\n", i-72, Energy_Spectrum[i]);
	}
	fclose(fp);
	system("pause");
}