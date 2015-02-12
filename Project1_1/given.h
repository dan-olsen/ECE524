#ifndef GIVEN_H_
#define GIVEN_H_

/***************************************************************************************************************************
Header Files
****************************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <assert.h>
#include <limits.h>
/***************************************************************************************************************************
CUDD header files
****************************************************************************************************************************/
#include "util.h"
#include "cudd.h"
#include "cuddInt.h"
#include "dddmp.h"
/***************************************************************************************************************************
Constant Declarations 
****************************************************************************************************************************/
/***************************************************************************************************************************
Constant Declarations 
****************************************************************************************************************************/
// VARIOUS CONSTANTS
#define Mfnam      20			// max size for a file name
#define Mnod    15000			// max number of nodes in a graph/node
#define Mlin      200			// max size of characters in a line
#define Mnam       25			// max size of a node name
#define Mtyp       10			// max type of nodes/gates
#define Mout       16			// max node out degree (nfo)
#define Min         9			// max node in degree (nfi)
#define Mpi       233			// max number of primary inputs
#define Mpo       140			// max number of primary outputs
// FILE MODE CONSTANTS 
#define Fred 0			        // file READ mode ==> "r"
#define Fnew 1			        // file WRITE mode ==> "w"
#define Fapp 2			        // file APPEND mode ==> "a"
// NODE TYPE CONSTANTS 
#define INPT 1					// Primary Input
#define AND  2					// AND 
#define NAND 3					// NAND 
#define OR   4					// OR 
#define NOR  5					// NOR 
#define XOR  6					// XOR 
#define XNOR 7					// XNOR 
#define BUFF 8					// BUFFER 
#define NOT  9					// INVERTER 
#define FROM 10					// STEM BRANCH
/***************************************************************************************************************************
Cudd Declarations 
****************************************************************************************************************************/
DdManager *manager;        //Manager for Cudd Package
static int view_number = 0;  //Global Variable used by Manager
/***************************************************************************************************************************
Structure Declarations 
****************************************************************************************************************************/
//1.Stucture declaration for LIST
typedef struct LIST_type {
   int  id;		   //id for current element		
   struct LIST_type *nxt;  //pointer to next id element( if there is no element, then it will be NULL)		
} LIST;
//2.Stucture declaration for NODE
typedef struct NODE_type
{
  char nam[Mnam];                 //name of the node
  int typ,nfi,nfo,mar,po;    //type, no of fanins, no of fanouts, mark, primary output
  LIST *fin,*fot;            //fanin members, fanout members 
  DdNode *fun;              //BDDS of function
  double npb,bpb;           //Signal Probality in netlist and bdd
} NODE;
/***************************************************************************************************************************
Functions in given.c
****************************************************************************************************************************/
/***************************************************************************************************************************
LIST Structure Functions
****************************************************************************************************************************/
void InsertList(LIST **,int);
void FormList(int *,int,LIST **);
void PrintList(LIST *);
int  ListSize(LIST *);
int  InList(LIST *,int);
void DeleteNoList(LIST **, int);
void CopyList(LIST *,LIST **);
void FreeList(LIST **);
/***************************************************************************************************************************
 NODE Structure Functions
****************************************************************************************************************************/
int ReadIsc(FILE *,NODE *);
void InitializeCircuit(NODE *,int);
int AssignType(char *);
void PrintCircuit(NODE *,int);
void ClearCircuit(NODE *);
/***************************************************************************************************************************
 Demo Functions  
****************************************************************************************************************************/
void DemoList(char *);
void DemoBdd(DdManager *);
/****************************************************************************************************************************/
#endif /* GIVEN_H_ */
