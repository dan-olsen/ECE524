#include "StorePaths.h"

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

void extractRFPDFs(GATE *Node, DdNode **PathSet, int Npo, int Tgat)
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
                    if(Node[i].TempVal == R1)
                	{
						Node[i].TempRpath = Cudd_zddChange(manager, onez, 2*i);
						Cudd_Ref(Node[i].TempRpath);

                    } else if(Node[i].TempVal == F0) {
                        Node[i].TempFpath = Cudd_zddChange(manager, onez, 2*i+1);
                        Cudd_Ref(Node[i].TempFpath);
                	}
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
                        if(Node[tmpList->Id].TempVal == R1 && Node[tmpList->Id].TempRpath != NULL)
                        {
                            tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempRpath, 2*i);
                            Cudd_Ref(tmpNode);

                        } else if(Node[tmpList->Id].TempVal == F0 && Node[tmpList->Id].TempFpath != NULL) {
                            tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempFpath, 2*i+1);
                            Cudd_Ref(tmpNode);

                        }
                        if(tmpNode != NULL)
                        {
                            if(Node[i].TempVal == R1)
                            {
                                if(Node[i].TempRpath == NULL)
                                {
                                    Node[i].TempRpath = tmpNode;
                                    tmpNode = NULL;

                                } else {
                                    tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempRpath);
                                    Cudd_Ref(tmpNode2);

                                    Cudd_RecursiveDerefZdd(manager, tmpNode);
                                    tmpNode = NULL;
                                    Cudd_RecursiveDerefZdd(manager, Node[i].TempRpath);
                                    Node[i].TempRpath = NULL;

                                    Node[i].TempRpath = tmpNode2;
                                }
                            } else if(Node[i].TempVal == F0)
                            {
                                if(Node[i].TempFpath == NULL)
                                {
                                    Node[i].TempFpath = tmpNode;
                                    tmpNode = NULL;

                                } else {
                                    tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempFpath);
                                    Cudd_Ref(tmpNode2);

                                    Cudd_RecursiveDerefZdd(manager, tmpNode);
                                    tmpNode = NULL;
                                    Cudd_RecursiveDerefZdd(manager, Node[i].TempFpath);
                                    Node[i].TempFpath = NULL;

                                    Node[i].TempFpath = tmpNode2;
                                }
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
                    if(Node[tmpList->Id].TempVal == R1 && Node[tmpList->Id].TempRpath != NULL)
                    {
                        tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempRpath, 2*i);
                        Cudd_Ref(tmpNode);

                    } else if(Node[tmpList->Id].TempVal == F0 && Node[tmpList->Id].TempFpath != NULL) {
                        tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].TempFpath, 2*i+1);
                        Cudd_Ref(tmpNode);

                    }

                    if(tmpNode != NULL)
                    {
                        if(Node[i].TempVal == R1)
                        {
                            if(Node[i].TempRpath == NULL)
                            {
                                Node[i].TempRpath = tmpNode;
                                tmpNode = NULL;

                            } else {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempRpath);
                                Cudd_Ref(tmpNode2);

                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                tmpNode = NULL;
                                Cudd_RecursiveDerefZdd(manager, Node[i].TempRpath);
                                Node[i].TempRpath = NULL;

                                Node[i].TempRpath = tmpNode2;
                            }
                        } else if(Node[i].TempVal == F0)
                        {
                            if(Node[i].TempFpath == NULL)
                            {
                                Node[i].TempFpath = tmpNode;
                                tmpNode = NULL;

                            } else {
                                tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].TempFpath);
                                Cudd_Ref(tmpNode2);

                                Cudd_RecursiveDerefZdd(manager, tmpNode);
                                tmpNode = NULL;
                                Cudd_RecursiveDerefZdd(manager, Node[i].TempFpath);
                                Node[i].TempFpath = NULL;

                                Node[i].TempFpath = tmpNode2;
                            }
                        }
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
        if(Node[primaryOutputs[j]].TempVal == R1 && Node[primaryOutputs[j]].TempRpath != NULL)
        {
            if(*PathSet == NULL)
            {
                *PathSet = Node[primaryOutputs[j]].TempRpath;
                Cudd_Ref(*PathSet);

            } else {
                tmpNode3 = Cudd_zddUnion(manager, *PathSet, Node[primaryOutputs[j]].TempRpath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, *PathSet);
                Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[j]].TempRpath);
                Node[primaryOutputs[j]].TempRpath = NULL;

                *PathSet = tmpNode3;
            }
        } else if(Node[primaryOutputs[j]].TempVal == F0 && Node[primaryOutputs[j]].TempFpath != NULL) {
            if(*PathSet == NULL)
            {
                *PathSet = Node[primaryOutputs[j]].TempFpath;
                Cudd_Ref(*PathSet);

            } else {
                tmpNode3 = Cudd_zddUnion(manager, *PathSet, Node[primaryOutputs[j]].TempFpath);
                Cudd_Ref(tmpNode3);
                Cudd_RecursiveDerefZdd(manager, *PathSet);
                Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[j]].TempFpath);
                Node[primaryOutputs[j]].TempFpath = NULL;

                *PathSet = tmpNode3;
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

