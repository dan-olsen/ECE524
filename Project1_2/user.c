#include "given.h"
#include "user.h"

const int robustSimOR [6][6]         =    {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, X1, S1, X1, X1},
                                         {F0, X1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, X1, F0, S1, X1, X0}};

const int nonRobustSimOR [6][6]        =     {{S0, R1, F0, S1, X1, X0},
                                         {R1, X1, R1, S1, X1, R1},
                                         {F0, R1, F0, S1, X1, F0},
                                         {S1, S1, S1, S1, S1, S1},
                                         {X1, X1, X1, S1, X1, X1},
                                         {X0, R1, F0, S1, X1, X0}};

const int robustSimAND [6][6]         =     {{S0, S0, S0, S0, S0, S0},
                                         {S0, R1, X0, R1, R1, X0},
                                         {S0, X0, X0, F0, X0, X0},
                                         {S0, R1, F0, S1, X1, X0},
                                         {S0, R1, X0, X1, X1, X0},
                                         {S0, X0, X0, X0, X0, X0}};

const int nonRobustSimAND [6][6]    =     {{S0, S0, S0, S0, S0, S0},
                                           {S0, R1, F0, R1, R1, X0},
                                           {S0, F0, X0, F0, F0, X0},
                                           {S0, R1, F0, S1, X1, X0},
                                           {S0, R1, F0, X1, X1, X0},
                                           {S0, X0, X0, X0, X0, X0}};

const int simNOT [6]                 =      {S1, F0, R1, S0, X0, X1};

void applyPattern(int i, int *patIndex, int *tmpVal);
void printInputVector(char *input);
void printPattern(int patIndex);
void initDelay();
void createZDD(LIST *pathList, DdNode **path);
void buildNLongestPath(int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths);
int compareList(LIST *a, LIST *b);
void InsertPathCount(PATH_COUNT **Cur, int delay, int count);
int checkPathSensitivity(LIST *path);
void storeGoodPaths(LIST *pathList);
void storeSuspectPaths(LIST *pathList);


void readPatternFile(FILE* patFile)
{
    int patColIndex, patIndex;
    char vector1 [Mpi];
    char vector2  [Mpi];
    int readCount1, readCount2;

    if ( NULL == (patterns =  (int*)malloc(Npi * sizeof(int)))) {
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

            if (NULL == (patterns = (int*)realloc(patterns, (Npi+patIndex) * sizeof(int))))
            {
                printf("realloc failed\n");

            }

        }
    }

    Tpat = patIndex;
}

