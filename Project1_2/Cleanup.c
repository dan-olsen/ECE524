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


void clearNodeNnT(GATE *Node, int Tgat)
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
                if(Node[i].NnT != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].NnT);
                    Node[i].NnT = NULL;
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}

void clearNodePnT(GATE *Node, int Tgat)
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
                if(Node[i].PnT != NULL)
                {
                    Cudd_RecursiveDerefZdd(manager, Node[i].PnT);
                    Node[i].PnT = NULL;
                }

                break;
            default:
                //printf("Hit Default at i: %d ", i);
                //printf("Type: %d\n", graph[i].typ);
                break;
        }
    }
}
