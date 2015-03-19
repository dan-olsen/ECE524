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

void applyPattern(int i, int *patIndex, int *tmpVal);
void printInputVector(char *input);
void printPattern(int patIndex);
void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
void initDelay();
DdNode *createZDD(LIST *pathList);
void buildNLongestPath(int n, int NodeIndex, int PathSetIndex);

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
	int i, j, mark = 0, tmpCount;
	LIST *tmpList = NULL;
	PATH_COUNT *pathIter = NULL, *currPath = NULL;
	int tmpDelay;

	for(i = 0, tmpDelay = 0; i <= Tgat; i++, tmpDelay = 0)
	{
		switch(Node[i].Type) {
			case INPT:
				Node[i].Delay = 0;
				printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

				InsertPathCount(&Node[i].Path, 0, 1);
				printf("Path at %d = %d,%d\n", i, Node[i].Path->Delay, Node[i].Path->Count);

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
					{
						tmpDelay = Node[tmpList->Id].Delay;
					}
				}

				Node[i].Delay = tmpDelay + 1;
				printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

				for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
				{
					for(pathIter = Node[tmpList->Id].Path; pathIter != NULL; pathIter = pathIter->Next)
					{
						if(Node[i].Path != NULL)
						{
							for(currPath = Node[i].Path; currPath != NULL; currPath = currPath->Next)
							{
								if(currPath->Delay == pathIter->Delay + 1)
								{
									currPath->Count = currPath->Count + pathIter->Count;

									mark = 1;
								}
							}

							if(mark == 1)
							{
								mark = 0;

							} else {
								InsertPathCount(&Node[i].Path, pathIter->Delay + 1, pathIter->Count);

							}
						} else {
							InsertPathCount(&Node[i].Path, pathIter->Delay + 1, pathIter->Count);

						}
					}
				}

				for(currPath = Node[i].Path; currPath != NULL; currPath = currPath->Next)
				{
					printf("Path at %d: Delay = %d Count = %d\n", i, currPath->Delay, currPath->Count);

				}

				break;
			case FROM:
				tmpList = Node[i].Fin;

				Node[i].Delay = Node[tmpList->Id].Delay;
				printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

				for(pathIter = Node[tmpList->Id].Path; pathIter != NULL; pathIter = pathIter->Next)
				{
					InsertPathCount(&Node[i].Path, pathIter->Delay, pathIter->Count);

				}

				for(currPath = Node[i].Path; currPath != NULL; currPath = currPath->Next)
				{
					printf("Path at %d: Delay = %d Count = %d\n", i, currPath->Delay, currPath->Count);

				}

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}

	pathSet = malloc(sizeof(PATH_SET *) * Npo);

	for(i = 0, j = Tgat-Npo+1; i < Npo; i++, j++)
	{
		pathSet[i].Id = j;
		pathSet[i].numLongestPath = 0;
		pathSet[i].numSecondLongestPath = 0;

		for(currPath = Node[j].Path; currPath != NULL; currPath = currPath->Next)
		{
			if(currPath->Delay == Node[j].Delay)
				pathSet[i].numLongestPath += currPath->Count;
			else if(currPath->Delay == Node[j].Delay - 1)
				pathSet[i].numSecondLongestPath += currPath->Count;
			else
				printf("Error\n");
		}

		printf("%d %d\n", pathSet[i].numLongestPath, pathSet[i].numSecondLongestPath);

		pathSet[i].longestPath = malloc(sizeof(PATH *) * pathSet[i].numLongestPath);
		pathSet[i].secondLongestPath = malloc(sizeof(PATH *) * pathSet[i].numSecondLongestPath);

		buildNLongestPath(1, j, i);
		buildNLongestPath(2, j, i);

	}

	printf("\n");
}

void patternSim()
{
	int i, j, patIndex, tmpVal;
	DdNode *tmpNode, *tmpNode2;
	PATH_COUNT *tmpPath;

	initDelay();

	/*
	tmpNode = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(tmpNode);

	tmpNode2 = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(tmpNode2);

	goodPaths.node = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(goodPaths.node);

	suspectSet.node = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(suspectSet.node);
	*/

	//iterate over patterns
	for(patIndex = 0; patIndex < Tpat; printf("\n"))
	{
		printf("Applying Pattern: ");
		printPattern(patIndex);

		//topologoical traversal to apply pattern
		for(tmpVal = 0, i = 0; i <= Tgat; i++, tmpVal = 0)
		{
			applyPattern(i, &patIndex, &tmpVal);

			//set mark
			if((Node[i].Val == R1) || (Node[i].Val == F0))
			{
				Node[i].Mark = 1;

			} else {
				Node[i].Mark = 0;

			}
		}

		for(j = Tgat-Npo+1; j <= Tgat; j++)
		{
			printf("Output of %s = %d\n", Node[j].Name, Node[j].Val);
			/*
			storeRobustPaths();
			*/

			/*
			for(tmpPath = Node[j].LongestPath; tmpPath != NULL; tmpPath = tmpPath->Next)
			{
				tmpNode2 = createZDD(tmpPath->Path);
				tmpNode = Cudd_zddUnion(manager, tmpNode2, goodPaths.node);
				Cudd_Ref(tmpNode);
				Cudd_RecursiveDeref(manager, tmpNode2);
				Cudd_RecursiveDeref(manager, goodPaths.node);

				goodPaths.node = tmpNode;

				printf("ZDD Count: %d\n", Cudd_zddCount(manager, goodPaths.node));
			}
			*/
		}
	}
}

