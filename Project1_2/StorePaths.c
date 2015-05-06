#include "StorePaths.h"

void setValidateMark(GATE* Node, int i);

void extractPDFs(GATE *Node, DdNode **PathSet, int Npo, int Tgat)
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
                        if(Node[tmpList->Id].Mark == 1 && Node[tmpList->Id].TempPath != NULL)
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
                    if(Node[tmpList->Id].Mark == 1 && Node[tmpList->Id].TempPath != NULL)
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

    for(j = 0; j < Npo; j++)
    {
        if(Node[primaryOutputs[j]].Mark == 1 && Node[primaryOutputs[j]].TempPath != NULL)
        {
            if(*PathSet == NULL)
            {
                *PathSet = Node[primaryOutputs[j]].TempPath;
                Cudd_Ref(*PathSet);

            } else {
                tmpNode3 = Cudd_zddUnion(manager, *PathSet, Node[primaryOutputs[j]].TempPath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, *PathSet);
                Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[j]].TempPath);
                Node[primaryOutputs[j]].TempPath = NULL;

                *PathSet = tmpNode3;
            }
        }
    }

    clearNodeZDDs(Node, Tgat);
}

void extractRPDFs(GATE *Node, DdNode **RPathSet, int Npo, int Tgat)
{
    int i, j;
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL, *tmpNode3 = NULL;

    for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++, tmpList = Node[i].Fin)
    {
        switch(Node[i].Type) {
            case INPT:
                if(Node[i].Val == R1)
                {
                    Node[i].TempRpath = Cudd_zddChange(manager, onez, 2*i);
                    Cudd_Ref(Node[i].TempRpath);

                }

                break;
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
                if(Node[i].Val == R1)
                {
                    for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                    {
                        if(Node[tmpList->Id].Val == R1 && Node[tmpList->Id].TempRpath != NULL)
                        {
                            if(Node[i].TempRpath == NULL)
                            {
                                Node[i].TempRpath = Cudd_zddChange(manager, Node[tmpList->Id].TempRpath, 2*i);
                                Cudd_Ref(Node[i].TempRpath);

                            } else {
                                tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempRpath, 2*i);
                                Cudd_Ref(tmpNode);
                            }

                            if(tmpNode != NULL)
                            {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempRpath);
                                Cudd_Ref(tmpNode2);

                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                tmpNode = NULL;

                                Cudd_RecursiveDerefZdd(manager, Node[i].TempRpath);

                                Node[i].TempRpath = tmpNode2;
                            }
                        }
                    }
                }

                break;
            case BUFF:
            case NOT:
            case FROM:
                tmpList = Node[i].Fin;

                if(Node[i].Val == R1)
                {
                    if(Node[tmpList->Id].Val == R1 && Node[tmpList->Id].TempRpath != NULL)
                    {
                        Node[i].TempRpath = Cudd_zddChange(manager, Node[tmpList->Id].TempRpath, 2*i);
                        Cudd_Ref(Node[i].TempRpath);

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

    for(j = 0; j < Npo; j++)
    {
        if(Node[primaryOutputs[j]].Val == R1 && Node[primaryOutputs[j]].TempRpath != NULL)
        {
            if(*RPathSet == NULL)
            {
                *RPathSet = Node[primaryOutputs[j]].TempRpath;
                Cudd_Ref(*RPathSet);

            } else {
                tmpNode3 = Cudd_zddUnion(manager, *RPathSet, Node[primaryOutputs[j]].TempRpath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, *RPathSet);
                Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[j]].TempRpath);
                Node[primaryOutputs[j]].TempRpath = NULL;

                *RPathSet = tmpNode3;
            }
        }
    }

    clearNodeZDDs(Node, Tgat);
}

