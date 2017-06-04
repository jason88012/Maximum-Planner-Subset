#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int buffsize = 10000;

#define MAX(a,b) (a>b?a:b)

int **mallocMatrix(int row, int col, int attr)
{
	int **A, *A_row;
	int i, j;

	A = (int**)malloc(row*sizeof(void*));
	A_row = (int*)malloc(row*col*sizeof(int));

	for (i = 0; i < row; ++i, A_row += col)
	{
		A[i] = A_row;
	}

	if (attr == -1)
	{
		for (i = 0; i < row; ++i)
		{
			for (j = 0; j < col; ++j)
			{
				A[i][j] = -1;
			}
		}
	}
	else if (attr == 0)
	{
		for (i = 0; i < row; ++i)
		{
			for (j = 0; j < col; ++j)
			{
				A[i][j] = 0;
			}
		}		
	}

	return A;
}

int *readFile(char const *fileName)
{
	FILE *fp = fopen(fileName, "r");

	int *inputs;
	int numbers;
	int i, n, c;
	int flag;

	inputs = (int*)malloc(buffsize*sizeof(int));

	n = 0;	flag = 0;
	while ( (c = fgetc(fp)) != EOF)
	{
		if (flag == 0)
		{
			fscanf(fp, "%d", &numbers);
			flag++;
		}
		else
		{
			fscanf(fp, "%d", &inputs[n]);
			n++;			
		}
	}

	n--;
	inputs[n] = -1;
	fclose(fp);

	return inputs;
}

int calcSize(char const *fileName)
{
	FILE *fp = fopen(fileName, "r");
	int *buff = (int*)malloc(buffsize*sizeof(int));
	int size = 0;
	int c;

	while ( (c = fgetc(fp)) != EOF)
	{
		fscanf(fp, "%d", &buff[size]);
		size++;
	}

	fclose(fp);
	free(buff);
	// except first number and last number
	size = size - 2;

	return size;
}

int findLine(int *inputs, int key)
{
	int i = 0;
	int other;
	int j;

	while(1)
	{
		if (inputs[i] == key)
		{
			j = i;
			if (j%2 == 0)
				other = inputs[++j];
			else
				other = inputs[--j];

			break;
		}
		i++;	
	}

	return other;
}

// MIS(i, j) = maximun numbers of chords between i~j
int **calcMIS(int *inputs, int **checkCase, int length)
{
	int **MIS = mallocMatrix(length, length, 0);
	int i,j,k;
	int diff;
	int count = 0;

	// MIS[i][i] = 0
	for (i = 0; i < length; ++i)
	{
		for (j = 0; j < length; ++j)
		{
			if (i == j)
			{
				MIS[i][j] = 0;
			}
		}
	}
	
	for (diff = 1; diff < length; ++diff)
	{
		i = 0;	j = i + diff;
		//printf("\nthis iteration diff = %d\n", diff);
		
		while(j != length)
		{
			k = findLine(inputs, j);
			// case1: k>j or k<i
			if(k < i || k > j)
			{
				MIS[i][j] = MIS[i][j-1];
				//printf("case1  \t");
				//printf("MIS[%d][%d] = %d\n", i,j,MIS[i][j]);
			}
			// case2: i<k<j
			else if (k > i && k < j)
			{
				// case2-1: line JK not include MIS
				if (MIS[i][j-1] >= (MIS[i][k-1] + MIS[k+1][j-1] + 1))
				{
					MIS[i][j] = MIS[i][j-1];
					//printf("case2-1\t");
					//printf("MIS[%d][%d] = %d\n", i,j,MIS[i][j]);
				}
				// case2-2: line JK include MIS
				else
				{
					MIS[i][j] = MIS[i][k-1] + MIS[k+1][j-1] + 1;
					checkCase[i][j] = 2;
					//printf("case2-2\t");
					//printf("MIS[%d][%d] = %d\n", i,j,MIS[i][j]);
				}
			}
			// case3: k == i
			else if(k == i)
			{
				if (diff == 1)
				{
					MIS[i][j] = 1;
					checkCase[i][j] = 3;
				}
				
				else
				{
					MIS[i][j] = MIS[i+1][j-1] +1;
					checkCase[i][j] = 3;
				}
				//printf("case3  \t");
				//printf("MIS[%d][%d] = %d\n", i,j,MIS[i][j]);
			}
			i++;
			j++;
		}
	}

	return MIS;
}

// global variable: a
// use in recursive counter
int a = 0;
void findChord(int i, int j, int *inputs, int **checkCase, int** lines)
{
	while( j-i > 1)
	{
		if (checkCase[i][j] == 2)
		{
			int k = findLine(inputs, j);
			//printf("%d %d\n", j, k);
			lines[a][0] = j;
			lines[a][1] = k;
			a++;
			findChord(k, j-1, inputs, checkCase, lines);
			j = k - 1;
		}
		else if(checkCase[i][j] == 3)
		{
			int k = findLine(inputs, j);
			//printf("%d %d\n", j, k);
			lines[a][0] = j;
			lines[a][1] = k;
			a++;
			findChord(i+1, j-1, inputs, checkCase, lines);
			j = i - 1;
		} 
		else
		{
			j--;
		}
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 3)
	{
		printf("Please enter input or output file name!\n");
		return 0;
	}

	int *inputs, *outputs, **MIS, **checkCase, **lines;
	int length, result;
	FILE *fout;
	fout = fopen(argv[2], "w+t");

	inputs = readFile(argv[1]);
	length = calcSize(argv[1]);
	checkCase = mallocMatrix(length, length, -1);

	MIS = calcMIS(inputs, checkCase, length);
	fprintf(fout, "%d\n", MIS[0][length-1]);
	
	lines = mallocMatrix(MIS[0][length-1], 2, -1);
	findChord(0, length-1, inputs, checkCase, lines);

	// write into output file
	for (int i = 0; i < MIS[0][length-1]; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			fprintf(fout, "%d ", lines[i][j]);
		}
		fprintf(fout, "\n");
	}

	return 0;
}




