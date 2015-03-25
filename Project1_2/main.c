#include "given.h"
#include "user.h"
/***************************************************************************************************
Command Instructions
***************************************************************************************************/
//To Compile: make
//To Run: ./project1 c17.isc c17.pattern c17.res 
/***************************************************************************************************
 Main Function
***************************************************************************************************/
int main(int argc, char **argv)
{
	FILE *Isc, *Pat, *Res;	//File pointers used for .isc, .pattern, and .res files
	//clock_t Start, End;		//Clock variables to calculate the Cputime
	//double Cpu;				//Total cpu time
	//int i, j;				//Temporary variables

	manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);	//Intializing CUDD package manger
	onez = Cudd_ReadZddOne(manager, ( (2 * Mnod ) + 5 ));

	/****************PART 1.-Read the .isc file and store the information in Node structure***********/
	Npi = Npo = Tgat = 0;							//Intialize values of all variables
	Node=(GATE *) malloc(Mnod * sizeof(GATE));     //Dynamic memory allocation for Node structure

	Isc = fopen(argv[1],"r");						//File pointer to open .isc file

	Tgat = ReadIsc(Isc,Node);						//Read .isc file and return index of last node

	fclose(Isc);									//Close file pointer for .isc file

	PrintGats(Node,Tgat);							//Print the information of each active gate in Node structure after reading .isc file
	CountPri(Node,Tgat,&Npi,&Npo);					//Count the No of Pis and Pos
	printf("\n\nNpi: %d Npo: %d\n\n",Npi,Npo);		//Print the no of primary inputs and outputs
	/***************************************************************************************************/

	Pat = fopen(argv[2],"r");		//File pointer to open .pattern file
	Res = fopen(argv[3],"w");		//File pointer to open .result file

	readPatternFile(Pat);

	patternSim();

	free(patterns);
	fclose(Pat);
	fclose(Res);

	clearPathZDDs();

	/***************************************************************************************************/
	printf("\nNo of Unreferenced Zdds: %d\n", Cudd_CheckZeroRef(manager));	//Checking any unreferenced bdds in manager
	Cudd_Quit(manager);														//Closing the cudd package manager
	ClearGat(Node,Tgat);													//Clear memeory for all members of Node
	free(Node);

	printf("Done\n");
	return 0;
}//end of main
/****************************************************************************************************************************/
