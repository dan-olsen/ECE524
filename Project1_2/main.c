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

    printf("Start Timing Diagnosis\n");

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
    //resFile = fopen(argv[3], "w");    //File pointer to open .result file

    initDelay(Node, Npi, Npo, Tgat);

    //iterate over patterns
    while((pattern = getNextPattern(&patFile, Npi)) != NULL)
    {
        printf("Applying Pattern: ");
        printPattern(pattern, Npi);

        //topologoical traversal to apply pattern
        applyPatternRobust(Node, pattern, Tgat);

        /*for(i = 0; i < Npo; i++)
        {
            printf("Robust Output at %d = %d\n", primaryOutputs[i], Node[primaryOutputs[i]].Val);
        }*/

        storeSensitizedPaths(Node, &RobustPathSet, Tgat);
        //storeSensitizedRPaths(Node, &RobustRpathSet);
        //storeSensitizedFPaths(Node, &RobustFpathSet);

        storeLSPaths(Node, Npo, &GoodPaths, &SuspectSet);

        applyPatternNonRobust(Node, pattern, Tgat);

        /*for(i = 0; i < Npo; i++)
        {
            printf("Non-Robust Output at %d = %d\n", primaryOutputs[i], Node[primaryOutputs[i]].Val);
        }*/

        storeSensitizedPaths(Node, &NonRobustPathSet, Tgat);
        //storeSensitizedRPaths(Node, &NonRobustRpathSet);
        //storeSensitizedFPaths(Node, &NonRobustFpathSet);

        free(pattern);

        printf("\n");
    }

    storeRnT(Node, &RobustPathSet, Tgat);
    storeNnT(Node, &NonRobustPathSet, Tgat);

    if(RobustPathSet != NULL)
        printf("Robust Path ZDD Count: %d\n", Cudd_zddCount(manager, RobustPathSet));

    if(NonRobustPathSet != NULL)
        printf("Non-Robust Path ZDD Count: %d\n", Cudd_zddCount(manager, NonRobustPathSet));

    if(GoodPaths != NULL)
        printf("Good Path ZDD Count: %d\n", Cudd_zddCount(manager, GoodPaths));

    if(SuspectSet != NULL)
    {
        tmpNode = Cudd_zddDiff(manager, SuspectSet, GoodPaths);
        Cudd_Ref(tmpNode);
        Cudd_RecursiveDerefZdd(manager, SuspectSet);

        SuspectSet = tmpNode;

        printf("Suspect Set ZDD Count: %d\n", Cudd_zddCount(manager, SuspectSet));
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
    //fclose(Res);

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
