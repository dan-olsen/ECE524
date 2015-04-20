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
    int Npi, Npo, Tgat;                             //Tot no of PIs, Pos, Maxid, Tot no of patterns in.vec, .fau
    FILE *Isc = NULL, *Pat = NULL, *Res = NULL;     //File pointers used for .isc, .pattern, and .res files
    clock_t Start, End;                             //Clock variables to calculate the Cputime
    double Cpu;                                     //Total cpu time
    GATE *Node = NULL;                              //Structure to store the ckt given in .isc file

    if(argc < 4)
    {
        fprintf(stderr, "Wrong number of arguments.\n");
        exit(1);  /* Exit, returning error code. */
    }

    //Intializing CUDD package manger
    manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    onez = Cudd_ReadZddOne(manager, ( (2 * Mnod ) + 5 ));

    /****************PART 1.-Read the .isc file and store the information in Node structure***********/
    //Intialize values of all variables
    Npi = Npo = Tgat = 0;

    //Dynamic memory allocation for Node structure
    Node = (GATE *) malloc(Mnod * sizeof(GATE));

    //File pointer to open .isc file
    Isc = fopen(argv[1], "r");

    if(Isc == NULL)
    {
        fprintf(stderr, "Failed to open Isc file.\n");
        exit(1);  /* Exit, returning error code. */
    }

    //Read .isc file and return index of last node
    Tgat = ReadIsc(Isc, Node);

    //Close file pointer for .isc file
    fclose(Isc);

    //Print the information of each active gate in Node structure after reading .isc file
    //PrintGats(Node, Tgat);

    //Count the No of Pis and Pos
    CountPri(Node, Tgat, &Npi, &Npo);

    //Print the no of primary inputs and outputs
    printf("\n\nNpi: %d Npo: %d\n\n", Npi, Npo);
    /***************************************************************************************************/

    //File pointer to open .pattern file
    Pat = fopen(argv[2], "r");

    //File pointer to open .result file
    //Res = fopen(argv[3], "w");

    patternSim(Node, Pat, Npi, Npo, Tgat);

    fclose(Pat);
    //fclose(Res);

    //freePathSet();

    /***************************************************************************************************/
    //Checking any unreferenced bdds in manager
    printf("\nNo of Unreferenced Zdds: %d\n", Cudd_CheckZeroRef(manager));

    //Closing the cudd package manager
    Cudd_Quit(manager);

    //Clear memeory for all members of Node
    ClearGat(Node, Tgat);

    free(Node);
    FreeInputOutputArrays();

    printf("Done\n");
    return 0;
}//end of main
/*******************************************************************************************************/
