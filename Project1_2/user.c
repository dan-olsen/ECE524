#include "given.h"
#include "user.h"

void readPatternFile(int *patterns, FILE* patFile)
{
	int i, j;
	char vector1 [Mpi];
	char vector2  [Mpi];
	int readCount1, readCount2;

	for(i = 0; !feof(patFile); i++)
	{
		readCount1 = fscanf(patFile, "%s", vector1);
		if(readCount1 != 0)
		{
			readCount2 = fscanf(patFile, "%s", vector2);
			if(readCount2 != 0)
			{
				if(readCount1 == readCount2)
				{
					for(j = 0; j < readCount2; j++)
					{
						if((vector1[j] == '0') && (vector2[j] == '0'))
						{
							printf("Here 00\n");
						} else if((vector1[j] == '0') && (vector2[j] == '1')) {
							printf("Here 01\n");

						} else if((vector1[j] == '1') && (vector2[j] == '0')) {
							printf("Here 10\n");

						} else if((vector1[j] == '1') && (vector2[j] == '1')) {
							printf("Here 11\n");

						} else if(((vector1[j] == 'x') || (vector1[j] == 'X')) && (vector2[j] == '1')) {
							printf("Here x1\n");

						} else if(((vector1[j] == 'x') || (vector1[j] == 'X')) && (vector2[j] == '0')) {
							printf("Here x0\n");

						} else {
							//error
						}
					}
				} else {
					//error
				}

			} else {
				//error
			}
		} else {
			//error
		}


	}
}

/****************************************************************************************************************************/