void storeRobustPaths()
{
	int i, j;
	LIST *tmpList = NULL;
	DdNode *tmpNode;

	for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++)
	{
		switch(Node[i].Type) {
			case INPT:
				if(Node[i].Val == R1)
				{
					Node[i].Rpath = Cudd_zddChange(manager, onez, 2*i);
					Cudd_Ref(Node[i].Rpath);

				} else if(Node[i].Val == F0) {
					Node[i].Fpath = Cudd_zddChange(manager, onez, (2*i)+1);
					Cudd_Ref(Node[i].Fpath);

				}

				break;
			case AND:
				if(Node[i].Mark == 1)
				{
					if(Node[i].Val == R1)
					{
						Node[i].Rpath = Cudd_zddChange(manager, onez, 2*i);
						Cudd_Ref(Node[i].Rpath);

					} else if(Node[i].Val == F0) {
						Node[i].Fpath = Cudd_zddChange(manager, onez, (2*i)+1);
						Cudd_Ref(Node[i].Fpath);

					}

					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{

						if(Node[i].Val == R1)
						{
							tmpNode = Cudd_zddUnion(manager, Node[tmpList->Id].Rpath, Node[i].Rpath);
							Cudd_Ref(tmpNode);

							Node[i].Rpath = tmpNode;

						} else if(Node[i].Val == F0) {
							tmpNode = Cudd_zddUnion(manager, Node[tmpList->Id].Fpath, Node[i].Fpath);
							Cudd_Ref(tmpNode);

							Node[i].Fpath = tmpNode;
						}
					}
				}

				break;
			case NAND:
				if(Node[i].Mark == 1)
				{
					if(Node[i].Val == R1)
					{
						Node[i].Rpath = Cudd_zddChange(manager, onez, 2*i);
						Cudd_Ref(Node[i].Rpath);

					} else if(Node[i].Val == F0) {
						Node[i].Fpath = Cudd_zddChange(manager, onez, (2*i)+1);
						Cudd_Ref(Node[i].Fpath);

					}

					for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
					{

						if(Node[i].Val == R1)
						{
							tmpNode = Cudd_zddUnion(manager, Node[tmpList->Id].Rpath, Node[i].Rpath);
							Cudd_Ref(tmpNode);

							Node[i].Rpath = tmpNode;

						} else if(Node[i].Val == F0) {
							tmpNode = Cudd_zddUnion(manager, Node[tmpList->Id].Fpath, Node[i].Fpath);
							Cudd_Ref(tmpNode);

							Node[i].Fpath = tmpNode;
						}
					}
				}

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
				tmpList = Node[i].Fin;

				if(Node[i].Val == R1)
				{
					Node[i].Rpath = Cudd_zddChange(manager, Node[tmpList->Id].Rpath, 2*i);
					Cudd_Ref(Node[i].Rpath);

				} else if(Node[i].Val == F0) {
					Node[i].Fpath = Cudd_zddChange(manager, Node[tmpList->Id].Fpath, (2*i)+1);
					Cudd_Ref(Node[i].Fpath);

				}

				break;
			case NOT:
				tmpList = Node[i].Fin;


				break;
			case FROM:
				tmpList = Node[i].Fin;

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}

	for(j = Tgat-Npo+1; j <= Tgat; j++)
	{
		if(Node[j].Val == R1)
		{
			tmpNode = Cudd_zddUnion(manager, robustPaths.Rpath, Node[j].Rpath);
			Cudd_Ref(tmpNode);

			robustPaths.Rpath = tmpNode;
		} else if(Node[j].Val == F0) {
			tmpNode = Cudd_zddUnion(manager, robustPaths.Fpath, Node[j].Fpath);
			Cudd_Ref(tmpNode);

			robustPaths.Fpath = tmpNode;
		}
	}

	clearNodeZDDs();
}

