#include "given.h"
#include "user.h"

void CreateBDD(NODE* graph, int max)
{
	int i;
	
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
				
				break;				 
			case NAND:
				
				break;
			case OR:
				
				break;
			case NOR:
				
				break;
			case XOR:
				
				break;
			case XNOR:
				
				break;
			case BUFF:
				
				break;
			case NOT:
				
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
