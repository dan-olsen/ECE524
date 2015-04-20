#include "given.h"
#include "user.h"

const int robustSimOR [6][6]        =   {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, X1, S1, X1, X1},
                                         {F0, X1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, X1, F0, S1, X1, X0}};

const int nonRobustSimOR [6][6]     =   {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, R1, S1, X1, R1},
                                         {F0, R1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, R1, F0, S1, X1, X0}};

const int robustSimAND [6][6]       =   {{S0, S0, S0, S0, S0, S0},
                                         {S0, R1, X0, R1, R1, X0},
                                         {S0, X0, X0, F0, X0, X0},
                                         {S0, R1, F0, S1, X1, X0},
                                         {S0, R1, X0, X1, X1, X0},
                                         {S0, X0, X0, X0, X0, X0}};

const int nonRobustSimAND [6][6]    =   {{S0, S0, S0, S0, S0, S0},
                                         {S0, R1, F0, R1, R1, X0},
                                         {S0, F0, X0, F0, F0, X0},
                                         {S0, R1, F0, S1, X1, X0},
                                         {S0, R1, F0, X1, X1, X0},
                                         {S0, X0, X0, X0, X0, X0}};

const int simNOT [6]                =   {S1, F0, R1, S0, X0, X1};

int* getNextPattern(FILE **patFile)
{
    int patIndex;
    char vector1 [Mpi];
    char vector2  [Mpi];
    int readCount1, readCount2;
    int *pattern;

    if ( NULL == (pattern =  (int*)malloc(Npi * sizeof(int)))) {
        printf("malloc failed\n");
        //error
    }

    if(!feof(*patFile))
    {
        readCount1 = fscanf(*patFile, "%s", vector1);
        readCount2 = fscanf(*patFile, "%s", vector2);

        if((readCount2 != -1) && (readCount1 != -1) && (readCount1 == readCount2))
        {
            printInputVector(vector1);
            printInputVector(vector2);

            for(patIndex = 0; patIndex < Npi; patIndex++)
            {
                if((vector1[patIndex] == '0') && (vector2[patIndex] == '0'))
                {
                    pattern[patIndex] = S0;

                } else if((vector1[patIndex] == '0') && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = R1;

                } else if((vector1[patIndex] == '1') && (vector2[patIndex] == '0')) {
                    pattern[patIndex] = F0;

                } else if((vector1[patIndex] == '1') && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = S1;

                } else if(((vector1[patIndex] == 'x') || (vector1[patIndex] == 'X')) && (vector2[patIndex] == '1')) {
                    pattern[patIndex] = X1;

                } else if(((vector1[patIndex] == 'x') || (vector1[patIndex] == 'X')) && (vector2[patIndex] == '0')) {
                    pattern[patIndex] = X0;

                } else {
                    //error
                    continue;

                }
            }

            printf("Resulting Pattern: ");
            printPattern(pattern);
            printf("\n");

        } else {
            printf("End of Pattern File\n");
            free(pattern);
            return NULL;
        }
    } else {
        printf("End of Pattern File\n");
        free(pattern);
        return NULL;
    }

    return pattern;
}

void patternSim(GATE *Node, FILE *patFile)
{
    int i, j;
    int *pattern = NULL;
    DdNode *RobustRpathSet = NULL, *RobustFpathSet = NULL, *RobustPathSet = NULL;
    DdNode *SuspectSet = NULL;
    DdNode *GoodPaths = NULL;
    DdNode *tmpNode = NULL;

    InitDelay(Node);

    //iterate over patterns
    while((pattern = getNextPattern(&patFile)) != NULL)
    {
        printf("Applying Pattern: ");
        printPattern(pattern);
        printf("\n");

        //topologoical traversal to apply pattern
        applyPattern(Node, i, pattern);

        free(pattern);

        storePaths(Node, &RobustPathSet);
        storeRPaths(Node, &RobustRpathSet);
        storeFPaths(Node, &RobustFpathSet);

        for(i = 0; i < Npo; i++)
        {
            for(j = 0; j < pathSet[i].numLongestPath; j++)
            {
                if((checkPathSensitivity(Node, pathSet[i].longestPath[j].Path) == 1))
                {
                    ListToZdd(pathSet[i].longestPath[j].Path, &GoodPaths);

                } else if(pathSet[i].longestPath[j].suspect == 0) {
                    pathSet[i].longestPath[j].suspect = 1;

                    ListToZdd(pathSet[i].longestPath[j].Path, &SuspectSet);

                }
            }

            for(j = 0; j < pathSet[i].numSecondLongestPath; j++)
            {
                if((checkPathSensitivity(Node, pathSet[i].secondLongestPath[j].Path) == 1))
                {
                    ListToZdd(pathSet[i].secondLongestPath[j].Path, &GoodPaths);

                } else if(pathSet[i].secondLongestPath[j].suspect == 0) {
                    pathSet[i].secondLongestPath[j].suspect = 1;

                    ListToZdd(pathSet[i].secondLongestPath[j].Path, &SuspectSet);
                }
            }
        }
	}

    if(RobustPathSet != NULL)
    	printf("Robust Path ZDD Count: %d\n", Cudd_zddCount(manager, RobustPathSet));

    if(GoodPaths != NULL)
         printf("Good Path ZDD Count: %d\n", Cudd_zddCount(manager, GoodPaths));

     if(SuspectSet != NULL) {
    	 tmpNode = Cudd_zddDiff(manager, SuspectSet, GoodPaths);
    	 Cudd_Ref(tmpNode);
    	 Cudd_RecursiveDerefZdd(manager, SuspectSet);

    	 SuspectSet = tmpNode;

         printf("Suspect Set ZDD Count: %d\n", Cudd_zddCount(manager, SuspectSet));
     }

    clearPathZDDs(&RobustRpathSet);
    clearPathZDDs(&RobustFpathSet);
	clearPathZDDs(&RobustPathSet);
	clearPathZDDs(&GoodPaths);
	clearPathZDDs(&SuspectSet);
}

