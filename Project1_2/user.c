#include "given.h"
#include "user.h"

const int robustSimOR [6][6] 		=	{{S0, R1, F0, S1, X1, X0},
										 {R1, X1, X1, S1, X1, X1},
										 {F0, X1, F0, S1, X1, F0},
										 {S1, S1, S1, S1, S1, S1},
										 {X1, X1, X1, S1, X1, X1},
										 {X0, X1, F0, S1, X1, X0}};

const int nonRobustSimOR [6][6]		= 	{{S0, R1, F0, S1, X1, X0},
										 {R1, X1, R1, S1, X1, R1},
										 {F0, R1, F0, S1, X1, F0},
										 {S1, S1, S1, S1, S1, S1},
										 {X1, X1, X1, S1, X1, X1},
										 {X0, R1, F0, S1, X1, X0}};

const int robustSimAND [6][6] 		= 	{{S0, S0, S0, S0, S0, S0},
										 {S0, R1, X0, R1, R1, X0},
										 {S0, X0, X0, F0, X0, X0},
										 {S0, R1, F0, S1, X1, X0},
										 {S0, R1, X0, X1, X1, X0},
										 {S0, X0, X0, X0, X0, X0}};

const int nonRobustSimAND [6][6]	= 	{{S0, S0, S0, S0, S0, S0},
								 	 	 {S0, R1, F0, R1, R1, X0},
								 	 	 {S0, F0, X0, F0, F0, X0},
								 	 	 {S0, R1, F0, S1, X1, X0},
								 	 	 {S0, R1, F0, X1, X1, X0},
								 	 	 {S0, X0, X0, X0, X0, X0}};

const int simNOT [6] 				= 	 {S1, F0, R1, S0, X0, X1};

void printInputVector(char * input);
void printPattern(int patIndex);

void readPatternFile(FILE* patFile)
{
	int patColIndex, patIndex;
	char vector1 [Mpi];
	char vector2  [Mpi];
	int readCount1, readCount2;

	if ( NULL == (patterns =  malloc(Npi * sizeof(int)))) {
		printf("malloc failed\n");
		//error
	}

	for(patIndex = 0; !feof(patFile);)
	{
		readCount1 = fscanf(patFile, "%s", vector1);
		readCount2 = fscanf(patFile, "%s", vector2);

		if((readCount2 != -1) && (readCount1 != -1) && (readCount1 == readCount2))
		{
				printInputVector(vector1);
				printInputVector(vector2);

			for(patColIndex = 0; patColIndex < Npi; patColIndex++, patIndex++)
			{
				if((vector1[patColIndex] == '0') && (vector2[patColIndex] == '0'))
				{
					patterns[patIndex] = S0;

				} else if((vector1[patColIndex] == '0') && (vector2[patColIndex] == '1')) {
					patterns[patIndex] = R1;

				} else if((vector1[patColIndex] == '1') && (vector2[patColIndex] == '0')) {
					patterns[patIndex] = F0;

				} else if((vector1[patColIndex] == '1') && (vector2[patColIndex] == '1')) {
					patterns[patIndex] = S1;

				} else if(((vector1[patColIndex] == 'x') || (vector1[patColIndex] == 'X')) && (vector2[patColIndex] == '1')) {
					patterns[patIndex] = X1;

				} else if(((vector1[patColIndex] == 'x') || (vector1[patColIndex] == 'X')) && (vector2[patColIndex] == '0')) {
					patterns[patIndex] = X0;

				} else {
					//error
					continue;

				}
			}

			printf("Resulting Pattern: ");
			printPattern(patIndex-Npi);
			printf("\n");

			patterns = realloc(patterns, (Npi+patIndex) * sizeof(int));

		}
	}

	Tpat = patIndex;
}

