#include "given.h"
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
	int Npi, Npo, Tgat;		//Tot no of PIs,Pos,Maxid,Tot no of patterns in.vec,.fau
	GATE Node[Mnod];		//Structure to store the ckt given in .isc file
	clock_t Start, End;		//Clock variables to calculate the Cputime
	double Cpu;				//Total cpu time
	int i, j;				//Temporary variables
	int *Patterns = NULL;
	int Tpat;

	manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);	//Intializing CUDD package manger
	onez = Cudd_ReadZddOne(manager, ( (2 * Mnod ) + 5 ));

	/****************PART 1.-Read the .isc file and store the information in Node structure***********/
	Npi = Npo = Tgat = 0;							//Intialize values of all variables

	Isc = fopen(argv[1],"r");						//File pointer to open .isc file

	Tgat = ReadIsc(Isc,Node);						//Read .isc file and return index of last node

	fclose(Isc);									//Close file pointer for .isc file

	//PrintGats(Node,Tgat);							//Print the information of each active gate in Node structure after reading .isc file
	CountPri(Node,Tgat,&Npi,&Npo);					//Count the No of Pis and Pos
	printf("\n\nNpi: %d Npo: %d\n",Npi,Npo);		//Print the no of primary inputs and outputs
	/***************************************************************************************************/

	Pat = fopen(argv[2],"r");		//File pointer to open .pattern file
	Res = fopen(argv[3],"w");		//File pointer to open .result file

	if ( NULL == (Patterns = (int *) malloc(Npi * sizeof(int))) ) {
		printf("malloc failed\n");
		//error
	}

	Tpat = readPatternFile(Patterns, Pat, Npi);

	initDelay(Node);

	patternSim(Node, Patterns, Tpat, Tgat, Npi, Npo);

	fclose(Pat);
	fclose(Res);

	/***************************************************************************************************/
	printf("\nNo of Unreferenced Zdds: %d\n", Cudd_CheckZeroRef(manager));	//Checking any unreferenced bdds in manager
	Cudd_Quit(manager);														//Closing the cudd package manager
	ClearGat(Node,Tgat);													//Clear memeory for all members of Node
	return 0;
}//end of main
/****************************************************************************************************************************/
