#include <stdio.h>
#include <iostream>
#include <time.h>

template<unsigned int N>
void BruteForce(int (&Out)[N][N], int (&A)[N][N], int (&B)[N][N])
{
    for (int i = 0; i < N ; i++)
    {
        for (int j = 0; j < N ; j++)
        {
            Out[i][j] = 0;
            for (int k = 0; k < N ; k++)
            {
                Out[i][j] = Out[i][j] + A[i][k]*B[k][j];
            }
        }
    }
}

template<unsigned int N>
void Sum(int (&Out)[N][N], int (&A)[N][N], int (&B)[N][N])
{
	for (int i = 0; i < N ; i++)
	{
		for (int j = 0; j < N ; j++)
		{
			Out[i][j] = A[i][j] + B[i][j];
		}
	}
}

template<unsigned int N>
void Difference(int (&Out)[N][N], int (&A)[N][N], int (&B)[N][N])
{
	for (int i = 0; i < N ; i++)
	{
		for (int j = 0; j < N ; j++)
		{
			Out[i][j] = A[i][j] - B[i][j];
		}
	}
}

template<unsigned int N>
void Strassen(int (&Out)[N][N], int (&A)[N][N], int (&B)[N][N])
{
	int TempLHS[N/2][N/2];
	int TempRHS[N/2][N/2];
	int Temp[N/2][N/2];

	//////////////////////////////////////////////////////////////////////////
	//Create Working Variables
	//////////////////////////////////////////////////////////////////////////

	int A00[N/2][N/2];
	int B00[N/2][N/2];
	for (int i = 0; i < N/2 ; i++)
	{
		for (int j = 0; j < N/2 ; j++)
		{
			A00[i][j] = A[i][j];
			B00[i][j] = B[i][j];
		}
	}

	int A01[N/2][N/2];
	int B01[N/2][N/2];
	for (int i = 0; i < N/2 ; i++)
	{
		for (int j = N/2; j < N ; j++)
		{
			A01[i][j-N/2] = A[i][j];
			B01[i][j-N/2] = B[i][j];
		}
	}


	int A10[N/2][N/2];
	int B10[N/2][N/2];
	for (int i = N/2; i < N ; i++)
	{
		for (int j = 0; j < N/2 ; j++)
		{
			A10[i-N/2][j] = A[i][j];
			B10[i-N/2][j] = B[i][j];
		}
	}

	int A11[N/2][N/2];
	int B11[N/2][N/2];
	for (int i = N/2; i < N ; i++)
	{
		for (int j = N/2; j < N ; j++)
		{
			A11[i-N/2][j-N/2] = A[i][j];
			B11[i-N/2][j-N/2] = B[i][j];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//Calculate intermediate Values
	//////////////////////////////////////////////////////////////////////////

	int M1[N/2][N/2];
	int M2[N/2][N/2];
	int M3[N/2][N/2];
	int M4[N/2][N/2];
	int M5[N/2][N/2];
	int M6[N/2][N/2];
	int M7[N/2][N/2];

	//M1
	Sum(TempLHS,A00,A11);
	Sum(TempRHS,B00,B11);
	Strassen(M1,TempLHS,TempRHS);

	//M2
	Sum(TempLHS,A10,A11);
	Strassen(M2,TempLHS,B00);

	//M3
	Difference(TempRHS,B01,B11);
	Strassen(M3,A00,TempRHS);

	//M4
	Difference(TempRHS,B10,B00);
	Strassen(M4,A11,TempRHS);

	//M5
	Sum(TempLHS,A00,A01);
	Strassen(M5,TempLHS,B11);

	//M6
	Difference(TempLHS,A10,A00);
	Sum(TempRHS,B00,B01);
	Strassen(M6,TempLHS,TempRHS);

	//M7
	Difference(TempLHS,A01,A11);
	Sum(TempRHS,B10,B11);
	Strassen(M7,TempLHS,TempRHS);
	
	//////////////////////////////////////////////////////////////////////////
	// Merge Result
	//////////////////////////////////////////////////////////////////////////

	//C00
	Sum(TempLHS,M1,M4);
	Difference(TempRHS,M7,M5);
	Sum(Temp,TempLHS,TempRHS);
	for (int i = 0; i < N/2 ; i++)
	{
		for (int j = 0; j < N/2 ; j++)
		{
			Out[i][j] = Temp[i][j];
		}
	}

	//C01
	Sum(Temp,M3,M5);
	for (int i = 0; i < N/2 ; i++)
	{
		for (int j = N/2; j < N ; j++)
		{
			Out[i][j] = Temp[i][j-N/2];
		}
	}

	//C10
	Sum(Temp,M2,M4);
	for (int i = N/2; i < N ; i++)
	{
		for (int j = 0; j < N/2 ; j++)
		{
			Out[i][j] = Temp[i-N/2][j];
		}
	}

	//C11
	Sum(TempLHS,M1,M3);
	Difference(TempRHS,M6,M2);
	Sum(Temp,TempLHS,TempRHS);
	for (int i = N/2; i < N ; i++)
	{
		for (int j = N/2; j < N ; j++)
		{
			Out[i][j] = Temp[i-N/2][j-N/2];
		}
	}
	return;
}

template<>
void Strassen<2>(int (&Out)[2][2], int (&A)[2][2], int (&B)[2][2])
{
    int m1 = (A[0][0]+A[1][1])*(B[0][0]+B[1][1]);
    int m2 = (A[1][0]+A[1][1])*(B[0][0]);
    int m3 = A[0][0]*(B[0][1]-B[1][1]);
    int m4 = A[1][1]*(B[1][0]-B[0][0]);
    int m5 = (A[0][0]+A[0][1])*B[1][1];
    int m6 = (A[1][0]-A[0][0])*(B[0][0]+B[0][1]);
    int m7 = (A[0][1]-A[1][1])*(B[1][0]+B[1][1]);

    Out[0][0] = m1 + m4 - m5 + m7;
    Out[1][0] = m2 + m4;
    Out[0][1] = m3 + m5;
    Out[1][1] = m1 + m3 - m2 + m6;

    return;
}


template<>
void Strassen<1>(int (&Out)[1][1], int (&A)[1][1], int (&B)[1][1])
{
	Out[0][0] = A[0][0]*B[0][0];
}

int main()
{
	int hold = 0;
	double msec=0.0;
	clock_t start, diff;

	//read Data6.txt
	const int num = 128;
	printf("Read Data11.txt Test\n");
	int x[num][num], y[num][num];
	FILE *in11file;
	in11file = fopen("../Data11.txt","r");
	fscanf(in11file,"%d",&hold);
	//first array
	for (int i = 0; i < num ; i++)
	{
		for (int j = 0; j < num ; j++)
		{
			fscanf(in11file,"%d", &x[i][j]);
		}
	}
	//second array
	for (int i = 0; i < num ; i++)
	{
		for (int j = 0; j < num ; j++)
		{
			fscanf(in11file,"%d", &y[i][j]);
		}
	}
	fclose(in11file);

	const int pow_of_2 = 128;
	int a[pow_of_2][pow_of_2];
	int b[pow_of_2][pow_of_2];

// 	std::cout << "\n\nFirst Array:\n\n";
// 	//first array
// 	for (int i = 0; i < pow_of_2 ; i++)
// 	{
// 		for (int j = 0; j < pow_of_2 ; j++)
// 		{
// 			a[i][j] = x[i][j];
// 			std::cout << a[i][j] << " ";
// 		}
// 		std::cout << "\n";
// 	}
	
// 	std::cout << "\n\nSecond Array:\n\n";
// 	//second array
// 	for (int i = 0; i < pow_of_2 ; i++)
// 	{
// 		for (int j = 0; j < pow_of_2 ; j++)
// 		{
// 			b[i][j] = y[i][j];
// 			std::cout << b[i][j] << " ";
// 		}
// 		std::cout << "\n";
// 	}

    std::cout << "\nArray Sizes: [" << pow_of_2 << "][" << pow_of_2 << "]:\n\n";
	int StrassenOut[pow_of_2][pow_of_2];

    //////////////////////////////////////////////////////////////////////////
	start = clock();
    Strassen(StrassenOut,a,b);
	diff = clock() - start;
    //////////////////////////////////////////////////////////////////////////

	std::cout << "\n\nStrassen Array:\n\n";
// 	for (int i = 0; i < pow_of_2 ; i++)
// 	{
// 		for (int j = 0; j < pow_of_2 ; j++)
// 		{
// 			std::cout << StrassenOut[i][j] << " ";
// 		}
// 		std::cout << "\n";
// 	}
	msec = ((float)diff)/CLOCKS_PER_SEC;
	printf("TIME: %d, %f\n",diff,msec);
	
    //////////////////////////////////////////////////////////////////////////
	start = clock();	
    BruteForce(StrassenOut,a,b);
    diff = clock() - start;
    //////////////////////////////////////////////////////////////////////////

	std::cout << "\n\nBrute force Array:\n\n";
//     for (int i = 0; i < pow_of_2 ; i++)
//     {
//         for (int j = 0; j < pow_of_2 ; j++)
//         {
//             std::cout << StrassenOut[i][j] << " ";
//         }
//         std::cout << "\n";
//     }
	msec = ((float)diff)/CLOCKS_PER_SEC;
	printf("TIME: %d, %f\n",diff,msec);
	scanf("%d",&hold);
	return 0;
}
