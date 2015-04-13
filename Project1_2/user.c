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
            //printInputVector(vector1);
            //printInputVector(vector2);

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

            //printf("Resulting Pattern: ");
            //printPattern(patIndex-Npi);
            //printf("\n");

            if (NULL == (patterns = (int*)realloc(patterns, (Npi+patIndex) * sizeof(int))))
            {
                printf("realloc failed\n");

            }

        }
    }

    Tpat = patIndex;
}

void initDelay(GATE *Node)
{
    int i, j, mark = 0, k;
    int *outputs = NULL;
    LIST *tmpList = NULL;
    int tmpDelay;
    PATH_COUNT *pathIter = NULL, *currPath = NULL;

    pathStack.contents = NULL;
    outputs = (int*) malloc(sizeof(int) * Npo);

    StackInit(&pathStack);

    for(i = 0, tmpDelay = 0, j = 0; i <= Tgat; i++, tmpDelay = 0)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Delay = 0;
                //printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                InsertPathCount(&Node[i].PathCount, 0, 1);
                //printf("Path at %d = %d,%d\n", i, Node[i].PathCount->Delay, Node[i].PathCount->Count);

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
                //("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

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
                    //printf("Path at %d: Delay = %d Count = %d\n", i, currPath->Delay, currPath->Count);

                }

                break;
            case FROM:
                tmpList = Node[i].Fin;

                Node[i].Delay = Node[tmpList->Id].Delay;
                //printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                for(pathIter = Node[tmpList->Id].PathCount; pathIter != NULL; pathIter = pathIter->Next)
                {
                    InsertPathCount(&Node[i].PathCount, pathIter->Delay, pathIter->Count);

                }

                for(currPath = Node[i].PathCount; currPath != NULL; currPath = currPath->Next)
                {
                    //printf("Path at %d: Delay = %d Count = %d\n", i, currPath->Delay, currPath->Count);

                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }

        if(Node[i].Fot == NULL && Node[i].Fin != NULL)
        {
            outputs[j] = i;
            j++;
        }
    }

    pathSet = (PATH_SET*) malloc(sizeof(PATH_SET) * Npo);

    for(i = 0; i < Npo; i++)
    {
        pathSet[i].Id = outputs[i];
        pathSet[i].numLongestPath = 0;
        pathSet[i].numSecondLongestPath = 0;
        pathSet[i].longestPath = NULL;
        pathSet[i].secondLongestPath = NULL;

        for(currPath = Node[outputs[i]].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(currPath->Delay == Node[outputs[i]].Delay)
                pathSet[i].numLongestPath += currPath->Count;
            else if(currPath->Delay == Node[outputs[i]].Delay - 1)
                pathSet[i].numSecondLongestPath += currPath->Count;
        }

        printf("Num longest and second longest at %d = %d %d\n", outputs[i], pathSet[i].numLongestPath, pathSet[i].numSecondLongestPath);

        pathSet[i].longestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numLongestPath);

        for(k = 0; k < pathSet[i].numLongestPath; k++)
        {
            pathSet[i].longestPath[k].Path = NULL;
        }

        pathSet[i].secondLongestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numSecondLongestPath);

        for(k = 0; k < pathSet[i].numSecondLongestPath; k++)
        {
            pathSet[i].secondLongestPath[k].Path = NULL;
        }

        for(currPath = Node[outputs[i]].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(Node[outputs[i]].Delay == currPath->Delay)
            {
                k = 0;
                StackPush(&pathStack, outputs[i]);
                buildNLongestPath(Node, 1, outputs[i], i, currPath->Delay-1, &k, pathSet[i].numLongestPath);
                StackPop(&pathStack);


            } else {
                k = 0;
                StackPush(&pathStack, outputs[i]);
                buildNLongestPath(Node, 2, outputs[i], i, currPath->Delay-1, &k, pathSet[i].numSecondLongestPath);
                StackPop(&pathStack);

            }
        }
    }

    StackDestroy(&pathStack);

    printf("\n");
}

