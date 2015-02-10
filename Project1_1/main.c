#include "given.h"
/***************************************************************************************************************************
Main Function( write all new functions in user.c only and call them in main.c) 
Compile   Command: make
Execution Command: ./proj1 "input file" "output file"
****************************************************************************************************************************/
int main(int argc, char **argv)
{
	FILE *fisc,*fout;	//file pointers used for .isc file and output file
	int Max = 0, Bound = 0;		//maxnode id,Max nodes in a bdd of ckt 
	NODE graph[Mnod];		//structure used to store the ckt information in .isc file


	//Read the .isc file and store the information in graph structure
	fisc = fopen(argv[1],"r"); 						//file pointer to open .isc file 
	//graph = (NODE *) malloc(Mnod * sizeof(NODE));	//dynamic memory allocation for graph structure
	Max = ReadIsc(fisc, graph);						//read .isc file and return index of last node in graph formed
	fclose(fisc);									//close file pointer for .isc file
	PrintCircuit(graph, Max); 						//print all members of graph structure

	manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);	//intializing CUDD package manger
	//fout = fopen(argv[2],"w");										//file pointer to open .out file 

	//Form the bdd functions with PIs as default ordering and find the maximum node size
	CreateBDD(graph, Max);

	FreeBDD(graph, Max);

	//Dereference the bdds
	printf("\nNo of Unreferenced Bdds %d\n", Cudd_CheckZeroRef(manager));
	Cudd_Quit(manager);			//closing the cudd package manager
	ClearCircuit(graph); 	//clear memeory for all members of graph
	//fclose(fout); 				//close the output file

	return 0;
}//end of main
/****************************************************************************************************************************/
