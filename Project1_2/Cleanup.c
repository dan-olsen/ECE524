#include "Cleanup.h"

void clearPathZDDs(DdNode **PathSet)
{
    if(*PathSet != NULL)
    {
        Cudd_RecursiveDerefZdd(manager, *PathSet);
        *PathSet = NULL;
    }
}

void clearNodeZDDs(GATE *Node, int Tgat)
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

                break;

            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}


void clearNodeRnT(GATE *Node, int Tgat)
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
                if(Node[i].RnT != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].RnT);
                    Node[i].RnT = NULL;
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}


void clearNodeNnt(GATE *Node, int Tgat)
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
                if(Node[i].Nnt != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].Nnt);
                    Node[i].Nnt = NULL;
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void clearNodePnt(GATE *Node, int Tgat)
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
                if(Node[i].Pnt != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].Pnt);
                    Node[i].Pnt = NULL;
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}
