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
void initDelay();
DdNode *createZDD(LIST *pathList);
void markPath();
void buildNLongestPath(int n, int NodeIndex, int PathSetIndex, int PathIndex);
int compareList(LIST *a, LIST *b);

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
	int i, j, mark = 0, tmpCount, k;
	LIST *tmpList = NULL;
	int tmpDelay;

	for(i = 0, tmpDelay = 0; i <= Tgat; i++, tmpDelay = 0)
	{
		switch(Node[i].Type) {
			case INPT:
				Node[i].Delay = 0;
				printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

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

				break;
			case FROM:
				tmpList = Node[i].Fin;

				Node[i].Delay = Node[tmpList->Id].Delay;
				printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}

	pathSet = malloc(sizeof(PATH_SET *) * Npo);

	for(i = 0, j = Tgat-Npo+1, markPath(); i < Npo; i++, j++)
	{
		pathSet[i].Id = j;
		pathSet[i].numLongestPath = Node[j].Long1;
		pathSet[i].numSecondLongestPath = Node[j].Long2;

		printf("%d %d\n", pathSet[i].numLongestPath, pathSet[i].numSecondLongestPath);

		pathSet[i].longestPath = malloc(sizeof(PATH *) * pathSet[i].numLongestPath);
		pathSet[i].secondLongestPath = malloc(sizeof(PATH *) * pathSet[i].numSecondLongestPath);

		for(k = 0; k < pathSet[i].numLongestPath; k++, markPath())
		{
			InsertEle(&pathSet[i].longestPath[k].Path, j);
			buildNLongestPath(1, j, i, k);

			printf("Longest Path #%d at %d = ", k, j);
			PrintList(pathSet[i].longestPath[k].Path);
			printf("\n");
		}
	}

	printf("\n");
}

void markPath()
{
	int i, j, mark = 0, tmpCount, k;
	LIST *tmpList = NULL;
	int tmpDelay;

	for(i = 0, tmpDelay = 0; i <= Tgat; i++, tmpDelay = 0)
	{
		switch(Node[i].Type) {
			case INPT:
				Node[i].Long1 = 1;
				Node[i].Long2 = 1;

				//printf("Long1 at %d = %d\n", i, Node[i].Long1);
				//printf("Long2 at %d = %d\n", i, Node[i].Long2);

				break;
			case AND:
			case NAND:
			case OR:
			case NOR:
			case XOR:
			case XNOR:
			case BUFF:
			case NOT:
				Node[i].Long1 = 0;
				Node[i].Long2 = 0;

				for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
				{
					if(Node[i].Delay == Node[tmpList->Id].Delay + 1)
					{
						Node[i].Long1 += Node[tmpList->Id].Long1;

						if(Node[tmpList->Id].Long2 != 0)
						{
							mark = tmpList->Id;
						}

					} else if(Node[i].Delay == Node[tmpList->Id].Delay + 2) {
						Node[i].Long2 += Node[tmpList->Id].Long2;

					}
				}

				if(mark != 0)
				{
					Node[i].Long2 += Node[mark].Long2;

				} else {
					mark = 0;
				}

				//printf("Long1 at %d = %d\n", i, Node[i].Long1);
				//printf("Long2 at %d = %d\n", i, Node[i].Long2);


				break;
			case FROM:
				Node[i].Long1 = 0;
				Node[i].Long2 = 0;

				tmpList = Node[i].Fin;

				Node[i].Long1 = Node[tmpList->Id].Long1;
				Node[i].Long2 = Node[tmpList->Id].Long2;

				//printf("Long1 at %d = %d\n", i, Node[i].Long1);
				//printf("Long2 at %d = %d\n", i, Node[i].Long2);

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

void buildNLongestPath(int n, int NodeIndex, int PathSetIndex, int PathIndex)
{
	LIST *tmpList;
	int mark = 0;
	int i;

	for(tmpList = Node[NodeIndex].Fin; tmpList != NULL; tmpList = tmpList->Next)
	{
			if(((Node[NodeIndex].Delay == Node[tmpList->Id].Delay + n) || (Node[NodeIndex].Delay == Node[tmpList->Id].Delay)) && (Node[tmpList->Id].Long1 != 0))
			{
				InsertEle(&pathSet[PathSetIndex].longestPath[PathIndex].Path, tmpList->Id);

				for(i = 0; i < pathSet[PathSetIndex].numLongestPath; i++)
				{
					if(compareList(pathSet[PathSetIndex].longestPath[PathIndex].Path, pathSet[PathSetIndex].longestPath[i].Path) && PathIndex != i)
					{
						mark = 1;
					}
				}

				if(mark == 1)
				{
					mark = 0;

					DeleteEle(&pathSet[PathSetIndex].longestPath[PathIndex].Path, tmpList->Id);

					continue;
				}

				Node[tmpList->Id].Long1--;

				buildNLongestPath(1, tmpList->Id, PathSetIndex, PathIndex);

				return;
			}
	}
}

void patternSim()
{
	int i, j, patIndex, tmpVal;
	DdNode *tmpNode, *tmpNode2;

	initDelay();

	tmpNode = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(tmpNode);

	tmpNode2 = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(tmpNode2);

	goodPaths.node = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(goodPaths.node);

	suspectSet.node = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(suspectSet.node);

	robustPaths.Rpath = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(robustPaths.Rpath);

	robustPaths.Fpath = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(robustPaths.Fpath);

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

		}

		storeRobustPaths();
		printf("Robust Path Rpath ZDD Count: %d\n", Cudd_zddCount(manager, robustPaths.Rpath));
		printf("Robust Path Fpath ZDD Count: %d\n", Cudd_zddCount(manager, robustPaths.Fpath));

		/*
		for(i = 0; i < Npo; i++)
		{
			for(j = 0; j < pathSet[i].numLongestPath; j++)
			{
				tmpNode2 = createZDD(pathSet[i].longestPath[j].Path);
				tmpNode = Cudd_zddUnion(manager, tmpNode2, goodPaths.node);
				Cudd_Ref(tmpNode);
				Cudd_RecursiveDeref(manager, tmpNode2);
				Cudd_RecursiveDeref(manager, goodPaths.node);

				goodPaths.node = tmpNode;

				printf("ZDD Count: %d\n", Cudd_zddCount(manager, goodPaths.node));
			}

		}*/
	}
}

void storeRobustPaths()
{
	int i, j;
	LIST *tmpList = NULL;
	DdNode *tmpNode, *tmpNode2, *tmpNode3, *tmpRobust;

	for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++, tmpList = Node[i].Fin)
	{
		switch(Node[i].Type) {
			case INPT:
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

					Node[i].RobustPath = Cudd_zddChange(manager, onez, i);
					Cudd_Ref(Node[i].RobustPath);
				}

				break;
			case AND:
			case NAND:
			case OR:
			case NOR:
			case XOR:
			case XNOR:
			case BUFF:
			case NOT:
			case FROM:
				tmpList = Node[i].Fin;

				if(Node[i].Mark == 1)
				{
					if(Node[tmpList->Id].Val == R1)
					{
						tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Rpath, 2*i);
						Cudd_Ref(tmpNode);

					} else if(Node[tmpList->Id].Val == F0) {
						tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Fpath, (2*i)+1);
						Cudd_Ref(tmpNode);
					}

					for(tmpList = tmpList->Next; tmpList != NULL; tmpList = tmpList->Next)
					{
						if(Node[tmpList->Id].Val == R1)
						{
							tmpNode2 = Cudd_zddChange(manager, Node[tmpList->Id].Rpath, 2*i);
							Cudd_Ref(tmpNode2);

						} else if(Node[tmpList->Id].Val == F0) {
							tmpNode2 = Cudd_zddChange(manager, Node[tmpList->Id].Fpath, (2*i)+1);
							Cudd_Ref(tmpNode2);

						} else {
							continue;

						}

						tmpRobust = Cudd_zddChange(manager, Node[tmpList->Id].RobustPath, 2*i);


						tmpNode3 = Cudd_zddUnion(manager, tmpNode, tmpNode2);
						Cudd_Ref(tmpNode3);
						Cudd_RecursiveDeref(manager, tmpNode);
						Cudd_RecursiveDeref(manager, tmpNode2);

						tmpNode = tmpNode3;
						//Node[i].RobustPath = Cudd_zddChange(manager, Node[tmpList->Id].Rpath, i);
						//Cudd_Ref(Node[i].RobustPath);
					}

					if(Node[i].Val == R1)
					{
						Node[i].Rpath = tmpNode;

					} else if(Node[i].Val == F0) {
						Node[i].Fpath = tmpNode;
					}

				}

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
			Cudd_RecursiveDeref(manager, robustPaths.Rpath);

			robustPaths.Rpath = tmpNode;
		} else if(Node[j].Val == F0) {
			tmpNode = Cudd_zddUnion(manager, robustPaths.Fpath, Node[j].Fpath);
			Cudd_Ref(tmpNode);
			Cudd_RecursiveDeref(manager, robustPaths.Fpath);

			robustPaths.Fpath = tmpNode;
		}
	}

	clearNodeZDDs();
}