void initDelay()
{
    int i, j, mark = 0, k;
    LIST *tmpList = NULL;
    int tmpDelay;
    PATH_COUNT *pathIter = NULL, *currPath = NULL;

    pathStack.contents = NULL;

    StackInit(&pathStack);

    for(i = 0, tmpDelay = 0; i <= Tgat; i++, tmpDelay = 0)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Delay = 0;
                printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                InsertPathCount(&Node[i].PathCount, 0, 1);
                printf("Path at %d = %d,%d\n", i, Node[i].PathCount->Delay, Node[i].PathCount->Count);

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
                    for(pathIter = Node[tmpList->Id].PathCount; pathIter != NULL; pathIter = pathIter->Next)
                    {
                        if(Node[i].PathCount != NULL)
                        {
                            for(currPath = Node[i].PathCount; currPath != NULL; currPath = currPath->Next)
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
                                InsertPathCount(&Node[i].PathCount, pathIter->Delay + 1, pathIter->Count);

                            }
                        } else {
                            InsertPathCount(&Node[i].PathCount, pathIter->Delay + 1, pathIter->Count);

                        }
                    }
                }

                for(currPath = Node[i].PathCount; currPath != NULL; currPath = currPath->Next)
                {
                    printf("Path at %d: Delay = %d Count = %d\n", i, currPath->Delay, currPath->Count);

                }

                break;
            case FROM:
                tmpList = Node[i].Fin;

                Node[i].Delay = Node[tmpList->Id].Delay;
                printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                for(pathIter = Node[tmpList->Id].PathCount; pathIter != NULL; pathIter = pathIter->Next)
                {
                    InsertPathCount(&Node[i].PathCount, pathIter->Delay, pathIter->Count);

                }

                for(currPath = Node[i].PathCount; currPath != NULL; currPath = currPath->Next)
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
    /*
    pathSet = (PATH_SET*) malloc(sizeof(PATH_SET) * Npo);

    for(i = 0, j = Tgat-Npo+1; i < Npo; i++, j++)
    {
        pathSet[i].Id = j;
        pathSet[i].numLongestPath = 0;
        pathSet[i].numSecondLongestPath = 0;
        pathSet[i].longestPath = NULL;
        pathSet[i].secondLongestPath = NULL;

        for(currPath = Node[j].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(currPath->Delay == Node[j].Delay)
                pathSet[i].numLongestPath += currPath->Count;
            else if(currPath->Delay == Node[j].Delay - 1)
                pathSet[i].numSecondLongestPath += currPath->Count;
        }

        printf("Num longest and second longest at %d = %d %d\n", j, pathSet[i].numLongestPath, pathSet[i].numSecondLongestPath);

        pathSet[i].longestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numLongestPath);

        for(k = 0; k < pathSet[i].numLongestPath; k++)
        {
            pathSet[i].longestPath[k].Path = NULL;
        }

        //pathSet[i].secondLongestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numSecondLongestPath);

        for(k = 0; k < pathSet[i].numSecondLongestPath; k++)
        {
            //pathSet[i].secondLongestPath[k].Path = NULL;
        }

        for(currPath = Node[j].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(Node[j].Delay == currPath->Delay)
            {
                k = 0;
                StackPush(&pathStack, j);
                buildNLongestPath(1, j, i, currPath->Delay-1, &k, pathSet[i].numLongestPath);
                StackPop(&pathStack);


            } else {
                k = 0;
                StackPush(&pathStack, j);
                //buildNLongestPath(2, j, i, currPath->Delay-1, &k, pathSet[i].numSecondLongestPath);
                StackPop(&pathStack);

            }
        }
    }*/

    StackDestroy(&pathStack);

    printf("\n");
}

void buildNLongestPath(int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths)
{
    LIST *tmpList;
    PATH_COUNT *finPathIter = NULL;

    for(tmpList = Node[NodeIndex].Fin; tmpList != NULL; tmpList = tmpList->Next)
    {
        for(finPathIter = Node[tmpList->Id].PathCount; finPathIter != NULL; finPathIter = finPathIter->Next)
        {
            if(currPathDelay == finPathIter->Delay)
            {
                StackPush(&pathStack, tmpList->Id);

                if(Node[tmpList->Id].Type == INPT)
                {
                    if(n == 1)
                    {
                        PrintStack(&pathStack);

                        if(pathSet[PathSetIndex].longestPath[*PathIndex].Path != NULL)
                        	printf("Path == NULL\n");

                        StackCopyToList(&pathStack, &pathSet[PathSetIndex].longestPath[*PathIndex].Path);

                        printf("Longest Path #%d at %d = ", *PathIndex, NodeIndex);
                        PrintList(pathSet[PathSetIndex].longestPath[*PathIndex].Path);
                        printf("\n");
                        fflush(stdout);

                    } else if(n == 2) {
                        PrintStack(&pathStack);

                        StackCopyToList(&pathStack, &pathSet[PathSetIndex].secondLongestPath[*PathIndex].Path);

                        //printf("Second LongestPath Path #%d at %d = ", *PathIndex, NodeIndex);
                        //PrintList(pathSet[PathSetIndex].secondLongestPath[*PathIndex].Path);
                        //printf("\n");
                        //fflush(stdout);
                    }



                    (*PathIndex)++;
                    StackPop(&pathStack);

                } else {
                    if(Node[tmpList->Id].Type != FROM)
                        buildNLongestPath(n, tmpList->Id, PathSetIndex, finPathIter->Delay-1, PathIndex, numPaths);
                    else
                        buildNLongestPath(n, tmpList->Id, PathSetIndex, finPathIter->Delay, PathIndex, numPaths);

                    StackPop(&pathStack);

                }

                if(*PathIndex >= numPaths)
                {
                    return;
                }
            }
        }
    }
}