void storeNnt(GATE *Node, int i, DdNode **PathSet)
{
    DdNode *tmpNode = NULL;
    DdNode *tmpNode2 = NULL;
    DdNode *tmpNode3 = NULL;
    LIST *tmpList = NULL;

    switch(Node[i].Type) {
        case INPT:
            tmpNode = Cudd_zddSubset1(manager, *PathSet, 2*i+1);
            Cudd_Ref(tmpNode);

            tmpNode2 = Cudd_zddSubset1(manager, *PathSet, 2*i);
            Cudd_Ref(tmpNode2);

            Node[i].Nnt = Cudd_zddUnion(manager, tmpNode, tmpNode2);
            Cudd_Ref(Node[i].Nnt);
            Cudd_RecursiveDerefZdd(manager, tmpNode);
            Cudd_RecursiveDerefZdd(manager, tmpNode2);

            break;
        case AND:
        case NAND:
        case OR:
        case NOR:
        case XOR:
        case XNOR:
            for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
            {
                tmpNode = Cudd_zddSubset1(manager, Node[tmpList->Id].Nnt, 2*tmpList->Id+1);
                Cudd_Ref(tmpNode);

                tmpNode2 = Cudd_zddSubset1(manager, Node[tmpList->Id].Nnt, 2*tmpList->Id);
                Cudd_Ref(tmpNode2);

                if(Node[i].Nnt == NULL)
                {
                    Node[i].Nnt = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                    Cudd_Ref(Node[i].Nnt);
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
                    tmpNode2 = Cudd_zddUnion(manager, tmpNode3, Node[i].Nnt);
                    Cudd_Ref(tmpNode2);
                    Cudd_RecursiveDerefZdd(manager, Node[i].Nnt);
                    Cudd_RecursiveDerefZdd(manager, tmpNode3);
                    tmpNode3 = NULL;

                    Node[i].Nnt = tmpNode2;
                }
            }

            break;
        case FROM:
        case NOT:
        case BUFF:
            tmpList = Node[i].Fin;

            tmpNode = Cudd_zddSubset1(manager, Node[tmpList->Id].Nnt, 2*tmpList->Id+1);
            Cudd_Ref(tmpNode);
            tmpNode2 = Cudd_zddSubset1(manager, Node[tmpList->Id].Nnt, 2*tmpList->Id);
            Cudd_Ref(tmpNode2);

            Node[i].Nnt = Cudd_zddUnion(manager, tmpNode, tmpNode2);
            Cudd_Ref(Node[i].Nnt);
            Cudd_RecursiveDerefZdd(manager, tmpNode);
            Cudd_RecursiveDerefZdd(manager, tmpNode2);

            break;
        default:
            //printf("Hit Default at i: %d ", i);
            //printf("Type: %d\n", graph[i].typ);
            break;
    }
}

