#include "given.h"
#include "PatternSim.h"
#include "StorePaths.h"

void printResults(FILE **resFile, DdNode **RobustPathSet, DdNode **NonRobustPathSet, DdNode **ValidatedNonRobustPathSet, DdNode **GoodPaths, DdNode **SuspectSet);
int getZddCount(DdNode **PathSet);
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
    DdNode *NonRobustPathSet = NULL;
    DdNode *ValidatedNonRobustPathSet = NULL;
    DdNode *SuspectSet = NULL;
    DdNode *GoodPaths = NULL;
    DdNode *tmpNode = NULL;
    char fname[50];
    FILE *dot = NULL;

    Start = clock();

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

        extractPDFs(Node, &RobustPathSet, Npo, Tgat);
        extractRPDFs(Node, &RobustRpathSet, Npo, Tgat);
        extractFPDFs(Node, &RobustFpathSet, Npo, Tgat);

        storeLSPaths(Node, Npo, &GoodPaths, &SuspectSet);

        free(pattern);

        //printf("\n");
    }

    if(GoodPaths != NULL)
    {
        ZddCount = Cudd_zddCount(manager, GoodPaths);

        printf("GoodPaths Zdd Count = %d\n", ZddCount);
    }

    storeRnT(Node, &RobustPathSet, Tgat);

    rewind(patFile);

    //iterate over patterns
    while((pattern = getNextPattern(&patFile, Npi)) != NULL)
    {
        //printf("Applying Pattern: ");
        //printPattern(pattern, Npi);

        //topologoical traversal to apply pattern
        applyPatternNonRobust(Node, pattern, Tgat);

        extractPDFs(Node, &NonRobustPathSet, Npo, Tgat);

        //storeNnT(Node, &NonRobustPathSet, Tgat);
        //storePnT(Node, &RobustPathSet, Tgat);

        //extractVNR(Node, &ValidatedNonRobustPathSet, Npo);
        /*
        for(i = 0; i <= Tgat; i++)
        {
            if(Node[i].NnT != NULL)
            {
                ZddCount = Cudd_zddCount(manager, Node[i].NnT);

                printf("NnT Zdd Count at %d = %d\n", i, ZddCount);
            }
        }

        for(i = 0; i <= Tgat; i++)
        {
            if(Node[i].PnT != NULL)
            {
                ZddCount = Cudd_zddCount(manager, Node[i].PnT);

                printf("PnT Zdd Count at %d = %d\n", i, ZddCount);

                Cudd_zddPrintDebug(manager, Node[i].PnT, 1, 3);

                //sprintf(fname, "../Project1_2/Dot/%d.dot", i);

                //dot = fopen(fname, "w");

                //Cudd_zddDumpDot(manager, 1, &(Node[i].PnT), NULL, NULL, dot);

                //fclose(dot);
            }
        }

        //dot = fopen("../Project1_2/Dot/NonRobust.dot", "w");

        //Cudd_zddDumpDot(manager, 1, &(NonRobustPathSet), NULL, NULL, dot);

        //fclose(dot);
        */
        clearNodeNnT(Node, Tgat);
        clearNodePnT(Node, Tgat);

        clearPathZDDs(&NonRobustPathSet);

        free(pattern);

        //printf("\n");
    }
    /*
    for(i = 0; i <= Tgat; i++)
    {
    	if(Node[i].RnT != NULL)
    	{
    		ZddCount = Cudd_zddCount(manager, Node[i].RnT);

    		printf("RnT Zdd Count at %d = %d\n", i, ZddCount);

            Cudd_zddPrintDebug(manager, Node[i].RnT, 0, 3);

            //sprintf(fname, "../Project1_2/Dot/%d.dot", i);

            //dot = fopen(fname, "w");

            //Cudd_zddDumpDot(manager, 1, &(Node[i].RnT), NULL, NULL, dot);

            //fclose(dot);

    	}

    	if(Node[i].NnT != NULL)
    	{
    		ZddCount = Cudd_zddCount(manager, Node[i].NnT);

    		printf("NnT Zdd Count at %d = %d\n", i, ZddCount);
    	}

    	if(Node[i].PnT != NULL)
    	{
    		ZddCount = Cudd_zddCount(manager, Node[i].PnT);

    		printf("PnT Zdd Count at %d = %d\n", i, ZddCount);
    	}
    }
    */
    //dot = fopen("../Project1_2/Dot/Robust.dot", "w");

    //Cudd_zddDumpDot(manager, 1, &(RobustPathSet), NULL, NULL, dot);

    //fclose(dot);

    //dot = fopen("./Dot/NonRobust.dot", "w");

    //(manager, 1, &(NonRobustPathSet), NULL, NULL, dot);

    //fclose(dot);

    if(GoodPaths != NULL)
    {
        ZddCount = Cudd_zddCount(manager, GoodPaths);

        printf("GoodPaths Zdd Count = %d\n", ZddCount);
    }

    printResults(&resFile, &RobustPathSet, &NonRobustPathSet, &ValidatedNonRobustPathSet, &GoodPaths, &SuspectSet);

    clearPathZDDs(&RobustRpathSet);
    clearPathZDDs(&RobustFpathSet);
    clearPathZDDs(&RobustPathSet);

    clearPathZDDs(&NonRobustPathSet);

    clearPathZDDs(&ValidatedNonRobustPathSet);

    clearPathZDDs(&GoodPaths);
    clearPathZDDs(&SuspectSet);

    clearNodeRnT(Node, Tgat);
    clearNodeNnT(Node, Tgat);
    clearNodePnT(Node, Tgat);

    fclose(patFile);

    /***************************************************************************************************/
    printf("\nNo of Unreferenced Zdds: %d\n", Cudd_CheckZeroRef(manager));      //Checking any unreferenced bdds in manager
    Cudd_Quit(manager);                                                         //Closing the cudd package manager
    ClearGat(Node, Tgat);                                                       //Clear memeory for all members of Node

    freePathSet(Npo);
    free(Node);
    freeInputOutputArrays();

    End = clock();

    Cpu = (double)(End - Start) / CLOCKS_PER_SEC;

    printf("\nExecution Time: %f s\n", Cpu);
    fprintf(resFile, "\nExecution Time: %f s\n", Cpu);

    fclose(resFile);

    printf("Done\n");
    return 0;
}//end of main