void buildNLongestPath(GATE *Node, int n, int NodeIndex, int PathSetIndex, int currPathDelay, int *PathIndex, int numPaths)
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
                        if(pathSet[PathSetIndex].longestPath[*PathIndex].Path != NULL)
                        	printf("Path == NULL\n");

                        StackCopyToList(&pathStack, &pathSet[PathSetIndex].longestPath[*PathIndex].Path);

                        //printf("Longest Path #%d at %d = ", *PathIndex, NodeIndex);
                        //PrintList(pathSet[PathSetIndex].longestPath[*PathIndex].Path);
                        //printf("\n");
                        //fflush(stdout);

                    } else if(n == 2) {
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
                        buildNLongestPath(Node, n, tmpList->Id, PathSetIndex, finPathIter->Delay-1, PathIndex, numPaths);
                    else
                        buildNLongestPath(Node, n, tmpList->Id, PathSetIndex, finPathIter->Delay, PathIndex, numPaths);

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

void patternSim(GATE *Node)
{
    int i, j, patIndex, tmpVal;
    DdNode *RpathSet, *FpathSet, *RobustPathSet;
    DdNode *SuspectSet;
    DdNode *GoodPaths;

    GoodPaths = NULL;
    SuspectSet = NULL;
    RpathSet = NULL;
    FpathSet = NULL;
    RobustPathSet = NULL;

    initDelay(Node);

    //iterate over patterns
    for(patIndex = 0; patIndex < Tpat; printf("\n"))
    {
        printf("Applying Pattern: ");
        printPattern(patIndex);

        //topologoical traversal to apply pattern
        for(tmpVal = 0, i = 0; i <= Tgat; i++, tmpVal = 0)
        {
            applyPattern(Node, i, &patIndex, &tmpVal);

            //set mark
            if((Node[i].Val == R1) || (Node[i].Val == F0))
            {
                Node[i].Mark = 1;

            } else {
                Node[i].Mark = 0;

            }
        }

        if(verbose == 1)
        {
            for(j = 1; j <= Tgat; j++)
            {
                if(Node[j].Fot == NULL)
                    printf("Output of %s = %d\n", Node[j].Name, Node[j].Val);

            }
        }

        storeRobustPaths(Node, &RobustPathSet);

        for(i = 0; i < Npo; i++)
        {
            for(j = 0; j < pathSet[i].numLongestPath; j++)
            {
                if(checkPathSensitivity(Node, pathSet[i].longestPath[j].Path) == 1)
                {
                    storeGoodPaths(pathSet[i].longestPath[j].Path, &GoodPaths);

                } else {
                    storeSuspectPaths(pathSet[i].longestPath[j].Path, &SuspectSet);
                }
            }

            for(j = 0; j < pathSet[i].numSecondLongestPath; j++)
            {
                if(checkPathSensitivity(Node, pathSet[i].secondLongestPath[j].Path) == 1)
                {
                    storeGoodPaths(pathSet[i].secondLongestPath[j].Path, &GoodPaths);

                } else {
                    storeSuspectPaths(pathSet[i].secondLongestPath[j].Path, &SuspectSet);
                }
            }
        }

        if(GoodPaths != NULL)
            printf("Good Path ZDD Count: %d\n", Cudd_zddCount(manager, GoodPaths));

        if(SuspectSet != NULL)
            printf("Suspect Set ZDD Count: %d\n", Cudd_zddCount(manager, SuspectSet));

    }

    clearPathZDDs(&RpathSet, &FpathSet, &RobustPathSet, &GoodPaths, &SuspectSet);
}

void storeGoodPaths(LIST *pathList, DdNode **GoodPaths)
{
    DdNode *tmpGoodPaths = NULL, *tmpPath = NULL;

    if(*GoodPaths == NULL)
    {
       *GoodPaths = createZDD(pathList);

    } else {
        tmpPath = createZDD(pathList);

        tmpGoodPaths = Cudd_zddUnion(manager, tmpPath, *GoodPaths);
        Cudd_Ref(tmpGoodPaths);

        Cudd_RecursiveDerefZdd(manager, tmpPath);
        Cudd_RecursiveDerefZdd(manager, *GoodPaths);

        *GoodPaths = tmpGoodPaths;
    }
}

void storeSuspectPaths(LIST *pathList, DdNode **SuspectSet)
{
    DdNode *tmpSuspectSet = NULL, *tmpPath = NULL;

    if(*SuspectSet == NULL)
    {
        *SuspectSet = createZDD(pathList);

    } else {
        tmpPath = createZDD(pathList);

        tmpSuspectSet = Cudd_zddUnion(manager, tmpPath, *SuspectSet);
        Cudd_Ref(tmpSuspectSet);

        Cudd_RecursiveDerefZdd(manager, tmpPath);
        Cudd_RecursiveDerefZdd(manager, *SuspectSet);

        *SuspectSet = tmpSuspectSet;
    }
}

void storeRobustPaths(GATE *Node, DdNode **RobustPathSet)
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

                    Node[i].RobustPath = Cudd_zddChange(manager, onez, i);
                    Cudd_Ref(Node[i].RobustPath);

                    //printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                    //fflush(stdout);
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
                                tmpNode = NULL;

                                Cudd_RecursiveDerefZdd(manager, Node[i].RobustPath);

                                Node[i].RobustPath = tmpNode2;
                            }
                            //printf("Robust ZDD Count at %d: %d\n", i, Cudd_zddCount(manager, Node[i].RobustPath));
                            //fflush(stdout);
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
                if(*RobustPathSet == NULL)
                {
                    //*RobustPathSet = Cudd_zddChange(manager, Node[j].RobustPath, j);
                    *RobustPathSet = Node[j].RobustPath;
                    Cudd_Ref(*RobustPathSet);

                } else {
                    tmpNode3 = Cudd_zddUnion(manager, *RobustPathSet, Node[j].RobustPath);
                    Cudd_Ref(tmpNode3);
                    Cudd_RecursiveDerefZdd(manager, *RobustPathSet);
                    Cudd_RecursiveDerefZdd(manager, Node[j].RobustPath);
                    Node[j].RobustPath = NULL;

                    *RobustPathSet = tmpNode3;
                }
            }
        }
    }

    printf("Robust Path ZDD Count: %d\n", Cudd_zddCount(manager, *RobustPathSet));
    //fflush(stdout);

    clearNodeZDDs(Node);
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

void clearPathZDDs(DdNode **RpathSet, DdNode **FpathSet, DdNode **RobustPathSet, DdNode **GoodPaths, DdNode **SuspectSet)
{
    if(*RpathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *RpathSet);
        *RpathSet = NULL;
    }

    if(*FpathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *FpathSet);
        *FpathSet = NULL;
    }

    if(*GoodPaths != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *GoodPaths);
        *GoodPaths = NULL;
    }

    if(*SuspectSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *SuspectSet);
        *SuspectSet = NULL;
    }

    if(*RobustPathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *RobustPathSet);
        *RobustPathSet = NULL;
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
                    /*if(Node[i].Rpath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].Rpath);
                        Node[i].Rpath = NULL;
                    }

                    if(Node[i].Fpath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].Fpath);
                        Node[i].Fpath = NULL;
                    }*/

                    if(Node[i].RobustPath != NULL)
                    {
                        Cudd_RecursiveDerefZdd(manager, Node[i].RobustPath);
                        Node[i].RobustPath = NULL;
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

void applyPattern(GATE *Node, int i, int *patIndex, int *tmpVal)
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
/****************************************************************************************************************************/
