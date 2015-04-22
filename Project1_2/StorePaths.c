#include "StorePaths.h"

void storeSensitizedPaths(GATE *Node, DdNode **PathSet, int Tgat)
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

    clearNodeZDDs(Node, Tgat);
}

void storeSensitizedRPaths(GATE *Node, DdNode **RPathSet)
{

}

void storeSensitizedFPaths(GATE *Node, DdNode **FPathSet)
{

}

void storeLSPaths(GATE *Node, int Npo, DdNode **GoodPaths, DdNode **SuspectSet)
{
    int i, j;

    for(i = 0; i < Npo; i++)
    {
        for(j = 0; j < pathSet[i].numLongestPath; j++)
        {
            if((checkPathSensitivity(Node, pathSet[i].longestPath[j].Path) == 1))
            {
                listToZdd(pathSet[i].longestPath[j].Path, GoodPaths);

            } else if(pathSet[i].longestPath[j].suspect == 0) {
                pathSet[i].longestPath[j].suspect = 1;

                listToZdd(pathSet[i].longestPath[j].Path, SuspectSet);

            }
        }

        for(j = 0; j < pathSet[i].numSecondLongestPath; j++)
        {
            if((checkPathSensitivity(Node, pathSet[i].secondLongestPath[j].Path) == 1))
            {
                listToZdd(pathSet[i].secondLongestPath[j].Path, GoodPaths);

            } else if(pathSet[i].secondLongestPath[j].suspect == 0) {
                pathSet[i].secondLongestPath[j].suspect = 1;

                listToZdd(pathSet[i].secondLongestPath[j].Path, SuspectSet);
            }
        }
    }
}

void storeNnT(GATE *Node, DdNode **PathSet, int Tgat)
{
    int i;

    for(i = 1; i < Tgat; i++)
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
                Node[i].NnT = Cudd_zddSubset1(manager, *PathSet, i);

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}


void storeRnT(GATE *Node, DdNode **PathSet, int Tgat)
{
    int i;

    for(i = 1; i < Tgat; i++)
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
                Node[i].RnT = Cudd_zddSubset1(manager, *PathSet, i);

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void listToZdd(LIST *pathList, DdNode **PathSet)
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