void patternSim()
{
    int i, j, patIndex, tmpVal;

    initDelay();

    GoodPaths = NULL;
    SuspectSet = NULL;
    RpathSet = NULL;
    FpathSet = NULL;
    RobustPathSet = NULL;

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

        //printf("No of Unreferenced Zdds before robust: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager

        storeRobustPaths();

        //printf("No of Unreferenced Zdds after robust: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager
        /*
        for(i = 0; i < Npo; i++)
        {
            for(j = 0; j < pathSet[i].numLongestPath; j++)
            {
                if(checkPathSensitivity(pathSet[i].longestPath[j].Path) == 1)
                {
                    storeGoodPaths(pathSet[i].longestPath[j].Path);

                } else {
                    storeSuspectPaths(pathSet[i].longestPath[j].Path);
                }
            }

            for(j = 0; j < pathSet[i].numSecondLongestPath; j++)
            {
                if(checkPathSensitivity(pathSet[i].secondLongestPath[j].Path) == 1)
                {
                    storeGoodPaths(pathSet[i].secondLongestPath[j].Path);

                } else {
                    storeSuspectPaths(pathSet[i].secondLongestPath[j].Path);
                }
            }
        }

        if(goodPaths != NULL)
            printf("Good Path ZDD Count: %d\n", Cudd_zddCount(manager, goodPaths));

        if(suspectSet != NULL)
            printf("Suspect Set ZDD Count: %d\n", Cudd_zddCount(manager, suspectSet));
        */
    }
}

void storeGoodPaths(LIST *pathList)
{
    DdNode *tmpGoodPaths = NULL, *tmpPath = NULL;

    if(GoodPaths == NULL)
    {
        createZDD(pathList, &GoodPaths);

    } else {
        createZDD(pathList, &tmpPath);

        tmpGoodPaths = Cudd_zddUnion(manager, tmpPath, GoodPaths);
        Cudd_Ref(tmpGoodPaths);

        Cudd_RecursiveDerefZdd(manager, tmpPath);
        Cudd_RecursiveDerefZdd(manager, GoodPaths);

        GoodPaths = tmpGoodPaths;
    }
}

void storeSuspectPaths(LIST *pathList)
{
    DdNode *tmpSuspectSet = NULL, *tmpPath = NULL;

    if(SuspectSet == NULL)
    {
        createZDD(pathList, &SuspectSet);

    } else {
        createZDD(pathList, &tmpPath);

        tmpSuspectSet = Cudd_zddUnion(manager, tmpPath, SuspectSet);
        Cudd_Ref(tmpSuspectSet);

        Cudd_RecursiveDerefZdd(manager, tmpPath);
        Cudd_RecursiveDerefZdd(manager, SuspectSet);

        SuspectSet = tmpSuspectSet;
    }
}

void storeRobustPathsRF()
{

}

