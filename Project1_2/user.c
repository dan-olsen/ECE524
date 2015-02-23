#include "given.h"
#include "user.h"

void printInputVector(char * input, int Npi);
void printPattern(int *patterns, int Npi, int patIndex);

int readPatternFile(int *patterns, FILE* patFile, int Npi)
{
	int patRowIndex, patColIndex, patIndex;
	char vector1 [Mpi];
	char vector2  [Mpi];
	int readCount1, readCount2;

	for(patIndex = 0; !feof(patFile);)
	{
		readCount1 = fscanf(patFile, "%s", vector1);

		if(readCount1 != -1)
		{
			readCount2 = fscanf(patFile, "%s", vector2);

			if(readCount2 != -1)
			{
				if(readCount1 == readCount2)
				{
					printInputVector(vector1, Npi);
					printInputVector(vector2, Npi);

					for(patColIndex = 0; patColIndex < Npi; patColIndex++, patIndex++)
					{
						if((vector1[patColIndex] == '0') && (vector2[patColIndex] == '0'))
						{
							patterns[patIndex] = STABLE_0;

						} else if((vector1[patColIndex] == '0') && (vector2[patColIndex] == '1')) {
							patterns[patIndex] = RISING_1;

						} else if((vector1[patColIndex] == '1') && (vector2[patColIndex] == '0')) {
							patterns[patIndex] = FALLING_0;

						} else if((vector1[patColIndex] == '1') && (vector2[patColIndex] == '1')) {
							patterns[patIndex] = STABLE_1;

						} else if(((vector1[patColIndex] == 'x') || (vector1[patColIndex] == 'X')) && (vector2[patColIndex] == '1')) {
							patterns[patIndex] = X_1;

						} else if(((vector1[patColIndex] == 'x') || (vector1[patColIndex] == 'X')) && (vector2[patColIndex] == '0')) {
							patterns[patIndex] = X_0;

						} else {
							//error
							continue;

						}
					}

					printPattern(patterns, Npi, patIndex);

				} else {
					//error
					continue;

				}

			} else {
				//error
				continue;

			}
		} else {
			//error
			continue;
		}

		patterns = (int *) realloc(patterns, (Npi+patIndex) * sizeof(int));
	}

	return (patIndex);
}

void initDelay(GATE *Node, int Tgat)
{
	int i;

	for(i = 0; i < Tgat; i++)
	{
		switch(Node[i].Type) {
			case INPT:


				break;
			case AND:


				break;
			case NAND:


				break;
			case OR:


				break;
			case NOR:


				break;
			case XOR:


				break;
			case XNOR:


				break;
			case BUFF:


				break;
			case NOT:


				break;
			case FROM:


				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

void patternSim(GATE *Node, int *Patterns, int Tgat, int Npi, int Npo)
{
	int i;

	for(i = 0; i < Tgat; i++)
	{
		switch(Node[i].Type) {
			case INPT:


				break;
			case AND:


				break;
			case NAND:


				break;
			case OR:


				break;
			case NOR:


				break;
			case XOR:


				break;
			case XNOR:


				break;
			case BUFF:


				break;
			case NOT:


				break;
			case FROM:


				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

void printInputVector(char * input, int Npi)
{
	int i;

	for(i = 0; i < Npi; i++)
	{
		printf("%c", input[i]);
	}

	printf("\n");
}

void printPattern(int *patterns, int Npi, int patIndex)
{
	int i;

	printf("Resulting Pattern: ");

	for(i = patIndex-Npi; i < patIndex; i++)
	{
		printf("%d", patterns[i]);
	}

	printf("\n\n");
}

/****************************************************************************************************************************/