void initDelay()
{
	int i;
	LIST *tmpList = NULL;
	int tmpDelay;

	for(i = 0, tmpDelay = 0; i < Tgat; i++, tmpDelay = 0)
	{
		switch(Node[i].Type) {
			case INPT:
				Node[i].Delay = 0;

				break;
			case AND:
			case NAND:
			case OR:
			case NOR:
			case XOR:
			case XNOR:
			case BUFF:
			case NOT:
				for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
				{
					if(tmpDelay < Node[tmpList->Id].Delay)
						tmpDelay = Node[tmpList->Id].Delay;
				}

				Node[i].Delay = tmpDelay + 1;

				break;
			case FROM:
				Node[i].Delay = Node[Node[i].Fin->Id].Delay;

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

void patternSim()
{
	int i, patIndex, tmpVal;
	LIST *tmpList = NULL;

	for(patIndex = 0; patIndex < Tpat; printf("\n"))
	{
		printf("Applying Pattern: ");
		printPattern(patIndex);

		for(tmpVal = 0, i = 0; i <= Tgat; i++, tmpVal = 0)
		{
			switch(Node[i].Type) {
				case INPT:
					Node[i].Val = patterns[patIndex];

					//printf("INPT %s Val = %d\n", Node[i].Name, Node[i].Val);

					patIndex++;

					break;
				case AND:
					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{
						if(tmpList == Node[i].Fin)
						{
							tmpVal = Node[tmpList->Id].Val;

							//printf("AND %s Fin %s tmpVal = %d\n", Node[i].Name, Node[tmpList->Id].Name, tmpVal);

							continue;
						}

						//printf("AND %s Fin %s val = %d tmpVal = %d\n", Node[i].Name, Node[tmpList->Id].Name, Node[tmpList->Id].Val, tmpVal);
						tmpVal = robustSimAND[tmpVal][Node[tmpList->Id].Val];

						//printf("AND %s Sim tmpVal = %d\n", Node[i].Name, tmpVal);

					}

					Node[i].Val = tmpVal;

					//printf("AND %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case NAND:
					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{
						if(tmpList == Node[i].Fin)
						{
							tmpVal = Node[tmpList->Id].Val;

							continue;
						}

						tmpVal = robustSimAND[tmpVal][Node[tmpList->Id].Val];

					}

					Node[i].Val = simNOT[tmpVal];

					//printf("NAND %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case OR:
					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{
						if(tmpList == Node[i].Fin)
						{
							tmpVal = Node[tmpList->Id].Val;

							continue;
						}

						tmpVal = robustSimOR[tmpVal][Node[tmpList->Id].Val];

					}

					Node[i].Val = tmpVal;

					//printf("OR %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case NOR:
					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{
						if(tmpList == Node[i].Fin)
						{
							tmpVal = Node[tmpList->Id].Val;

							continue;
						}

						tmpVal = robustSimOR[tmpVal][Node[tmpList->Id].Val];

					}

					Node[i].Val = simNOT[tmpVal];

					//printf("NOR %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case XOR:
					printf("XOR not supported");


					break;
				case XNOR:
					printf("XNOR not supported");


					break;
				case BUFF:
					tmpList = Node[i].Fin;

					Node[i].Val = Node[tmpList->Id].Val;

					//printf("BUFF %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case NOT:
					tmpList = Node[i].Fin;

					Node[i].Val = simNOT[Node[tmpList->Id].Val];

					//printf("NOT %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				case FROM:
					tmpList = Node[i].Fin;

					Node[i].Val = Node[tmpList->Id].Val;

					//printf("FROM %s Val = %d\n", Node[i].Name, Node[i].Val);

					break;
				default:
					//printf("Hit Default at i: %d ", i);
					//printf("Type: %d\n", graph[i].typ);
					break;
			}

			if(i > (Tgat-Npo))
			{
				printf("Output of %s = %d\n", Node[i].Name, Node[i].Val);
			}
		}
	}
}

void printInputVector(char * input)
{
	int i;

	for(i = 0; i < Npi; i++)
	{
		printf("%c", input[i]);
	}

	printf("\n");
}

void printPattern(int patIndex)
{
	int i;

	for(i = patIndex; i < patIndex+Npi; i++)
	{
		printf("%d", patterns[i]);
	}

	printf("\n");
}

/****************************************************************************************************************************/