void storeRobustPaths()
{
    int i, j;
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL, *tmpNode3 = NULL;
    //printf("No of Unreferenced Zdds start robust: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager

    for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++, tmpList = Node[i].Fin)
    {
        switch(Node[i].Type) {
            case INPT:
                if(Node[i].Mark == 1)
                {

                    Node[i].RobustPath = Cudd_zddChange(manager, onez, i);
                    Cudd_Ref(Node[i].RobustPath);

                    printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                    fflush(stdout);
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
                            if(Node[i].RobustPath == NULL)
                            {
                                Node[i].RobustPath = Cudd_zddChange(manager, Node[tmpList->Id].RobustPath, i);
                                Cudd_Ref(Node[i].RobustPath);

                            } else {
                                tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].RobustPath, i);
                                Cudd_Ref(tmpNode);
                            }

                            if(tmpNode != NULL)
                            {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].RobustPath);
                                Cudd_Ref(tmpNode2);
                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                Cudd_RecursiveDerefZdd(manager, Node[i].RobustPath);

                                Node[i].RobustPath = tmpNode2;
                            }
                            printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                            fflush(stdout);
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
                        Node[i].RobustPath = Cudd_zddChange(manager, Node[tmpList->Id].RobustPath, i);
                        Cudd_Ref(Node[i].RobustPath);

                        printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                        fflush(stdout);
                    }
                }


                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }

    }
    //printf("No of Unreferenced Zdds after topo: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager


    for(j = Tgat-Npo+1; j <= Tgat; j++)
    {
        if(Node[j].Mark == 1)
        {
            if(RobustPathSet == NULL)
            {
                RobustPathSet = Node[j].RobustPath;
            } else {
                tmpNode3 = Cudd_zddUnion(manager, RobustPathSet, Node[j].RobustPath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, RobustPathSet);
                Cudd_RecursiveDerefZdd(manager, Node[j].RobustPath);

                RobustPathSet = tmpNode3;
            }
        }
    }
    //printf("No of Unreferenced Zdds after output: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager

    printf("Robust Path ZDD Count: %d\n", Cudd_zddCount(manager, RobustPathSet));
    fflush(stdout);


    //printf("No of Unreferenced Zdds before clear: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager

    clearNodeZDDs();
    //printf("No of Unreferenced Zdds after clear: %d\n", Cudd_CheckZeroRef(manager));    //Checking any unreferenced bdds in manager

}

void createZDD(LIST *pathList, DdNode **path)
{
    DdNode *tmp = NULL;

    *path = Cudd_zddChange(manager, onez, pathList->Id);
    Cudd_Ref(*path);
    pathList = pathList->Next;

    for( ; pathList != NULL; pathList = pathList->Next)
    {
        tmp = Cudd_zddChange(manager, *path, pathList->Id);
        Cudd_Ref(tmp);
        Cudd_RecursiveDerefZdd(manager, *path);

        *path = tmp;
    }
}

void clearPathZDDs()
{
    fflush(stdout);

    if(RpathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, RpathSet);
        RpathSet = NULL;
    }

    if(FpathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, FpathSet);
        FpathSet = NULL;
    }

    if(RobustPathSet != NULL)
    {
        //Cudd_RecursiveDerefZdd(manager, RobustPathSet);
        //RobustPathSet = NULL;
    }

    if(GoodPaths != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, GoodPaths);
        GoodPaths = NULL;
    }

    if(SuspectSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, SuspectSet);
        SuspectSet = NULL;
    }

}

void clearNodeZDDs()
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
                //ff("Recursive Deref %d\n", i);
                if(Node[i].Rpath != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].Rpath);
                    Node[i].Rpath = NULL;
                }

                if(Node[i].Fpath != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].Fpath);
                    Node[i].Fpath = NULL;
                }

                if(Node[i].RobustPath != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].RobustPath);
                    Node[i].RobustPath = NULL;
                }


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

        free(pathSet[i].longestPath);
        free(pathSet[i].secondLongestPath);
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

void InsertPathCount(PATH_COUNT **Cur, int delay, int count)
{
    PATH_COUNT *tl=NULL;
    PATH_COUNT *nl=NULL;

    if ((tl=(PATH_COUNT *) malloc(sizeof(PATH_COUNT)))==NULL){
        printf("PATH_COUNT: Out of memory\n");
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
            if(nl->Count==count && nl->Delay == delay)
            {
                break;
            }
            if(nl->Next==NULL)
            {
                nl->Next=tl;
            }

            nl=nl->Next;
        }
    }

    return;
}

int checkPathSensitivity(LIST *path)
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
/****************************************************************************************************************************/