DdNode *createZDD(LIST *pathList)
{
	DdNode *node, *tmp;

	node = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(node);

	for( ; pathList != NULL; pathList = pathList->Next)
	{
		tmp = Cudd_zddChange(manager, node, pathList->Id);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(manager, node);

		node = tmp;

	}

	return node;
}

void clearPathZDDs()
{
	/*
	if(robustPaths.Rpath != NULL)
		Cudd_RecursiveDeref(manager, robustPaths.Rpath);

	if(robustPaths.Fpath != NULL)
		Cudd_RecursiveDeref(manager, robustPaths.Fpath);
	*/
	//Cudd_RecursiveDeref(manager, goodPaths.node);
	//Cudd_RecursiveDeref(manager, suspectSet.node);

}

void clearNodeZDDs()
{
	int i;

	for(i = 0; i < Tgat; i++)
	{
		switch(Node[i].Type) {
			case INPT:
			case AND:
			case NAND:
			case OR:
			case NOR:
			case XOR:
			case XNOR:
			case FROM:
			case NOT:
			case BUFF:
				//printf("Recursive Deref %d\n", i);
				if(Node[i].Rpath != NULL)
					Cudd_RecursiveDeref(manager, Node[i].Rpath);

				if(Node[i].Fpath != NULL)
					Cudd_RecursiveDeref(manager, Node[i].Fpath);

				break;

			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

void applyPattern(int i, int *patIndex, int *tmpVal)
{
	LIST *tmpList = NULL;

	switch(Node[i].Type) {
		case INPT:
			Node[i].Val = patterns[*patIndex];

			*patIndex = *patIndex + 1;

			break;
		case AND:
			for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
			{
				if(tmpList == Node[i].Fin)
				{
					*tmpVal = Node[tmpList->Id].Val;

					continue;
				}

				*tmpVal = robustSimAND[*tmpVal][Node[tmpList->Id].Val];
			}

			Node[i].Val = *tmpVal;

			break;
		case NAND:
			for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
			{
				if(tmpList == Node[i].Fin)
				{
					*tmpVal = Node[tmpList->Id].Val;

					continue;
				}

				*tmpVal = robustSimAND[*tmpVal][Node[tmpList->Id].Val];

			}

			Node[i].Val = simNOT[*tmpVal];

			break;
		case OR:
			for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
			{
				if(tmpList == Node[i].Fin)
				{
					*tmpVal = Node[tmpList->Id].Val;

					continue;
				}

				*tmpVal = robustSimOR[*tmpVal][Node[tmpList->Id].Val];

			}

			Node[i].Val = *tmpVal;

			break;
		case NOR:
			for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
			{
				if(tmpList == Node[i].Fin)
				{
					*tmpVal = Node[tmpList->Id].Val;

					continue;
				}

				*tmpVal = robustSimOR[*tmpVal][Node[tmpList->Id].Val];

			}

			Node[i].Val = simNOT[*tmpVal];

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

			break;
		case NOT:
			tmpList = Node[i].Fin;

			Node[i].Val = simNOT[Node[tmpList->Id].Val];

			break;
		case FROM:
			tmpList = Node[i].Fin;

			Node[i].Val = Node[tmpList->Id].Val;

			break;
		default:
			//printf("Hit Default at i: %d ", i);
			//printf("Type: %d\n", graph[i].typ);
			break;
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

void InsertPathCount(PATH_COUNT **Cur, int delay, int count)
{
	PATH_COUNT *tl=NULL;
	PATH_COUNT *nl=NULL;

	if ((tl=(PATH_COUNT *) malloc(sizeof(PATH_COUNT)))==NULL){
		printf("LIST: Out of memory\n");
		exit(1);
	} else {
		tl->Next = NULL;
		tl->Delay = delay;
		tl->Count = count;

		if(*Cur==NULL)
		{
			*Cur=tl;
			return;
		}

		nl=*Cur;

		while(nl!=NULL)
		{
			if(nl->Next==NULL)
			{
				nl->Next=tl;
				break;
			}

			nl=nl->Next;
		}
	}

	return;
}

void buildNLongestPath(int n, int NodeIndex, int PathSetIndex)
{
	LIST *tmpList;
	PATH_COUNT *pathIter = NULL, *currPath = NULL;
	int mark = 0;

	for(tmpList = Node[NodeIndex].Fin; tmpList != NULL; tmpList = tmpList->Next)
	{
		for(pathIter = Node[tmpList->Id].Path; pathIter != NULL; pathIter = pathIter->Next)
		{
			for(currPath = Node[NodeIndex].Path; currPath != NULL; currPath = currPath->Next)
			{
				if(currPath->Delay == pathIter->Delay + 1)
				{

				}
			}

		}
	}
}


/****************************************************************************************************************************/