DdNode *createZDD(LIST *pathList)
{
	DdNode *tmpPath, *tmp, *path;

	path = Cudd_zddChange(manager, onez, 0);
	Cudd_Ref(path);

	for( ; pathList != NULL; pathList = pathList->Next)
	{
		tmp = Cudd_zddChange(manager, onez, pathList->Id);
		Cudd_Ref(tmp);

		tmpPath = Cudd_zddUnion(manager, tmp, path);
		Cudd_Ref(tmpPath);
		Cudd_RecursiveDeref(manager, path);
		Cudd_RecursiveDeref(manager, tmp);

		path = tmpPath;

	}

	return path;
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

void freePathSet()
{
	int i, j;

	for(i = 0; i < Npo; i++)
	{
		for(j = 0; j < pathSet[i].numLongestPath; j++)
		{
			if(pathSet[i].longestPath[j].Path != NULL)
				FreeList(&pathSet[i].longestPath[j].Path);
		}
		for(j = 0; j < pathSet[i].numSecondLongestPath; j++)
		{
			if(pathSet[i].secondLongestPath[j].Path != NULL)
				FreeList(&pathSet[i].secondLongestPath[j].Path);
		}

		//free(pathSet[i].longestPath);
		//free(pathSet[i].secondLongestPath);
	}

	free(pathSet);
}

int compareList(LIST *a, LIST *b)
{
	while(1)
	{
		/* base case */
		if(a == NULL && b == NULL)
		{
			return 1;
		}

		if(a == NULL && b != NULL)
		{
			return 0;
		}

		if(a != NULL && b == NULL)
		{
			return 0;
		}

		if(a->Id != b->Id)
		{
			return 0;
		}

		/* If we reach here, then a and b are not NULL and their
		   data is same, so move to next nodes in both lists */
		a = a->Next;
		b = b->Next;
	}

	return 0;
}
/****************************************************************************************************************************/
