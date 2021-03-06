#include "given.h"
#include "user.h"

void CreateBDD(NODE* graph, int max)
{
	int i = 0, j = 0;
	DdNode *tmpNode = NULL;
	LIST *tmpList = NULL;
	int inputCount = 0;
	
	for(i = 0; i < max; i++)
	{
		switch(graph[i].typ) {
			case INPT:
				graph[i].fun = Cudd_bddIthVar(manager, inputCount);
				inputCount++;

				Cudd_Ref(graph[i].fun);
				printf("BDD input %d\n", i);
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;				
			case AND:
				printf("BDD And %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddAnd(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}


				Cudd_PrintMinterm(manager, graph[i].fun);

				break;				 
			case NAND:
				printf("BDD Nand %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddNand(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}
				
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;
			case OR:
				printf("BDD Or %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddOr(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}
				
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;
			case NOR:
				printf("BDD Nor %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddNor(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}
				
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;
			case XOR:
				printf("BDD Xor %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddXor(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}
				
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;
			case XNOR:
				printf("BDD Xnor %d\n", i);
				graph[i].fun = Cudd_ReadOne(manager);
				Cudd_Ref(graph[i].fun);

				for(tmpList = graph[i].fin, j = 0; j < graph[i].nfi; j++, tmpList = graph[i].fin->nxt)
				{
					tmpNode = Cudd_bddXnor(manager, graph[tmpList->id].fun, graph[i].fun);
					Cudd_Ref(tmpNode);
					Cudd_RecursiveDeref(manager, graph[i].fun);
					graph[i].fun = tmpNode;
				}
				
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;
			case BUFF:
				printf("Buff %d\n", i);
				tmpList = graph[i].fin;

				graph[i].fun = graph[tmpList->id].fun;
				Cudd_Ref(graph[i].fun);
				
				break;
			case NOT:
				printf("Not %d\n", i);
				tmpList = graph[i].fin;

				graph[i].fun = Cudd_Not(graph[tmpList->id].fun);
				Cudd_Ref(graph[i].fun);
				
				break;
			case FROM:
				printf("From %d\n", i);
				tmpList = graph[i].fin;

				graph[i].fun = graph[tmpList->id].fun;
				Cudd_Ref(graph[i].fun);

				break;
			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}

	return;
}
void FreeBDD(NODE *graph, int max)
{
	int i;
	
	for(i = 0; i < max; i++)
	{
		switch(graph[i].typ) {
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
				Cudd_RecursiveDeref(manager, graph[i].fun);

				break;

			default:
				//printf("Hit Default at i: %d ", i);
				//printf("Type: %d\n", graph[i].typ);
				break;
		}
	}
}

/****************************************************************************************************************************/
