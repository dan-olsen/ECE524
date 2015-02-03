#include "given.h"
#include "user.h"

void CreateBDD(NODE* graph, int max)
{
	int i, j;
	
	for(i = 0; i < max; i++)
	{
		switch(graph[i].typ) {
			case INPT:
				graph[i].fun = Cudd_bddIthVar(manager, i);
				Cudd_Ref(graph[i].fun);
				printf("BDD input %d\n", i);
				Cudd_PrintMinterm(manager, graph[i].fun);

				break;				
			case AND:
				graph[i].fun = Cudd_bddAnd(manager, );
				
				break;				 
			case NAND:
				graph[i].fun = Cudd_bddNand(manager, );
				
				break;
			case OR:
				graph[i].fun = Cudd_bddOr(manager, );
				
				break;
			case NOR:
				graph[i].fun = Cudd_bddNor(manager, );
				
				break;
			case XOR:
				graph[i].fun = Cudd_bddXor(manager, );
				
				break;
			case XNOR:
				graph[i].fun = Cudd_bddXnor(manager, );
				
				break;
			case BUFF:
				
				break;
			case NOT:
				graph[i].fun = Cudd_bddNot(manager, );
				
				break;
			case FROM:
				
				break;
			default:
				//print error
				continue;
		}
	}

	return;
}
void ClearBDD(NODE *graph, int max)
{
	
}

/****************************************************************************************************************************/
