#include "given.h"
#include "PatternSim.h"
#include "StorePaths.h"
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
    int Npi, Npo, Tgat, i;                                         //Tot no of PIs, Pos, Maxid, Tot no of patterns in.vec, .fau
    FILE *iscFile = NULL, *patFile = NULL, *resFile = NULL;     //File pointers used for .isc, .pattern, and .res files
    clock_t Start, End;                                         //Clock variables to calculate the Cputime
    double Cpu;                                                 //Total cpu time
    GATE *Node = NULL;                                          //Structure to store the ckt given in .isc file

    int ZddCount = 0;
    int *pattern = NULL;
    DdNode *RobustRpathSet = NULL, *RobustFpathSet = NULL, *RobustPathSet = NULL;
    DdNode *NonRobustPathSet = NULL, *NonRobustRpathSet = NULL, *NonRobustFpathSet = NULL;
    DdNode *SuspectSet = NULL;
    DdNode *GoodPaths = NULL;
    DdNode *tmpNode = NULL;

    if(argc < 4)
    {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);  /* Exit, returning error code. */
    }

    printf("Start Timing Diagnosis for %s\n", argv[1]);

    manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);      //Intializing CUDD package manger
    onez = Cudd_ReadZddOne(manager, ( (2 * Mnod ) + 5 ));

    /****************PART 1.-Read the .isc file and store the information in Node structure***********/
    Npi = Npo = Tgat = 0;                           //Intialize values of all variables
    Node = (GATE *) malloc(Mnod * sizeof(GATE));    //Dynamic memory allocation for Node structure

    iscFile = fopen(argv[1], "r");                  //File pointer to open .isc file

    if(iscFile == NULL)
    {
        fprintf(stderr, "Failed to open Isc file.\n");
        exit(1);  /* Exit, returning error code. */
    }

    Tgat = ReadIsc(iscFile, Node);  //Read .isc file and return index of last node
    fclose(iscFile);                //Close file pointer for .isc file

    //PrintGats(Node, Tgat);                        //Print the information of each active gate in Node structure after reading .isc file
    CountPri(Node, Tgat, &Npi, &Npo);               //Count the No of Pis and Pos
    printf("\nNpi: %d Npo: %d\n", Npi, Npo);    //Print the no of primary inputs and outputs

    /***************************************************************************************************/

    patFile = fopen(argv[2], "r");      //File pointer to open .pattern file
    resFile = fopen(argv[3], "w");    //File pointer to open .result file

    initDelay(Node, Npi, Npo, Tgat);

    //iterate over patterns
    while((pattern = getNextPattern(&patFile, Npi)) != NULL)
    {
        //printf("Applying Pattern: ");
        //printPattern(pattern, Npi);

        //topologoical traversal to apply pattern
        applyPatternRobust(Node, pattern, Tgat);

        storeSensitizedPaths(Node, &RobustPathSet, Tgat);
        //storeSensitizedRPaths(Node, &RobustRpathSet);
        //storeSensitizedFPaths(Node, &RobustFpathSet);

        storeLSPaths(Node, Npo, &GoodPaths, &SuspectSet);

        //applyPatternNonRobust(Node, pattern, Tgat);

        //storeSensitizedPaths(Node, &NonRobustPathSet, Tgat);

        free(pattern);

        //printf("\n");
    }

    //storeRnT(Node, &RobustPathSet, Tgat);
    //storeNnT(Node, &NonRobustPathSet, Tgat);

    for(i = 0; i <= Tgat; i++)
    {
    	if(Node[i].NnT != NULL)
    	{
    		ZddCount = Cudd_zddCount(manager, Node[i].NnT);

    		printf("NnT Zdd Count at %d = %d\n", i, ZddCount);
    	}
    }

    if(RobustPathSet != NULL)
    {
    	ZddCount = Cudd_zddCount(manager, RobustPathSet);

        printf("Robust Path ZDD Count: %d\n", ZddCount);
        fprintf(resFile, "Robust Path ZDD Count: %d\n", ZddCount);

    } else {
        printf("Robust Path ZDD Count: 0\n");
        fprintf(resFile, "Robust Path ZDD Count: 0\n");

    }

    /*if(NonRobustPathSet != NULL)
    {
    	ZddCount = Cudd_zddCount(manager, NonRobustPathSet);

        printf("Non-Robust Path ZDD Count: %d\n", ZddCount);
        fprintf(resFile, "Non-Robust Path ZDD Count: %d\n", ZddCount);

    } else {
        printf("Non-Robust Path ZDD Count: 0\n");
        fprintf(resFile, "Non-Robust Path ZDD Count: 0\n");

    }*/

    if(GoodPaths != NULL)
    {
    	ZddCount = Cudd_zddCount(manager, GoodPaths);

        printf("Good Path ZDD Count: %d\n", ZddCount);
        fprintf(resFile, "Good Path ZDD Count: %d\n", ZddCount);

    } else {
        printf("Good Path ZDD Count: 0\n");
        fprintf(resFile, "Good Path ZDD Count: 0\n");

    }

    if(SuspectSet != NULL)
    {
    	if(GoodPaths != NULL)
    	{
			tmpNode = Cudd_zddDiff(manager, SuspectSet, GoodPaths);
			Cudd_Ref(tmpNode);
			Cudd_RecursiveDerefZdd(manager, SuspectSet);

			SuspectSet = tmpNode;
    	}

    	ZddCount = Cudd_zddCount(manager, SuspectSet);

        printf("Suspect Set ZDD Count: %d\n", ZddCount);
        fprintf(resFile, "Suspect Set ZDD Count: %d\n", ZddCount);

    } else {
        printf("Suspect Set ZDD Count: 0\n");
        fprintf(resFile, "Suspect Set ZDD Count: 0\n");

    }

    clearPathZDDs(&RobustRpathSet);
    clearPathZDDs(&RobustFpathSet);
    clearPathZDDs(&RobustPathSet);

    clearPathZDDs(&NonRobustRpathSet);
    clearPathZDDs(&NonRobustFpathSet);
    clearPathZDDs(&NonRobustPathSet);

    clearPathZDDs(&GoodPaths);
    clearPathZDDs(&SuspectSet);

    clearNodeRnT(Node, Tgat);
    clearNodeNnT(Node, Tgat);

    fclose(patFile);
    fclose(resFile);

    /***************************************************************************************************/
    printf("\nNo of Unreferenced Zdds: %d\n", Cudd_CheckZeroRef(manager));      //Checking any unreferenced bdds in manager
    Cudd_Quit(manager);                                                         //Closing the cudd package manager
    ClearGat(Node, Tgat);                                                       //Clear memeory for all members of Node

    freePathSet(Npo);
    free(Node);
    freeInputOutputArrays();

    printf("Done\n");
    return 0;
}//end of main
/*******************************************************************************************************/