void ListToZdd(LIST *pathList, DdNode **PathSet)
{
    DdNode *tmpGoodPaths = NULL, *tmpPath = NULL;

    if(*PathSet == NULL)
    {
       *PathSet = createZDD(pathList);

    } else {
        tmpPath = createZDD(pathList);

        tmpGoodPaths = Cudd_zddUnion(manager, tmpPath, *PathSet);
        Cudd_Ref(tmpGoodPaths);

        Cudd_RecursiveDerefZdd(manager, tmpPath);
        Cudd_RecursiveDerefZdd(manager, *PathSet);

        *PathSet = tmpGoodPaths;
    }
}

void storePaths(GATE *Node, DdNode **PathSet)
{
    int i, j;
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL, *tmpNode3 = NULL;

    for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++, tmpList = Node[i].Fin)
    {
        switch(Node[i].Type) {
            case INPT:
                if(Node[i].Mark == 1)
                {
                    Node[i].TempPath = Cudd_zddChange(manager, onez, i);
                    Cudd_Ref(Node[i].TempPath);

                }

                break;
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
                if(Node[i].Mark == 1)
                {
                    for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                    {
                        if(Node[tmpList->Id].Mark == 1)
                        {
                            if(Node[i].TempPath == NULL)
                            {
                                Node[i].TempPath = Cudd_zddChange(manager, Node[tmpList->Id].TempPath, i);
                                Cudd_Ref(Node[i].TempPath);

                            } else {
                                tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempPath, i);
                                Cudd_Ref(tmpNode);
                            }

                            if(tmpNode != NULL)
                            {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempPath);
                                Cudd_Ref(tmpNode2);

                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                tmpNode = NULL;

                                Cudd_RecursiveDerefZdd(manager, Node[i].TempPath);

                                Node[i].TempPath = tmpNode2;
                            }
                        }
                    }
                }

                break;
            case BUFF:
            case NOT:
            case FROM:
                tmpList = Node[i].Fin;

                if(Node[i].Mark == 1)
                {
                    if(Node[tmpList->Id].Mark == 1)
                    {
                        Node[i].TempPath = Cudd_zddChange(manager, Node[tmpList->Id].TempPath, i);
                        Cudd_Ref(Node[i].TempPath);

                        //printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                        //fflush(stdout);
                    }
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }

    }

    for(j = 1; j <= Tgat; j++)
    {
        if(Node[j].Fot == NULL)
        {
            if(Node[j].Mark == 1)
            {
                if(*PathSet == NULL)
                {
                    *PathSet = Node[j].TempPath;
                    Cudd_Ref(*PathSet);

                } else {
                    tmpNode3 = Cudd_zddUnion(manager, *PathSet, Node[j].TempPath);
                    Cudd_Ref(tmpNode3);
                    Cudd_RecursiveDerefZdd(manager, *PathSet);
                    Cudd_RecursiveDerefZdd(manager, Node[j].TempPath);
                    Node[j].TempPath = NULL;

                    *PathSet = tmpNode3;
                }
            }
        }
    }

    clearNodeZDDs(Node);
}

void storeRPaths(GATE *Node, DdNode **RPathSet)
{

}

void storeFPaths(GATE *Node, DdNode **FPathSet)
{

}

DdNode* createZDD(LIST *pathList)
{
    DdNode *tmp = NULL;
    DdNode *path = NULL;

    path = Cudd_zddChange(manager, onez, pathList->Id);
    Cudd_Ref(path);
    pathList = pathList->Next;

    for( ; pathList != NULL; pathList = pathList->Next)
    {
        tmp = Cudd_zddChange(manager, path, pathList->Id);
        Cudd_Ref(tmp);
        Cudd_RecursiveDerefZdd(manager, path);

        path = tmp;
    }

    return path;
}

void clearPathZDDs(DdNode **PathSet)
{
    if(*PathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *PathSet);
        *PathSet = NULL;
    }
}

void clearNodeZDDs(GATE *Node)
{
    int i;

    for(i = 0; i <= Tgat; i++)
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
                if(Node[i].Mark == 1)
                {
                    //ff("Recursive Deref %d\n", i);
                    if(Node[i].TempRpath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].TempRpath);
                        Node[i].TempRpath = NULL;
                    }

                    if(Node[i].TempFpath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].TempFpath);
                        Node[i].TempFpath = NULL;
                    }

                    if(Node[i].TempPath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].TempPath);
                        Node[i].TempPath = NULL;
                    }
                }

                break;

            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void applyPattern(GATE *Node, int i, int *pattern)
{
    LIST *tmpList = NULL;
    int tmpVal = 0;
    int patIndex;

    for(i = 1, patIndex = 0; i <= Tgat; i++)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Val = pattern[patIndex];

                patIndex++;

                break;
            case AND:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(tmpList == Node[i].Fin)
                    {
                        tmpVal = Node[tmpList->Id].Val;

                        continue;
                    }

                    tmpVal = robustSimAND[tmpVal][Node[tmpList->Id].Val];
                }

                Node[i].Val = tmpVal;

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

        //set mark
        if((Node[i].Val == R1) || (Node[i].Val == F0))
        {
            Node[i].Mark = 1;

        } else {
            Node[i].Mark = 0;

        }
    }
}

int checkPathSensitivity(GATE *Node, LIST *path)
{
    for( ; path != NULL; path = path->Next)
    {
        if(Node[path->Id].Mark == 0)
        {
            return 0;
        }
    }

    return 1;
}