void storeRnT(GATE *Node, DdNode **PathSet, int Tgat)
{
    int i;
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

                tmpNode2 = Cudd_zddSubset1(manager, *PathSet, 2*i);
                Cudd_Ref(tmpNode2);

                Node[i].RnT = Cudd_zddUnion(manager, tmpNode, tmpNode2);
                Cudd_Ref(Node[i].RnT);
                Cudd_RecursiveDerefZdd(manager, tmpNode);
                Cudd_RecursiveDerefZdd(manager, tmpNode2);

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

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void storePnt(GATE *Node, int i, DdNode **PathSet, int Tgat)
{
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL, *tmpNode2 = NULL;

    switch(Node[i].Type) {
        case INPT:
            //Node[i].Pnt = *PathSet;

            if(Node[i].RobustVal == R1 || Node[i].RobustVal == F0)
            {
                if(Node[i].RobustVal == R1)
                {
                    Node[i].Pnt = Cudd_zddChange(manager, onez, 2*i);
                    Cudd_Ref(Node[i].Pnt);

                } else if(Node[i].RobustVal == F0) {
                    Node[i].Pnt = Cudd_zddChange(manager, onez, 2*i+1);
                    Cudd_Ref(Node[i].Pnt);
                }
            }

            break;
        case AND:
        case NAND:
        case OR:
        case NOR:
        case XOR:
        case XNOR:
            if(Node[i].RobustVal == R1 || Node[i].RobustVal == F0)
            {
                for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
                {
                    if(Node[tmpList->Id].RobustVal == R1 && Node[tmpList->Id].Pnt != NULL)
                    {
                        tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Pnt, 2*i);
                        Cudd_Ref(tmpNode);

                    } else if(Node[tmpList->Id].RobustVal == F0 && Node[tmpList->Id].Pnt != NULL) {
                        tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Pnt, 2*i+1);
                        Cudd_Ref(tmpNode);

                    }
                    if(tmpNode != NULL)
                    {
                        if(Node[i].Pnt == NULL)
                        {
                            Node[i].Pnt = tmpNode;
                            tmpNode = NULL;

                        } else {
                            tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].Pnt);
                            Cudd_Ref(tmpNode2);

                            Cudd_RecursiveDerefZdd(manager, tmpNode);
                            tmpNode = NULL;
                            Cudd_RecursiveDerefZdd(manager, Node[i].Pnt);
                            Node[i].Pnt = NULL;

                            Node[i].Pnt = tmpNode2;
                        }
                    }
                }
            }

            break;
        case BUFF:
        case NOT:
        case FROM:
            tmpList = Node[i].Fin;

            if(Node[i].RobustVal == R1 || Node[i].RobustVal == F0)
            {
                if(Node[tmpList->Id].RobustVal == R1 && Node[tmpList->Id].Pnt != NULL)
                {
                    tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Pnt, 2*i);
                    Cudd_Ref(tmpNode);

                } else if(Node[tmpList->Id].RobustVal == F0 && Node[tmpList->Id].Pnt != NULL) {
                    tmpNode = Cudd_zddChange(manager, Node[tmpList->Id].Pnt, 2*i+1);
                    Cudd_Ref(tmpNode);

                }

                if(tmpNode != NULL)
                {
                    if(Node[i].Pnt == NULL)
                    {
                        Node[i].Pnt = tmpNode;
                        tmpNode = NULL;

                    } else {
                        tmpNode2 = Cudd_zddUnion(manager, tmpNode, Node[i].Pnt);
                        Cudd_Ref(tmpNode2);

                        Cudd_RecursiveDerefZdd(manager, tmpNode);
                        tmpNode = NULL;
                        Cudd_RecursiveDerefZdd(manager, Node[i].Pnt);
                        Node[i].Pnt = NULL;

                        Node[i].Pnt = tmpNode2;
                    }
                }
            }

            break;
        default:
            //printf("Hit Default at i: %d ", i);
            //printf("Type: %d\n", graph[i].typ);
            break;
    }

    clearNodeZDDs(Node, Tgat);

}

void extractVNR(GATE *Node, DdNode **PathSet, int Npo)
{
	int i;
	DdNode *tmpNode = NULL;

    for(i = 1; i < Npo; i++)
    {
    	if(*PathSet == NULL)
		{
            *PathSet = Node[primaryOutputs[i]].Pnt;
			Cudd_Ref(*PathSet);

		} else {
            tmpNode = Cudd_zddUnion(manager, *PathSet, Node[primaryOutputs[i]].Pnt);
			Cudd_Ref(tmpNode);
			Cudd_RecursiveDerefZdd(manager, *PathSet);
            Cudd_RecursiveDerefZdd(manager, Node[primaryOutputs[i]].Pnt);
            Node[primaryOutputs[i]].Pnt = NULL;

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

void checkRobustlyTestedOffInput(GATE* Node, int i, DdNode **RobustPathSet)
{
    LIST *tmpList = NULL;
    DdNode *tmpNode = NULL;
    DdNode *tmpNode2 = NULL;

    for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
    {
        if(Node[tmpList->Id].RnT != NULL && Node[tmpList->Id].Pnt != NULL && Node[i].Nnt != NULL)
        {
            tmpNode = Cudd_zddProduct(manager, Node[tmpList->Id].RnT, Node[tmpList->Id].Pnt);
            Cudd_Ref(tmpNode);

            tmpNode2 = Cudd_zddIntersect(manager, *RobustPathSet, tmpNode);
            Cudd_Ref(tmpNode2);
            Cudd_RecursiveDerefZdd(manager, tmpNode);

            tmpNode = containment(manager, tmpNode2, Node[tmpList->Id].Pnt);
            Cudd_Ref(tmpNode);
            Cudd_RecursiveDerefZdd(manager, tmpNode2);

            tmpNode2 = Cudd_zddIntersect(manager, Node[i].Nnt, tmpNode);
            Cudd_Ref(tmpNode2);
            Cudd_RecursiveDerefZdd(manager, tmpNode);
            Cudd_RecursiveDerefZdd(manager, Node[i].Nnt);

            Node[i].Nnt = tmpNode2;
        }
    }

    if(Node[i].Nnt != NULL)
    {
        //Node[i].Pnt =
    }
}

void setCosensitizationMark(GATE* Node, int i)
{
    LIST *tmpList = NULL;
    int RMark = 0, FMark = 0;

	for(tmpList = Node[i].Fin; tmpList != NULL; tmpList = tmpList->Next)
	{
        if(Node[tmpList->Id].TempVal == R1)
		{
			RMark = 1;
		}

        if(Node[tmpList->Id].TempVal == F0)
		{
			FMark = 1;
		}
	}

	if(RMark == 1 && FMark == 1)
	{
        Node[i].CosensitizationMark = 1;
	}
}