void extractFPDFs(GATE *Node, DdNode **FPathSet, int Npo, int Tgat)
{
    int i, j;
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL, *tmpNode3 = NULL;

    for(i = 0, tmpList = Node[i].Fin; i <= Tgat; i++, tmpList = Node[i].Fin)
    {
        switch(Node[i].Type) {
            case INPT:
                if(Node[i].Val == F0)
                {
                    Node[i].TempFpath = Cudd_zddChange(manager, onez, (2*i)+1);
                    Cudd_Ref(Node[i].TempFpath);

                }

                break;
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
                if(Node[i].Val == F0)
                {
                    for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                    {
                        if(Node[tmpList->Id].Val == F0 && Node[tmpList->Id].TempFpath != NULL)
                        {
                            if(Node[i].TempFpath == NULL)
                            {
                                Node[i].TempFpath = Cudd_zddChange(manager, Node[tmpList->Id].TempFpath, (2*i)+1);
                                Cudd_Ref(Node[i].TempFpath);

                            } else {
                                tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempFpath, (2*i)+1);
                                Cudd_Ref(tmpNode);
                            }

                            if(tmpNode != NULL)
                            {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempFpath);
                                Cudd_Ref(tmpNode2);

                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                tmpNode = NULL;

                                Cudd_RecursiveDerefZdd(manager, Node[i].TempFpath);

                                Node[i].TempFpath = tmpNode2;
                            }
                        }
                    }
                }

                break;
            case BUFF:
            case NOT:
            case FROM:
                tmpList = Node[i].Fin;

                if(Node[i].Val == F0)
                {
                    if(Node[tmpList->Id].Val == F0 && Node[tmpList->Id].TempFpath != NULL)
                    {
                        Node[i].TempFpath = Cudd_zddChange(manager, Node[tmpList->Id].TempFpath, (2*i)+1);
                        Cudd_Ref(Node[i].TempFpath);

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

    for(j = 0; j < Npo; j++)
    {
        if(Node[primaryOutputs[j]].Val == F0 && Node[primaryOutputs[j]].TempFpath != NULL)
        {
            if(*FPathSet == NULL)
            {
                *FPathSet = Node[primaryOutputs[j]].TempFpath;
                Cudd_Ref(*FPathSet);

            } else {
                tmpNode3 = Cudd_zddUnion(manager, *FPathSet, Node[primaryOutputs[j]].TempFpath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, *FPathSet);
                Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[j]].TempFpath);
                Node[primaryOutputs[j]].TempFpath = NULL;

                *FPathSet = tmpNode3;
            }
        }
    }

    clearNodeZDDs(Node, Tgat);
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

    for(i = 0; i <= Tgat; i++)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].NnT = Cudd_zddSubset1(manager, *PathSet, i);
                Cudd_Ref(Node[i].NnT);

                break;
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
                Cudd_Ref(Node[i].NnT);

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
    int ZddCount = 0;
    DdNode *tmpNode = NULL;
    DdNode *tmpNode2 = NULL;
    DdNode *tmpNode3 = NULL;
    LIST *tmpList = NULL;

    for(i = 1; i <= Tgat; i++)
    {
        switch(Node[i].Type) {
            case INPT:
                tmpNode = Cudd_zddSubset1(manager, *PathSet, 2*i+1);
                Cudd_Ref(tmpNode);

                ZddCount = Cudd_zddCount(manager, tmpNode);

                printf("Temp RnT Zdd Count at i = %d 2*%d+1(%d) = %d\n", i, i, 2*i+1, ZddCount);
                Cudd_zddPrintDebug(manager, tmpNode, 0, 3);

                tmpNode2 = Cudd_zddSubset1(manager, *PathSet, 2*i);
                Cudd_Ref(tmpNode2);

                ZddCount = Cudd_zddCount(manager, tmpNode2);

                printf("Temp RnT Zdd Count at i = %d 2*%d(%d) = %d\n", i, i, 2*i, ZddCount);
                Cudd_zddPrintDebug(manager, tmpNode2, 0, 3);

                Node[i].RnT = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                Cudd_Ref(Node[i].RnT);
                Cudd_RecursiveDerefZdd(manager, tmpNode);
                Cudd_RecursiveDerefZdd(manager, tmpNode2);

                ZddCount = Cudd_zddCount(manager,  Node[i].RnT);

                printf("RnT at i = %d Zdd Count = %d\n", i, ZddCount);
                Cudd_zddPrintDebug(manager,  Node[i].RnT, 0, 3);

                break;
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    tmpNode = Cudd_zddSubset1(manager, Node[tmpList->Id].RnT, 2*i+1);
                    Cudd_Ref(tmpNode);

                    tmpNode2 = Cudd_zddSubset1(manager, Node[tmpList->Id].RnT, 2*i);
                    Cudd_Ref(tmpNode2);

                    if(Node[i].RnT == NULL)
                    {
                        Node[i].RnT = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                        Cudd_Ref(Node[i].RnT);
                        Cudd_RecursiveDerefZdd(manager, tmpNode);
                        Cudd_RecursiveDerefZdd(manager, tmpNode2);

                    } else {
                        tmpNode3 = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                        Cudd_Ref(tmpNode3);
                        Cudd_RecursiveDerefZdd(manager, tmpNode);
                        Cudd_RecursiveDerefZdd(manager, tmpNode2);

                    }

                    if(tmpNode3 != NULL)
                    {
                        tmpNode2 = Cudd_zddUnion(manager, tmpNode3, Node[i].RnT);
                        Cudd_Ref(tmpNode2);
                        Cudd_RecursiveDerefZdd(manager, Node[i].RnT);
                        Cudd_RecursiveDerefZdd(manager, tmpNode3);
                        tmpNode3 = NULL;

                        Node[i].RnT = tmpNode2;
                    }
                }

                ZddCount = Cudd_zddCount(manager,  Node[i].RnT);

                printf("RnT at i = %d Zdd Count = %d\n", i, ZddCount);
                Cudd_zddPrintDebug(manager,  Node[i].RnT, 0, 3);

                break;
            case FROM:
            case NOT:
            case BUFF:
                tmpList = Node[i].Fin;

                tmpNode = Cudd_zddSubset1(manager, Node[tmpList->Id].RnT, 2*i+1);
                Cudd_Ref(tmpNode);
                tmpNode2 = Cudd_zddSubset1(manager, Node[tmpList->Id].RnT, 2*i);
                Cudd_Ref(tmpNode2);

                Node[i].RnT = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                Cudd_Ref(Node[i].RnT);
				Cudd_RecursiveDerefZdd(manager, tmpNode);
				Cudd_RecursiveDerefZdd(manager, tmpNode2);

                ZddCount = Cudd_zddCount(manager,  Node[i].RnT);

                printf("RnT at i = %d Zdd Count = %d\n", i, ZddCount);
                Cudd_zddPrintDebug(manager,  Node[i].RnT, 0, 3);

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void storePnT(GATE *Node, DdNode **PathSet, int Tgat)
{
    int i;
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL;

    for(i = 1; i <= Tgat; i++)
    {
        switch(Node[i].Type) {
            case INPT:
                Node[i].PnT = *PathSet;

                //Node[i].PnT = Cudd_zddSubset1(manager, *PathSet, i);
				Cudd_Ref(Node[i].PnT);

				break;
            case AND:
            case NAND:
            case OR:
            case NOR:
            case XOR:
            case XNOR:
            case FROM:
            case NOT:
            case BUFF:
            	for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
				{
					if(Node[i].PnT == NULL)
					{
                        Node[i].PnT = Cudd_zddChange(manager, Node[tmpList->Id].PnT, i);
						Cudd_Ref(Node[i].PnT);

					} else {
                        tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].PnT, i);
						Cudd_Ref(tmpNode);
					}

					if(tmpNode != NULL)
					{
						tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].PnT);
						Cudd_Ref(tmpNode2);

						Cudd_RecursiveDerefZdd(manager, tmpNode);
						tmpNode = NULL;

						Cudd_RecursiveDerefZdd(manager, Node[i].PnT);

						Node[i].PnT = tmpNode2;
					}
				}

                tmpNode = Cudd_zddSubset1(manager, *PathSet, i);
                Cudd_Ref(tmpNode);

                tmpNode2 = Cudd_zddDiff(manager, tmpNode, Node[i].PnT);
                Cudd_Ref(tmpNode2);

                Cudd_RecursiveDerefZdd(manager, tmpNode);
                tmpNode = NULL;

                Cudd_RecursiveDerefZdd(manager, Node[i].PnT);

                Node[i].PnT = tmpNode2;

            	setValidateMark(Node, i);

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void extractVNR(GATE *Node, DdNode **PathSet, int Npo)
{
	int i;
	DdNode *tmpNode = NULL;

    for(i = 1; i < Npo; i++)
    {
    	if(*PathSet == NULL)
		{
			*PathSet = Node[primaryOutputs[i]].PnT;
			Cudd_Ref(*PathSet);

		} else {
			tmpNode = Cudd_zddUnion(manager, *PathSet, Node[primaryOutputs[i]].PnT);
			Cudd_Ref(tmpNode);
			Cudd_RecursiveDerefZdd(manager, *PathSet);
			Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[i]].PnT);
			Node[primaryOutputs[i]].PnT = NULL;

			*PathSet = tmpNode;
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

DdNode *containment(DdManager *zdd, DdNode *P, DdNode *Q)
{
    DdNode *R = NULL;
    DdNode *P1 = NULL, *P0 = NULL;
    DdNode *Q1 = NULL, *Q0 = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL;
    int x = 0;

    if(Q == DD_ONE(zdd))
        return P;

    if(P == DD_ZERO(zdd) || P == DD_ONE(zdd))
        return DD_ZERO(zdd);

    if(P == Q)
        return DD_ONE(zdd);

    R = cuddCacheLookup2Zdd(zdd, containment, P, Q);

    if(R != NULL)
        return R;

	if ( cuddIsConstant( Q ) )
		x = Q->index;
	else
		x = zdd->permZ[Q->index];

    cuddZddGetCofactors2(zdd, P, x, &P1, &P0);

    if (P1 == NULL)
		return NULL;

    //cuddRef(P1);

    if (P0 == NULL)
    {
    	Cudd_RecursiveDerefZdd(zdd, P1);
		return NULL;
    }

    //cuddRef(P0);

    cuddZddGetCofactors2(zdd, Q, x, &Q1, &Q0);

    if(Q1 != NULL)
    {
    	//cuddRef(Q1);

        R = containment(zdd, P1, Q1);

		if (R == NULL)
	    {
	    	Cudd_RecursiveDerefZdd(zdd, Q1);
	    	Cudd_RecursiveDerefZdd(zdd, P1);
	    	Cudd_RecursiveDerefZdd(zdd, P0);

			return NULL;
	    }

		cuddRef(R);
		//cuddDeref(P1);
		//cuddDeref(Q1);
    }

    if(Q0 != NULL)
    {
    	//cuddRef(Q0);

        tmpNode = containment(zdd, P1, Q1);

		if (tmpNode == NULL)
		{
	    	Cudd_RecursiveDerefZdd(zdd, Q1);
	    	Cudd_RecursiveDerefZdd(zdd, P1);
	    	Cudd_RecursiveDerefZdd(zdd, P0);
	    	Cudd_RecursiveDerefZdd(zdd, Q0);

			return NULL;
		}

		cuddRef(tmpNode);
		//cuddDeref(P1);
        //cuddDeref(Q1);

        tmpNode2 = cuddZddUnion(zdd, R, tmpNode);

		if (tmpNode2 == NULL)
		{
	    	Cudd_RecursiveDerefZdd(zdd, Q1);
	    	Cudd_RecursiveDerefZdd(zdd, P1);
	    	Cudd_RecursiveDerefZdd(zdd, P0);
	    	Cudd_RecursiveDerefZdd(zdd, Q0);
			Cudd_RecursiveDerefZdd(zdd, tmpNode);

			return NULL;
		}

		cuddRef(tmpNode2);
		cuddDeref(R);
		cuddDeref(tmpNode);

        R = tmpNode2;
    }

    tmpNode = cuddZddDiff(zdd, P, Q);
    cuddRef(tmpNode);
    cuddDeref(P);
    cuddDeref(Q);

    cuddCacheInsert2(zdd, containment, P, Q, R);

    return R;
}

void eliminate(DdNode **P, DdNode **Q)
{

}

void setValidateMark(GATE* Node, int i)
{
    LIST *tmpList = NULL;
    int RMark = 0, FMark = 0;

	for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
	{
		if(Node[tmpList->Id].Val == R1)
		{
			RMark = 1;
		}

		if(Node[tmpList->Id].Val == F0)
		{
			FMark = 1;
		}
	}

	if(RMark == 1 && FMark == 1)
	{
		Node[i].ValidateMark = 1;
	}
}
