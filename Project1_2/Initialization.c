#include "Initialization.h"

stackT pathStack;

void initDelay(GATE *Node, int Npi, int Npo, int Tgat)
{
    int i, mark = 0, k;
    LIST *tmpList = NULL;
    int tmpDelay;
    PATH_COUNT *pathIter = NULL, *currPath = NULL;

    initInputOuputArrays(Node, Npi, Npo, Tgat);

    pathStack.contents = NULL;

    StackInit(&pathStack);

    for(i = 0, tmpDelay = 0; i <= Tgat; i++, tmpDelay = 0)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].Delay = 0;
                //printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                insertPathCount(&Node[i].PathCount, 0, 1);

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
                                insertPathCount(&Node[i].PathCount, pathIter->Delay + 1, pathIter->Count);

                            }
                        } else {
                            insertPathCount(&Node[i].PathCount, pathIter->Delay + 1, pathIter->Count);

                        }
                    }
                }

                break;
            case FROM:
                tmpList = Node[i].Fin;

                Node[i].Delay = Node[tmpList->Id].Delay;
                //printf("Delay at %s = %d\n", Node[i].Name, Node[i].Delay);

                for(pathIter = Node[tmpList->Id].PathCount; pathIter != NULL; pathIter = pathIter->Next)
                {
                    insertPathCount(&Node[i].PathCount, pathIter->Delay, pathIter->Count);

                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }

    pathSet = (PATH_SET*) malloc(sizeof(PATH_SET) * Npo);

    for(i = 0; i < Npo; i++)
    {
        pathSet[i].Id = primaryOutputs[i];
        pathSet[i].numLongestPath = 0;
        pathSet[i].numSecondLongestPath = 0;
        pathSet[i].longestPath = NULL;
        pathSet[i].secondLongestPath = NULL;

        for(currPath = Node[primaryOutputs[i]].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(currPath->Delay == Node[primaryOutputs[i]].Delay)
                pathSet[i].numLongestPath += currPath->Count;
            else if(currPath->Delay == Node[primaryOutputs[i]].Delay - 1)
                pathSet[i].numSecondLongestPath += currPath->Count;
        }

        printf("Num longest and second longest at %d = %d %d\n", primaryOutputs[i], pathSet[i].numLongestPath, pathSet[i].numSecondLongestPath);

        pathSet[i].longestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numLongestPath);

        for(k = 0; k < pathSet[i].numLongestPath; k++)
        {
            pathSet[i].longestPath[k].Path = NULL;
            pathSet[i].longestPath[k].suspect = 0;
        }

        pathSet[i].secondLongestPath = (PATH*)malloc(sizeof(PATH) * pathSet[i].numSecondLongestPath);

        for(k = 0; k < pathSet[i].numSecondLongestPath; k++)
        {
            pathSet[i].secondLongestPath[k].Path = NULL;
            pathSet[i].secondLongestPath[k].suspect = 0;
        }

        for(currPath = Node[primaryOutputs[i]].PathCount; currPath != NULL; currPath = currPath->Next)
        {
            if(Node[primaryOutputs[i]].Delay == currPath->Delay)
            {
                k = 0;
                StackPush(&pathStack, primaryOutputs[i]);
                buildNLongestPath(Node, 1, primaryOutputs[i], i, currPath->Delay-1, &k, pathSet[i].numLongestPath);
                StackPop(&pathStack);


            } else {
                k = 0;
                StackPush(&pathStack, primaryOutputs[i]);
                buildNLongestPath(Node, 2, primaryOutputs[i], i, currPath->Delay-1, &k, pathSet[i].numSecondLongestPath);
                StackPop(&pathStack);

            }
        }
    }

    StackDestroy(&pathStack);

    printf("\n");
}

void initInputOuputArrays(GATE *Node, int Npi, int Npo, int Tgat)
{
    int i, j, k;

    primaryOutputs = (int*) malloc(sizeof(int) * Npo);
    primaryInputs = (int*) malloc(sizeof(int) * Npi);

    for(i = 0, j = 0, k = 0; i <= Tgat; i++)
    {
        if(Node[i].Fot != NULL && Node[i].Fin == NULL)
        {
            primaryInputs[k] = i;
            k++;
        }

        if(Node[i].Fot == NULL && Node[i].Fin != NULL)
        {
            primaryOutputs[j] = i;
            j++;
        }
    }
}

void freeInputOutputArrays()
{
    if(primaryOutputs != NULL)
        free(primaryOutputs);

    if(primaryInputs != NULL)
        free(primaryInputs);
}

void insertPathCount(PATH_COUNT **Cur, int delay, int count)
{
    PATH_COUNT *tl= NULL;
    PATH_COUNT *nl= NULL;

    if ((tl =(PATH_COUNT *) malloc(sizeof(PATH_COUNT))) == NULL){
        printf("PATH_COUNT: Out of memory\n");
        exit(1);
    } else {
        tl->Next = NULL;
        tl->Delay = delay;
        tl->Count = count;

        if(*Cur == NULL)
        {
            *Cur = tl;
            return;
        }

        nl = *Cur;

        while(nl!= NULL)
        {
            if(nl->Count == count && nl->Delay == delay)
            {
                break;
            }
            if(nl->Next == NULL)
            {
                nl->Next = tl;
            }

            nl = nl->Next;
        }
    }

    return;
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

void freePathSet(int Npo)
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