void printResults(FILE **resFile, DdNode **RobustPathSet, DdNode **NonRobustPathSet, DdNode **ValidatedNonRobustPathSet, DdNode **GoodPaths, DdNode **SuspectSet)
{
    int ZddCount = 0;
    DdNode *tmpNode = NULL;

    ZddCount = getZddCount(RobustPathSet);
    printf("Robust Path ZDD Count: %d\n", ZddCount);
    fprintf(*resFile, "Robust Path ZDD Count: %d\n", ZddCount);

    ZddCount = getZddCount(NonRobustPathSet);
    printf("Non-Robust Path ZDD Count: %d\n", ZddCount);
    fprintf(*resFile, "Non-Robust Path ZDD Count: %d\n", ZddCount);

    ZddCount = getZddCount(ValidatedNonRobustPathSet);
	printf("Validated Non-Robust Path ZDD Count: %d\n", ZddCount);
	fprintf(*resFile, "Validated Non-Robust Path ZDD Count: %d\n", ZddCount);

    ZddCount = getZddCount(GoodPaths);
    printf("Good Path ZDD Count: %d\n", ZddCount);
    fprintf(*resFile, "Good Path ZDD Count: %d\n", ZddCount);

    if(*GoodPaths != NULL && *SuspectSet != NULL)
    {
        tmpNode = Cudd_zddDiff(manager, *SuspectSet, *GoodPaths);
        Cudd_Ref(tmpNode);
        Cudd_RecursiveDerefZdd(manager, *SuspectSet);

        *SuspectSet = tmpNode;
    }

    ZddCount = getZddCount(SuspectSet);
    printf("Suspect Set ZDD Count: %d\n", ZddCount);
    fprintf(*resFile, "Suspect Set ZDD Count: %d\n", ZddCount);

}

int getZddCount(DdNode **PathSet)
{
    if(*PathSet != NULL)
    {
        return Cudd_zddCount(manager, *PathSet);

    } else {
        return 0;

    }
}

/*******************************************************************************************************/
